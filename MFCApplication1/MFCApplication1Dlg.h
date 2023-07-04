
// MFCApplication1Dlg.h: 헤더 파일
//

#pragma once
#include <iostream>
#include <queue>
#include <Windows.h>
#include "afxdialogex.h"
#include "software.h"
#include "simple_gui.h"
#include "guiProcess.h"
using namespace std;

// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	
	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HBITMAP m_hBitmap;
	BITMAP m_bitmap;
	CBrush m_bk_brush;
	COLORREF myColor;
	CListCtrl m_running_processList;
	CListCtrl m_processList;
	CListCtrl m_processResult;
	CListCtrl m_coreList;
	CString m_processName;
	
	vector<GuiProcess> processList;
	vector<int> coreList;
	CEdit m_total_power;
	CEdit m_process_name_control;
	CEdit m_burst_time_control;
	CEdit m_arrival_time_control;
	CEdit m_time_quantum_control;
	CEdit m_time_quantum2_control;
	int m_arrivalTime;
	int m_burstTime;
	int m_cpuType;
	int m_algorithm;
	int m_time_quantum;  // RR 전용 time quantum
	int m_time_quantum2; // CBTA 전용 critical time
	double m_power_consumption;
	
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMCustomdrawCoreList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawProcessList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawProcessResult(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedProcessClearBtn();
	afx_msg void OnBnClickedProcessInterBtn();
	afx_msg void OnBnClickedCoreInterBtn();
	afx_msg void OnBnClickedCoreClearBtn();
	afx_msg void OnBnClickedSchedulingRr();
	afx_msg void OnBnClickedProcessRandomBtn();
	afx_msg void OnBnClickedProcessRandomHelp();
	afx_msg void OnBnClickedRunBtn();
	afx_msg void OnBnClickedPauseBtn();
	afx_msg void OnBnClickedPauseStartBtn();
	afx_msg void OnBnClickedStop();
	void movingScroll();

	CStatic m_static_os;
	CStatic m_static_algo;
	CStatic m_static_simulator;
	CStatic m_static_team;
	CStatic m_static_koraetech;
	CButton m_ctrl_fcfs;
	CButton m_ctrl_rr;
	CButton m_ctrl_spn;
	CButton m_ctrl_strn;
	CButton m_ctrl_hrrn;
	CButton m_ctrl_udsm;
	CButton m_ctrl_p;
	CButton m_ctrl_e;
	CStatic m_static_total_power;
	CStatic m_static_name;
	CStatic m_static_at;
	CStatic m_static_bt;
	CStatic m_static_algo_group;
	CStatic m_static_algo_select;
	CStatic m_static_process_reg;
	CStatic m_static_process_info;
	CStatic m_static_processor_reg;
	CStatic m_static_scheduling;
	CBitmapButton m_btn_process_inter;
	CBitmapButton m_btn_process_clear;
	CBitmapButton m_btn_core_inter;
	CBitmapButton m_btn_core_clear;
	CBitmapButton m_btn_process_random;
	CBitmapButton m_process_random_help;
	CBitmapButton m_run_btn;
	CBitmapButton m_pause_btn;
	CBitmapButton m_stop_btn;
	CBitmapButton m_pause_start_btn;
};
