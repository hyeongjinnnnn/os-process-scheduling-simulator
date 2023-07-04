#include "pch.h"
#include "MFCApplication1.h"
#include <vector>
#include <atlstr.h>
#include "GuiProcess.h"

using namespace std;

GuiProcess::GuiProcess(CString name, int arrivalTime, int burstTime) {
	random_device rd;
	mt19937 gen(rd());
	this->name = name;
	this->arrivalTime = arrivalTime;
	this->burstTime = burstTime;

	// ��� �۾��� �Ⱥ��� ���� �ִ� ���� ������ 255, 255, 255 ���� ��� 50 ������ ���� �������ݴϴ�.
	uniform_int_distribution<> dist(0, 205);
	processColor = RGB(dist(gen), dist(gen), dist(gen));
}

