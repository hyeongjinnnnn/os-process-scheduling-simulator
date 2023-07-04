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
    // ���μ��� ���̺� ���� �� ���μ��� ���
    process_table = new PCB[N];
    for (int i = 0; i < N; i++)
        process_table[i].registProcess(at[i], bt[i]);

    // ����ť ����
    RQ = new Que(N);
    // �����ھ�ť ����
    RPQ = new Que(P);
    // ���μ��� ����
    processor = new Processor(P);
    processor->setType(core_types);
    // �����ٷ� ����
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
    // running_process�� ���� �ܰ迡���� �� cpu�� ���� ���¸� ��Ÿ���� �迭
    // cpu_restart�� �̹� �ܰ迡�� cpu�� ��õ� ���θ� ��Ÿ�� �迭
    int p, c;

    RPQ->clear();                               // �����ھ� ť �ʱ�ȭ
    for (int i = 0; i < N; i++)                 // �������μ��� �迭 �ʱ�ȭ
        newly_terminated_process[i] = 0;

    // ����� ���μ��� ���� Ȯ���Ͽ� cpu, PCB ����
    for (int i = 0; i < P; i++) {   // ���μ����� 0~(P-1)�� �ھ ���Ͽ�
        cpu_restarted[i] = 0;

        p = processor->core[i].allocated_process;           // p: i��° �ھ �Ҵ�Ǿ��ִ� ���μ��� ��ȣ
        if ((process_table[p].RT <= 0) && (p != CPU_NO_PROCESS)) {    //p�� ���μ����� ���� ��Ȳ�̸�
            process_table[p].state = PROCESS_STATE_TERMINATED;
            newly_terminated_process[p] = 1;
            processor->core[i].allocated_process = CPU_NO_PROCESS;
        }
    }

    if (scheduler->preemptive == true) {
        // i.���� ����� �����ٸ� �˰���

        // ����ť�� running ������ ���μ����� ��� ��ť
        for (int i = 0; i < P; i++) {
            if (processor->core[i].allocated_process != CPU_NO_PROCESS) {
                RQ->enque(processor->core[i].allocated_process);
            }
        }

        // ���μ��� �����ٸ�: ���μ����� ����ġ�� ����ϰ� ����ġ�� ���� ����ť�� ������
        scheduler->setWeight(process_table, weight, N, clk);
        RQ->sort(weight);

        // allocatable_processor, high_process �ʱ�ȭ
        int* allocatable_processor = new int[P];
        int* high_process = new int[P];
        int hp = 0;	                        // high_process ���� ����

        for (int i = 0; i < P; i++) {
            allocatable_processor[i] = 0;
            high_process[i] = -1;
        }


        // ����ť���� �ִ� P������ ��ť�Ͽ� allocatable_processor, high_process�� ä��
        bool allocated;
        for (int i = 0; i < P; i++) {
            allocated = false;
            if (!RQ->is_empty()) {
                p = RQ->deque();

                for (int j = 0; j < P; j++) {
                    // �ش� ���μ����� ���� Ŭ���� �Ҵ�Ǿ� �־��ٸ�, 
                    // j��° ���μ����� ���Ҵ��� �������� ������ ǥ��
                    if (processor->core[j].allocated_process == p) {
                        allocatable_processor[j] = -1;
                        allocated = true;
                        break;
                    }
                }

                // �ش� ���μ����� ���� Ŭ���� �Ҵ�Ǿ� ���� �ʾҴٸ�
                // i��° ���μ����� ���� �Ҵ��ؾ��ϴ� ���μ��� ��Ͽ� �߰�
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
        // ii. ���� ����� �����ٸ� �˰��� 
        for (int i = 0; i < P; i++) {
            if (processor->core[i].allocated_process == CPU_NO_PROCESS) {
                RPQ->enque(i);
            }
        }

        scheduler->setWeight(process_table, weight, N, clk);
        RQ->sort(weight);

        while (!RPQ->is_empty()) {
            if (RQ->is_empty()) { break; }
            c = RPQ->deque();     // c: �ָ��ϴ� �ھ��� ��ȣ
            p = RQ->deque();      // p: �Ҵ��� ���μ��� ��ȣ

            processor->core[c].allocated_process = p;
            process_table[p].state = PROCESS_STATE_RUNNING;
        }
    }

    // ��õ��� CPU ��ȣ�� ������ ���μ����� ���
    for (int i = 0; i < P; i++) {
        if ((running_process[i] == CPU_NO_PROCESS) && (processor->core[i].allocated_process != CPU_NO_PROCESS))
            cpu_restarted[i] = 1;
        running_process[i] = processor->core[i].allocated_process;
    }
    return;
}

// RT, WT, TT, NTT ���
void Kernel::proceed(std::vector<int>& running_process, std::vector<int>& cpu_restarted) {
    int p;
    for (int i = 0; i < P; i++) {
        if (cpu_restarted[i]) {
            processor->core[i].activate();
        }
        p = running_process[i];
        if (p != CPU_NO_PROCESS) {
            processor->core[i].total_consumption += processor->core[i].regular_consumption;      // �⺻ �Һ� ���� �Ҹ�
            process_table[p].RT -= processor->core[i].work_rate;              // ���μ��� ó��(RT ����)
            process_table[p].actual_working_time += 1;                        // ���� ���� �ð� ����
            if ((process_table[p].RT <= 0) && (process_table[p].TT == PCB_UNCERTAIN) && (process_table[p].WT == PCB_UNCERTAIN)) {
                process_table[p].TT = clk + 1 - process_table[p].AT;          // ó���� ���� ���μ����� TT�� ���
                process_table[p].WT = process_table[p].TT - process_table[p].actual_working_time;
                process_table[p].NTT = (double)process_table[p].TT / (double)process_table[p].actual_working_time;
                // ó���� ���� ���μ����� NTT�� ���
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