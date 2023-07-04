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
    // Wating Time / Turn-around Time / Remaining Time(�ܿ� �۾� �ð�) / Nomalized Turn-around Time
    int actual_working_time;
    // ������ ó���� �ð��� ������ ��Ÿ���ϴ�. (= actual burst time)
    int state;
    // ���μ��� ���¸� ��Ÿ���ϴ�. PROCESS_STATE_CREATED ������ ����� ����Ͽ� ��Ÿ���ϴ�.

public:
    // PCB Ŭ���� �������Դϴ�. 
    PCB();
    // PCB�� ���μ����� ������ ����մϴ�.
    void registProcess(int at, int bt);
};


class Que {
public:
    int capacity;      // �迭 ũ��
    int num;           // �迭�� ����� �ڷ��� ũ��
    int* body;         // �ڷḦ ������ �迭
    int head;          // ������ ��ť�� ������ �ε���
    int tail;          // ������ ��ť�� ������ ������ �ε���


public:
    Que(int cap);
    ~Que(void);

    // ť�� ���� �� ��� true�� ��ȯ�մϴ�.
    bool is_full();
    // ť�� ��������� true�� ��ȯ�մϴ�.
    bool is_empty();
    // �־��� weights(= �� ������ ����ġ�� �迭�� ������ ��)�� ���� ť�� ������ �����մϴ�.
    void sort(std::vector<double>& weights);
    void sort(double* weights);
    // �־��� ������ ��ť�մϴ�.
    void enque(int p);
    // ť�� tail���� ���Ҹ� ��ť�մϴ�. 
    int deque();
    // ť �ȿ� ������ ���� �ִ��� �Ǵ��մϴ�.
    bool has(int p);
    // ť�� ���빰�� ���
    void clear();
};


class Scheduler {
public:
    // preemptive�� ���� ���� ���θ� ��Ÿ���� �Ӽ��Դϴ�. ���� ������ �����ٸ� �޼ҵ��� ��� True�� �����մϴ�.
    bool preemptive;

public:
    // �����ٷ� Ŭ������ �������Դϴ�. �ʿ��� ��� �������̵� �Ͽ� ����Ͻʽÿ�..
    Scheduler();
    // �Ӽ� �������Դϴ�. �����ӰԵ�, �Ʒ����� �Ӽ� �ʱ�ȭ�� �ϰ� �ƽ��ϴ�.
    virtual void setAttr(int* settings) {}
    // �����ٸ����� ����� ����ġ�� �迭�� weights�� �����ϴ� �Լ��Դϴ�.
    virtual void setWeight(PCB* process_table, std::vector<double>& weights, int n, int clk) {}
    // Kernel::afterProcess���� ����ϴ� �Լ��Դϴ�. ������ �������̵��ؼ� ����Ͻø� �˴ϴ�.
    void afterProcess();
};


class FCFS : public Scheduler {
    // ��� ������ ���⿡�� �����մϴ�. 
public:
    bool done_once;


    // ��� �Լ��� ���⿡�� �����մϴ�. 
public:
    // FCFS Ŭ������ �������Դϴ�. preemptive ����, time quantum ��, 
    // �ʿ��� �Ӽ��� ����ų� �ʱ�ȭ�մϴ�.
    FCFS() {
        this->preemptive = false;
        this->done_once = false;
    };

    // FCFS�� ����ġ ���� �Լ��Դϴ�. 
    // process_table: PCB �� ������ �迭�Դϴ�. ��, process_table[i]�� i��° ���μ����� PCB�� ��Ÿ���ϴ�. process_table[i].AT ���� �Ӽ��� �����Ӱ� ����� �� �ֽ��ϴ�.
    //               �ٸ�, process_table[i]�� ���� �������� �ʵ��� �����մϴ�.
    // weight: weight[i]�� i��° ���μ����� ����ġ�� ��Ÿ���ϴ�. ������ ���� ȣ��Ǹ�, �� �Լ��� ������ �� weight�� ���Ҹ� �����ϰ� �������ִ� ���Դϴ�. 
    // n: PCB�� ����(= ���μ����� ����)�Դϴ�.
    void setWeight(PCB* process_table, std::vector<double>& weight, int n, int clk) {
        if (done_once == true)
            return;

        for (int i = 0; i < n; i++) {
            weight[i] = -double(process_table[i].AT) - (double)(i + 1) / (n + 1);
            // i/n�� ���ִ� ������ ������ AT�� ���� �� ���μ����� ������ ���� ���Ŀ� ���Ͽ� �Ź� �ڹٲ�� ��Ȳ�� ���� �����Դϴ�.
        }
        done_once = true;
        return;
    }
};


class RR : public Scheduler {
public:
    int N;                    // ���μ��� ����
    int P;                    // ���μ��� ����
    int* tc;                  // Ÿ�� ī����. ���μ��� ���� ���� Ƚ�� Ȯ��
    int tq;                   // Ÿ�� ����
    Que* SRQ;                 // sub_readyqueue

public:
    RR(int delta, int n, int p) : N(n), P(p) {
        this->preemptive = true;        // ���� ���ɼ�
        this->tc = new int[N];          // �� ���μ����� ó�� Ƚ�� Ȯ�ο� �迭
        this->tq = delta;               // Ÿ�� ����
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

            // ���� arrival�� ���μ����� SRQ�� ��ť
            if (clk == process_table[i].AT)
                SRQ->enque(i);

            // ���μ����� �Ҵ�� ���μ����� tc ++
            if (process_table[i].state == PROCESS_STATE_RUNNING)
                tc[i]++;

            // rp=���μ��� �Ҵ��� ����� �ʿ䰡 ���� ���μ����� ����.
            // ���� Ŭ������ ����� ���μ����� �������� �Ѵ�. 
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
    int N;            // ���μ��� ����
    int P;            // ���μ��� ����
    int ct;           // �Ӱ� �ð�
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
    Que* RQ;                //����ť
    Que* RPQ;               // �����ھ� ť: Resting Processor Que
    PCB* process_table;
    Processor* processor;
    Scheduler* scheduler;

public:
    // Ŀ�� Ŭ������ �������Դϴ�.  
    // n: ���μ����� ���� / p: cpu �ھ��� ���� / sm: �����ٸ� �޼ҵ�
    // at[n]: ���μ����� ����̹� Ÿ��
    // bt[n]: ���μ����� cpu ����Ʈ Ÿ��
    // core_types: �ھ� Ÿ��
    // settings: �����ٷ��� �ɼ�
    Kernel(int n, int p, int sm, std::vector<int>& at, std::vector<int>& bt, std::vector<int>& core_types, int* settings);

    ~Kernel();

    // ���μ��� ���̺� ��ϵ� PCB���� ���� Ŭ���� ���μ����� AT�� ���մϴ�.
    // clk >=AT �̸鼭 CREATED ������ ���μ����� Ready ���·� ��ȯ�մϴ�.
    void arrivalCheck();

    // ���μ��� ���̺� ��ϵ� ��� ���μ����� terminated �����̸� True�� ��ȯ�մϴ�.
    bool allTerminated();

    // �����ٸ� ���͸� ���� �� ���μ����� ����ġ�� ���ϰ�, 
    // ����ġ�� ���� ����ť�� ������ �� ���μ����� CPU �ھ �Ҵ��մϴ�.
    void processSchedule(std::vector<int>& running_process, std::vector<int>& cpu_restarted, std::vector<int>& newly_terminated_process, std::vector<double>& weight);

    // �� CPU�� �Ҵ�� ���μ����� �����մϴ�.  
    void proceed(std::vector<int>& running_processes, std::vector<int>& cpu_restarted);

    // �����ٸ� ���� ���� �ʿ��� ��ó���� �����մϴ�.
    void afterProcess() { return; }

    // ���� ������� ���� ���μ����� �����ϴ� ��� ���� �޽��� �߰��Ͽ� ���μ����� 1�� �����մϴ�.
    // ������� ���� ���μ����� �����ϴ� ��� true��, �׷��� ���� ��� false�� ��ȯ�մϴ�.
    bool stepProcess(std::vector<int>& running_process, std::vector<int>& cpu_restarted, std::vector<int>& newly_terminated_process, std::vector<double>& weight);

    // �� ���μ����� ���¸� Ȯ���մϴ�.
    void get_process_state(std::vector<int>& process_state);
    // RT�� Ȯ���մϴ�.
    void get_remaining_time(std::vector<int>& remaining_time);
    // BT�� Ȯ���մϴ�.
    void get_actual_working_time(std::vector<int>& actual_working_time);
    // WT�� Ȯ���մϴ�.
    void get_wating_time(std::vector<int>& waiting_time);
    // TT�� Ȯ���մϴ�.
    void get_turn_around_time(std::vector<int>& turn_around_time);
    // NTT�� Ȯ���մϴ�.
    void get_normalized_turn_around_time(std::vector<double>& normalized_turn_around_time);
};







#endif