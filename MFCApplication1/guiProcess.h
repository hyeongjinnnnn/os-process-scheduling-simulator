#pragma once
#include <afx.h>
#include <random>

/* 
	����ڿ��� �Է¹޴� ���μ����Դϴ�. 
	�� ��ü�� ������� GuiProcess vector�� ���� �� 
	���� �ý���(PCB)�� ���μ��� ������ ����մϴ�.
*/
class GuiProcess {
public:
	CString name;
	int arrivalTime;
	int burstTime;
	COLORREF processColor;

	GuiProcess(CString name, int arrivalTime, int burstTime);
};