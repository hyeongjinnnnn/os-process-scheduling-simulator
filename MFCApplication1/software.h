#pragma once
#ifndef __SOFTWARE_HPP__
#define __SOFTWARE_HPP__

#define SCHEDULING_FCFS   0
#define SCHEDULING_RR     1
#define SCHEDULING_SPN    2
#define SCHEDULING_SRTN   3
#define SCHEDULING_HRRN   4
#define SCHEDULING_CBTA   5

#define PROCESS_STATE_NOT_LOADED    -1
#define PROCESS_STATE_CREATED        0
#define PROCESS_STATE_READY          1
#define PROCESS_STATE_RUNNING        2
#define PROCESS_STATE_TERMINATED     3

#include "hardware.h"


class PCB {
public:
    int AT;   int BT;
    // Arrival Time / Bust Time
    int WT;   int TT;      int RT;   double NTT;
    // Wating Time / Turn-around Time / Remaining Time(잔여 작업 시간) / Nomalized Turn-around Time
    int actual_working_time;
    // 실제로 처리된 시간의 총합을 나타냅니다. (= actual burst time)
    int state;
    // 프로세스 상태를 나타냅니다. PROCESS_STATE_CREATED 따위의 상수를 사용하여 나타냅니다.

public:
    // PCB 클래스 생성자입니다. 
    PCB();
    // PCB에 프로세스의 정보를 등록합니다.
    void registProcess(int at, int bt);
};


class Que {
public:
    int capacity;      // 배열 크기
    int num;           // 배열에 저장된 자료의 크기
    int* body;         // 자료를 저장할 배열
    int head;          // 다음에 디큐할 정수의 인덱스
    int tail;          // 다음에 인큐할 정수를 저장할 인덱스


public:
    Que(int cap);
    ~Que(void);

    // 큐가 가득 찬 경우 true를 반환합니다.
    bool is_full();
    // 큐가 비어있으면 true를 반환합니다.
    bool is_empty();
    // 주어진 weights(= 각 원소의 가중치를 배열로 정리한 것)에 따라 큐의 내용을 정렬합니다.
    void sort(std::vector<double>& weights);
    void sort(double* weights);
    // 주어진 정수를 인큐합니다.
    void enque(int p);
    // 큐의 tail에서 원소를 디큐합니다. 
    int deque();
    // 큐 안에 지정한 값이 있는지 판단합니다.
    bool has(int p);
    // 큐의 내용물을 비움
    void clear();
};


class Scheduler {
public:
    // preemptive는 선점 가능 여부를 나타내는 속성입니다. 선점 가능한 스케줄링 메소드의 경우 True로 지정합니다.
    bool preemptive;

public:
    // 스케줄러 클래스의 생성자입니다. 필요한 경우 오버라이딩 하여 사용하십시오..
    Scheduler();
    // 속성 설정자입니다. 공교롭게도, 아래에서 속성 초기화를 하게 됐습니다.
    virtual void setAttr(int* settings) {}
    // 스케줄링에서 사용할 가중치의 배열인 weights를 결정하는 함수입니다.
    virtual void setWeight(PCB* process_table, std::vector<double>& weights, int n, int clk) {}
    // Kernel::afterProcess에서 사용하는 함수입니다. 적절히 오버라이딩해서 사용하시면 됩니다.
    void afterProcess();
};


class FCFS : public Scheduler {
    // 멤버 변수는 여기에서 정의합니다. 
public:
    bool done_once;


    // 멤버 함수는 여기에서 정의합니다. 
public:
    // FCFS 클래스의 생성자입니다. preemptive 설정, time quantum 등, 
    // 필요한 속성을 만들거나 초기화합니다.
    FCFS() {
        this->preemptive = false;
        this->done_once = false;
    };

    // FCFS의 가중치 설정 함수입니다. 
    // process_table: PCB 로 구성된 배열입니다. 즉, process_table[i]는 i번째 프로세스의 PCB를 나타냅니다. process_table[i].AT 등의 속성을 자유롭게 사용할 수 있습니다.
    //               다만, process_table[i]의 값은 변경하지 않도록 주의합니다.
    // weight: weight[i]는 i번째 프로세스의 가중치를 나타냅니다. 참조에 의해 호출되며, 본 함수의 목적은 이 weight의 원소를 적절하게 변경해주는 것입니다. 
    // n: PCB의 개수(= 프로세스의 개수)입니다.
    void setWeight(PCB* process_table, std::vector<double>& weight, int n, int clk) {
        if (done_once == true)
            return;

        for (int i = 0; i < n; i++) {
            weight[i] = -double(process_table[i].AT) - (double)(i + 1) / (n + 1);
            // i/n을 빼주는 이유는 동일한 AT를 갖는 두 프로세스의 순서가 버블 정렬에 의하여 매번 뒤바뀌는 상황을 막기 위함입니다.
        }
        done_once = true;
        return;
    }
};


class RR : public Scheduler {
public:
    int N;                    // 프로세스 개수
    int P;                    // 프로세서 개수
    int* tc;                  // 타임 카운터. 프로세스 연속 실행 횟수 확인
    int tq;                   // 타임 퀀텀
    Que* SRQ;                 // sub_readyqueue

public:
    RR(int delta, int n, int p) : N(n), P(p) {
        this->preemptive = true;        // 선점 가능성
        this->tc = new int[N];          // 각 프로세스별 처리 횟수 확인용 배열
        this->tq = delta;               // 타임 퀀텀
        for (int i = 0; i < N; i++)
            tc[i] = 0;

        this->SRQ = new Que(N);
        return;
    }

    ~RR() {
        delete[] tc;
        delete SRQ;
        return;
    }

    void setWeight(PCB* process_table, std::vector<double>& weight, int n, int clk) {
        int i = 0;
        int rp = 0;

        for (i = 0; i < N; i++) {

            // 새로 arrival한 프로세스를 SRQ에 인큐
            if (clk == process_table[i].AT)
                SRQ->enque(i);

            // 프로세서에 할당된 프로세스의 tc ++
            if (process_table[i].state == PROCESS_STATE_RUNNING)
                tc[i]++;

            // rp=프로세스 할당을 재고할 필요가 없는 프로세서의 개수.
            // 이전 클럭에서 종료된 프로세스도 고려해줘야 한다. 
            if ((tc[i] < tq) && (process_table[i].RT >= 0) && (process_table[i].state == PROCESS_STATE_RUNNING))
                rp++;
        }
        rp = P - rp;

        for (i = 0; i < N; i++) {
            if (process_table[i].state == PROCESS_STATE_TERMINATED) {
                tc[i] = 0;
                weight[i] = 0;
            }
            std::cout << tc[i] << std::endl;
            if (tc[i] >= tq) {
                tc[i] = 0;
                weight[i] = 0;
                SRQ->enque(i);
            }
        }

        for (i = 0; i < rp; i++) {
            if (SRQ->is_empty())
                return;

            weight[SRQ->deque()] = 1;
        }
        return;
    }
};

class SPN :public Scheduler {
public:
    SPN() {
        this->preemptive = false;
    }

    void setWeight(PCB* process_table, std::vector<double>& weight, int n, int clk) {
        for (int i = 0; i < n; i++) {
            weight[i] = -process_table[i].RT;
        }
    }
};

class SRTN : public Scheduler {
public:
    SRTN() {
        this->preemptive = true;
    }

    void setWeight(PCB* process_table, std::vector<double>& weight, int n, int clk) {
        for (int i = 0; i < n; i++) {
            weight[i] = -process_table[i].RT;
        }
    }
};

class HRRN : public Scheduler {
public:
    HRRN() {
        this->preemptive = false;
    }

    void setWeight(PCB* process_table, std::vector<double>& weight, int n, int clk) {
        for (int i = 0; i < n; i++) {
            weight[i] = (double)(clk - process_table[i].AT + process_table[i].BT) / (double)process_table[i].BT;
        }
    }
};


class CBTA : public Scheduler {
public:
    int N;            // 프로세스 개수
    int P;            // 프로세서 개수
    int ct;           // 임계 시간
    int done_once;

public:
    CBTA(int critical_time, int n, int p) {
        this->preemptive = true;
        this->N = n;
        this->P = p;
        this->ct = critical_time;
        this->done_once = false;
    }

    ~CBTA() {
    }

    void setWeight(PCB* process_table, std::vector<double>& weight, int n, int clk) {
        if (done_once)
            return;

        double largest_BT = 0;
        for (int i = 0; i < N; i++) {
            if (process_table[i].BT > largest_BT)
                largest_BT = process_table[i].BT;
        }

        for (int i = 0; i < N; i++) {
            if (process_table[i].BT <= ct)
                weight[i] = 1 + (double)ct - process_table[i].BT - double(i + 1) / double(n + 1);
            else
                weight[i] = process_table[i].BT - largest_BT - double(i + 1) / double(n + 1);

        }
        done_once = true;
        return;
    }
};




class Kernel {
public:
    int N;
    int P;
    int smethod;
    int clk;
    Que* RQ;                //레디큐
    Que* RPQ;               // 유휴코어 큐: Resting Processor Que
    PCB* process_table;
    Processor* processor;
    Scheduler* scheduler;

public:
    // 커널 클래스의 생성자입니다.  
    // n: 프로세스의 개수 / p: cpu 코어의 개수 / sm: 스케줄링 메소드
    // at[n]: 프로세스별 어라이벌 타임
    // bt[n]: 프로세스별 cpu 버스트 타임
    // core_types: 코어 타입
    // settings: 스케줄러별 옵션
    Kernel(int n, int p, int sm, std::vector<int>& at, std::vector<int>& bt, std::vector<int>& core_types, int* settings);

    ~Kernel();

    // 프로세스 테이블에 등록된 PCB에서 현재 클락과 프로세스별 AT를 비교합니다.
    // clk >=AT 이면서 CREATED 상태인 프로세스를 Ready 상태로 전환합니다.
    void arrivalCheck();

    // 프로세스 테이블에 등록된 모든 프로세스가 terminated 상태이면 True를 반환합니다.
    bool allTerminated();

    // 스케줄링 펑터를 통해 각 프로세스별 가중치를 정하고, 
    // 가중치에 따라 레디큐를 정렬한 후 프로세스를 CPU 코어에 할당합니다.
    void processSchedule(std::vector<int>& running_process, std::vector<int>& cpu_restarted, std::vector<int>& newly_terminated_process, std::vector<double>& weight);

    // 각 CPU가 할당된 프로세스를 실행합니다.  
    void proceed(std::vector<int>& running_processes, std::vector<int>& cpu_restarted);

    // 스케줄링 펑터 별로 필요한 뒷처리를 수행합니다.
    void afterProcess() { return; }

    // 아직 종료되지 않은 프로세스가 존재하는 경우 다음 펄스를 추가하여 프로세스를 1번 진행합니다.
    // 종료되지 않은 프로세스가 존재하는 경우 true를, 그렇지 않은 경우 false를 반환합니다.
    bool stepProcess(std::vector<int>& running_process, std::vector<int>& cpu_restarted, std::vector<int>& newly_terminated_process, std::vector<double>& weight);

    // 각 프로세스별 상태를 확인합니다.
    void get_process_state(std::vector<int>& process_state);
    // RT를 확인합니다.
    void get_remaining_time(std::vector<int>& remaining_time);
    // BT를 확인합니다.
    void get_actual_working_time(std::vector<int>& actual_working_time);
    // WT를 확인합니다.
    void get_wating_time(std::vector<int>& waiting_time);
    // TT를 확인합니다.
    void get_turn_around_time(std::vector<int>& turn_around_time);
    // NTT를 확인합니다.
    void get_normalized_turn_around_time(std::vector<double>& normalized_turn_around_time);
};







#endif