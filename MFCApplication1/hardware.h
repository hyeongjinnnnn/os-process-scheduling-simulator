#pragma once
#include <vector>
#ifndef __HARDWARE_HPP__
#define __HARDWARE_HPP__

#define CPU_TYPE_NONE	0
#define CPU_TYPE_P		1
#define CPU_TYPE_E		2

#define CPU_NO_PROCESS	-1


class Core{
public:
	int type;							// cpu 타입 (Perfomance 중심)
	int work_rate;						// 단위시간당 처리 속도 [작업/초]
	double regular_consumption;			// 일반 동작시 소비 전력
	double starting_consumption;		// 재시동시 소비 전력
	double total_consumption;			// 커널 구동 이례로 소비한 총 전력 크기
	int allocated_process;				// cpu에 할당된 프로세스 번호

public:
	// 코어 클래스의 생성자입니다. 멤버 속성은 전부 기본값으로 지정합니다.
	Core(void) : type(CPU_TYPE_NONE),
		work_rate(0),
		regular_consumption(0.0),
		starting_consumption(0.0),
		allocated_process(CPU_NO_PROCESS),
		total_consumption(0.0)
	{};

	// 지정된 type에 따라 멤버변수의 초기화를 진행합니다.
	// 총 소비 전력은 변경하지 않으므로 주의합니다.
	void setType(int type=CPU_TYPE_E);
	// 코어가 재시작하는 경우 총 소비 전력에 시동 전력을 추가.
	void activate();
};


class Processor {
public:
	int P;			// 사용 CPU 개수
	Core* core;		// CPU 코어

public:
	Processor(int p) :P(p) {
		core = new Core[P];
		return;
	}
	~Processor() {
		delete[] core;
		return;
	}

	void setType(std::vector<int>& cpu_type) {
		for (int i = 0; i < P; i++)
			core[i].setType(cpu_type[i]);
		return;
	}
	Core& operator[](int idx) {return core[idx];}
	void get_power_usage(std::vector<double>& power_usage);
};




#endif