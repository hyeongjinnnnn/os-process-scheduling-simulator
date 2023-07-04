#pragma once
#include <afx.h>
#include <random>

/* 
	사용자에게 입력받는 프로세스입니다. 
	이 객체를 기반으로 GuiProcess vector를 만든 후 
	내부 시스템(PCB)에 프로세스 정보를 등록합니다.
*/
class GuiProcess {
public:
	CString name;
	int arrivalTime;
	int burstTime;
	COLORREF processColor;

	GuiProcess(CString name, int arrivalTime, int burstTime);
};