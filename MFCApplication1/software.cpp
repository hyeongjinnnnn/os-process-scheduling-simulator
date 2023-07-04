#include <iostream>
#include "software.h"
#include <vector>
//#include "Windows.h"

#define PCB_UNCERTAIN   -1

#define QUE_NO_ENTITY   -1


PCB::PCB() :AT(PCB_UNCERTAIN), BT(PCB_UNCERTAIN), RT(PCB_UNCERTAIN),
WT(PCB_UNCERTAIN), TT(PCB_UNCERTAIN), NTT(PCB_UNCERTAIN),
actual_working_time(PROCESS_STATE_NOT_LOADED), state(PROCESS_STATE_NOT_LOADED)
{ }

void PCB::registProcess(int at, int bt) {
    AT = at;
    BT = bt;
    RT = bt;
    actual_working_time = 0;
    state = PROCESS_STATE_CREATED;
    return;
}


Que::Que(int cap) : capacity(cap), num(0), head(0), tail(0) {
    Que::body = new int[cap];
}

Que::~Que(void) { delete[] Que::body; }

bool Que::is_full() { return num == capacity; }

bool Que::is_empty() { return num == 0; }

void Que::enque(int p) {
    if (is_full())
        return;
    body[tail] = p;
    num++;
    tail = (tail + 1) % capacity;
    return;
}

int Que::deque() {
    if (is_empty())
        return QUE_NO_ENTITY;
    int r = body[head];
    head = (head + 1) % capacity;
    num--;
    return r;
}

void Que::sort(std::vector<double>& weights) {
    int i, j;
    int temp;
    for (i = 0; i < num; i++) {
        for (j = 0; j < num - i - 1; j++) {
            if (weights[body[(head + j) % capacity]] < weights[body[(head + j + 1) % capacity]]) {
                temp = body[(head + j) % capacity];
                body[(head + j) % capacity] = body[(head + j + 1) % capacity];
                body[(head + j + 1) % capacity] = temp;
            }
        }
    }
    return;
}

void Que::sort(double* weights) {
    int i, j;
    int temp;
    for (i = 0; i < num; i++) {
        for (j = 0; j < num - i - 1; j++) {
            if (weights[body[(head + j) % capacity]] < weights[body[(head + j + 1) % capacity]]) {
                temp = body[(head + j) % capacity];
                body[(head + j) % capacity] = body[(head + j + 1) % capacity];
                body[(head + j + 1) % capacity] = temp;
            }
        }
    }
    return;
}

bool Que::has(int p) {
    for (int i = 0; i < num; i++) {
        if (body[(i + head) % capacity] == p)
            return true;
    }
    return false;
}

void Que::clear() {
    while (!is_empty())
        deque();
    return;
}

Scheduler::Scheduler() : preemptive(false) {
    return;
}

void Scheduler::afterProcess() {
    return;
}

Kernel::Kernel(int n, int p, int sm, std::vector<int>& at, std::vector<int>& bt, std::vector<int>& core_types, int* settings) : N(n), P(p), smethod(sm), clk(-1) {
    // 프로세스 테이블 생성 및 프로세스 등록
    process_table = new PCB[N];
    for (int i = 0; i < N; i++)
        process_table[i].registProcess(at[i], bt[i]);

    // 레디큐 생성
    RQ = new Que(N);
    // 유휴코어큐 생성
    RPQ = new Que(P);
    // 프로세서 생성
    processor = new Processor(P);
    processor->setType(core_types);
    // 스케줄러 생성
    switch (smethod) {
    case SCHEDULING_FCFS:
        scheduler = new FCFS();
        break;
    case SCHEDULING_RR:
        scheduler = new RR(settings[0], settings[1], settings[2]);   // tq, n, p
        scheduler->setAttr(settings);
        break;
    case SCHEDULING_SPN:
        scheduler = new SPN();
        break;
    case SCHEDULING_SRTN:
        scheduler = new SRTN();
        break;
    case SCHEDULING_HRRN:
        scheduler = new HRRN();
        break;
    case SCHEDULING_CBTA:
        scheduler = new CBTA(settings[0], settings[1], settings[2]);
        break;
    default:
        scheduler = new SPN();
    }
    return;
}

Kernel::~Kernel() {
    delete[] process_table;
    delete RQ;
    delete RPQ;
    delete processor;
    delete scheduler;
    return;
}

void Kernel::arrivalCheck() {
    for (int i = 0; i < N; i++) {
        if ((process_table[i].state == PROCESS_STATE_CREATED) && (process_table[i].AT <= clk)) {
            process_table[i].state = PROCESS_STATE_READY;
            RQ->enque(i);
        }
    }
    return;
}

bool Kernel::allTerminated() {
    for (int i = 0; i < N; i++) {
        std::cout << i << ": ";
        if (process_table[i].state < PROCESS_STATE_TERMINATED) {
            std::cout << "not terminated" << std::endl;
            return false;
        }
        std::cout << "Terminated" << std::endl;
    }
    return true;
}

void Kernel::processSchedule(std::vector<int>& running_process, std::vector<int>& cpu_restarted, std::vector<int>& newly_terminated_process, std::vector<double>& weight) {
    // running_process는 이전 단계에서의 각 cpu의 동작 상태를 나타내는 배열
    // cpu_restart는 이번 단계에서 cpu의 재시동 여부를 나타낼 배열
    int p, c;

    RPQ->clear();                               // 유휴코어 큐 초기화
    for (int i = 0; i < N; i++)                 // 종료프로세스 배열 초기화
        newly_terminated_process[i] = 0;

    // 종료된 프로세서 유무 확인하여 cpu, PCB 정리
    for (int i = 0; i < P; i++) {   // 프로세서의 0~(P-1)번 코어에 대하여
        cpu_restarted[i] = 0;

        p = processor->core[i].allocated_process;           // p: i번째 코어에 할당되어있던 프로세스 번호
        if ((process_table[p].RT <= 0) && (p != CPU_NO_PROCESS)) {    //p번 프로세스가 끝난 상황이면
            process_table[p].state = PROCESS_STATE_TERMINATED;
            newly_terminated_process[p] = 1;
            processor->core[i].allocated_process = CPU_NO_PROCESS;
        }
    }

    if (scheduler->preemptive == true) {
        // i.선점 방식의 스케줄링 알고리즘

        // 레디큐에 running 상태의 프로세스를 모두 인큐
        for (int i = 0; i < P; i++) {
            if (processor->core[i].allocated_process != CPU_NO_PROCESS) {
                RQ->enque(processor->core[i].allocated_process);
            }
        }

        // 프로세스 스케줄링: 프로세서별 가중치를 계산하고 가중치에 따라 레디큐를 정렬함
        scheduler->setWeight(process_table, weight, N, clk);
        RQ->sort(weight);

        // allocatable_processor, high_process 초기화
        int* allocatable_processor = new int[P];
        int* high_process = new int[P];
        int hp = 0;	                        // high_process 원소 개수

        for (int i = 0; i < P; i++) {
            allocatable_processor[i] = 0;
            high_process[i] = -1;
        }


        // 레디큐에서 최대 P개까지 디큐하여 allocatable_processor, high_process를 채움
        bool allocated;
        for (int i = 0; i < P; i++) {
            allocated = false;
            if (!RQ->is_empty()) {
                p = RQ->deque();

                for (int j = 0; j < P; j++) {
                    // 해당 프로세스가 이전 클럭에 할당되어 있었다면, 
                    // j번째 프로세서는 재할당을 수행하지 않음을 표시
                    if (processor->core[j].allocated_process == p) {
                        allocatable_processor[j] = -1;
                        allocated = true;
                        break;
                    }
                }

                // 해당 프로세스가 이전 클럭에 할당되어 있지 않았다면
                // i번째 프로세스는 새로 할당해야하는 프로세스 목록에 추가
                if (!allocated) {
                    high_process[hp] = p;
                    hp++;
                }
            }
        }
       
        p = 0;
        for (c = 0; c < P; c++) {
            if (p >= hp)
                break;
            if (allocatable_processor[c] == -1)
                continue;
            if (processor->core[c].allocated_process != CPU_NO_PROCESS)
                process_table[processor->core[c].allocated_process].state = PROCESS_STATE_READY;
            process_table[high_process[p]].state = PROCESS_STATE_RUNNING;
            processor->core[c].allocated_process = high_process[p];
            p++;
        }
        delete[] allocatable_processor;
        delete[] high_process;
    }
    else {
        // ii. 비선점 방식의 스케줄링 알고리즘 
        for (int i = 0; i < P; i++) {
            if (processor->core[i].allocated_process == CPU_NO_PROCESS) {
                RPQ->enque(i);
            }
        }

        scheduler->setWeight(process_table, weight, N, clk);
        RQ->sort(weight);

        while (!RPQ->is_empty()) {
            if (RQ->is_empty()) { break; }
            c = RPQ->deque();     // c: 주목하는 코어의 번호
            p = RQ->deque();      // p: 할당할 프로세스 번호

            processor->core[c].allocated_process = p;
            process_table[p].state = PROCESS_STATE_RUNNING;
        }
    }

    // 재시동된 CPU 번호와 실행할 프로세스를 계산
    for (int i = 0; i < P; i++) {
        if ((running_process[i] == CPU_NO_PROCESS) && (processor->core[i].allocated_process != CPU_NO_PROCESS))
            cpu_restarted[i] = 1;
        running_process[i] = processor->core[i].allocated_process;
    }
    return;
}

// RT, WT, TT, NTT 계산
void Kernel::proceed(std::vector<int>& running_process, std::vector<int>& cpu_restarted) {
    int p;
    for (int i = 0; i < P; i++) {
        if (cpu_restarted[i]) {
            processor->core[i].activate();
        }
        p = running_process[i];
        if (p != CPU_NO_PROCESS) {
            processor->core[i].total_consumption += processor->core[i].regular_consumption;      // 기본 소비 전력 소모
            process_table[p].RT -= processor->core[i].work_rate;              // 프로세스 처리(RT 감소)
            process_table[p].actual_working_time += 1;                        // 실제 실행 시간 증가
            if ((process_table[p].RT <= 0) && (process_table[p].TT == PCB_UNCERTAIN) && (process_table[p].WT == PCB_UNCERTAIN)) {
                process_table[p].TT = clk + 1 - process_table[p].AT;          // 처리가 끝난 프로세스의 TT를 계산
                process_table[p].WT = process_table[p].TT - process_table[p].actual_working_time;
                process_table[p].NTT = (double)process_table[p].TT / (double)process_table[p].actual_working_time;
                // 처리가 끝난 프로세스의 NTT를 계산
            }
        }
    }
    return;
}

bool Kernel::stepProcess(std::vector<int>& running_process, std::vector<int>& cpu_restarted, std::vector<int>& newly_terminated_process, std::vector<double>& weight) {
    clk++;
    arrivalCheck();

    processSchedule(running_process, cpu_restarted, newly_terminated_process, weight);
    proceed(running_process, cpu_restarted);
    afterProcess();
    if (allTerminated())
        return true;
    return false;
}

void Kernel::get_process_state(std::vector<int>& process_state) {
    for (int i = 0; i < N; i++) {
        process_state[i] = process_table[i].state;
    }
    return;
}

void Kernel::get_remaining_time(std::vector<int>& remaning_time) {
    for (int i = 0; i < N; i++)
        remaning_time[i] = process_table[i].RT;
    return;
}

void Kernel::get_wating_time(std::vector<int>& waiting_time) {
    for (int i = 0; i < N; i++)
        waiting_time[i] = process_table[i].WT;
    return;
}

void Kernel::get_actual_working_time(std::vector<int>& actual_working_time) {
    for (int i = 0; i < N; i++)
        actual_working_time[i] = process_table[i].actual_working_time;
    return;
}

void Kernel::get_turn_around_time(std::vector<int>& turn_around_time) {
    for (int i = 0; i < N; i++)
        turn_around_time[i] = process_table[i].TT;
    return;
}

void Kernel::get_normalized_turn_around_time(std::vector<double>& normalized_turn_around_time) {
    for (int i = 0; i < N; i++)
        normalized_turn_around_time[i] = process_table[i].NTT;
    return;
}