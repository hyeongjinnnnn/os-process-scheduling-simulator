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
	int type;							// cpu Ÿ�� (Perfomance �߽�)
	int work_rate;						// �����ð��� ó�� �ӵ� [�۾�/��]
	double regular_consumption;			// �Ϲ� ���۽� �Һ� ����
	double starting_consumption;		// ��õ��� �Һ� ����
	double total_consumption;			// Ŀ�� ���� �̷ʷ� �Һ��� �� ���� ũ��
	int allocated_process;				// cpu�� �Ҵ�� ���μ��� ��ȣ

public:
	// �ھ� Ŭ������ �������Դϴ�. ��� �Ӽ��� ���� �⺻������ �����մϴ�.
	Core(void) : type(CPU_TYPE_NONE),
		work_rate(0),
		regular_consumption(0.0),
		starting_consumption(0.0),
		allocated_process(CPU_NO_PROCESS),
		total_consumption(0.0)
	{};

	// ������ type�� ���� ��������� �ʱ�ȭ�� �����մϴ�.
	// �� �Һ� ������ �������� �����Ƿ� �����մϴ�.
	void setType(int type=CPU_TYPE_E);
	// �ھ ������ϴ� ��� �� �Һ� ���¿� �õ� ������ �߰�.
	void activate();
};


class Processor {
public:
	int P;			// ��� CPU ����
	Core* core;		// CPU �ھ�

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