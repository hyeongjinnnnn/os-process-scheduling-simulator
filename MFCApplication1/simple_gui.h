#pragma once
#include "pch.h"
#include "MFCApplication1.h"
#include< vector>
#ifndef __SIMPLE_GUI_HPP__
#define __SIMPLE_GUI_HPP__


// 화면에 running_process를 출력합니다.
// p: 코어 개수
void DispRunningProcess(std::vector<int>& running_process, int p, int index);

// terminated_process에 새로 종료된 프로세스를 등록합니다.
// n: 프로세스 개수
void RegistTerminatedProcess(std::vector<int>& newly_terminated_process,
	std::vector<int>& terminated_process,
	std::vector<int>& actual_burst_time,
	std::vector<int>& waiting_time,
	std::vector<int>& turn_around_time,
	std::vector<double>& normalized_turn_around_time,
	int n);

//화면에 각 코어별 총 전력 사용량을 표시합니다.
// p: 코어 개수
void DispPowerConsumption(std::vector<double>& power_usage, int p);

#endif