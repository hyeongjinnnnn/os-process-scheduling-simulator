#pragma once
#include "pch.h"
#include "MFCApplication1.h"
#include< vector>
#ifndef __SIMPLE_GUI_HPP__
#define __SIMPLE_GUI_HPP__


// ȭ�鿡 running_process�� ����մϴ�.
// p: �ھ� ����
void DispRunningProcess(std::vector<int>& running_process, int p, int index);

// terminated_process�� ���� ����� ���μ����� ����մϴ�.
// n: ���μ��� ����
void RegistTerminatedProcess(std::vector<int>& newly_terminated_process,
	std::vector<int>& terminated_process,
	std::vector<int>& actual_burst_time,
	std::vector<int>& waiting_time,
	std::vector<int>& turn_around_time,
	std::vector<double>& normalized_turn_around_time,
	int n);

//ȭ�鿡 �� �ھ �� ���� ��뷮�� ǥ���մϴ�.
// p: �ھ� ����
void DispPowerConsumption(std::vector<double>& power_usage, int p);

#endif