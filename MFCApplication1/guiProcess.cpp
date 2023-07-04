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

	// 흰색 글씨가 안보일 수도 있는 것을 염려해 255, 255, 255 보다 모두 50 전으로 색을 선택해줍니다.
	uniform_int_distribution<> dist(0, 205);
	processColor = RGB(dist(gen), dist(gen), dist(gen));
}

