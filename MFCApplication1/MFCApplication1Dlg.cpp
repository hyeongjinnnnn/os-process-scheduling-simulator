
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "guiProcess.h"
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define THREAD_STOP    0
#define THREAD_RUNNING 1
#define THREAD_PAUSE   2

CWinThread* m_pThread;
int m_eThreadWork = THREAD_STOP;  // 스레드는 돌아가고 있지 않는 상태로 초기화 해줍니다.
int listview = 0;				  // 코어 리스트 컨트롤과 실행중인 프로세스 리스트 컨트롤을 맞추기 위한 전역 변수입니다.
CFont myFont1;
CFont myFont2;
CFont myFont3;
CFont myFont4;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
	, m_processName(_T("P0"))
	, m_arrivalTime(0)
	, m_burstTime(1)
	, m_cpuType(0)
	, m_algorithm(0)
	, m_time_quantum(2)
	, myColor(RGB(48, 51, 69))
	, m_time_quantum2(2)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_VIEW, m_running_processList);
	DDX_Control(pDX, IDC_PROCESS_LIST, m_processList);
	DDX_Text(pDX, IDC_PROCESS_NAME, m_processName);
	DDX_Text(pDX, IDC_ARRIVAL_TIME, m_arrivalTime);
	DDX_Text(pDX, IDC_BURST_TIME, m_burstTime);
	DDX_Control(pDX, IDC_PROCESS_RESULT, m_processResult);
	DDX_Radio(pDX, IDC_CPU_TYPE_SELECT, m_cpuType);
	DDX_Control(pDX, IDC_CORE_LIST, m_coreList);
	DDX_Control(pDX, IDC_POWER_CONSUMPTION, m_total_power);
	DDX_Control(pDX, IDC_PROCESS_NAME, m_process_name_control);
	DDX_Control(pDX, IDC_BURST_TIME, m_burst_time_control);
	DDX_Control(pDX, IDC_ARRIVAL_TIME, m_arrival_time_control);
	DDX_Radio(pDX, IDC_SCHEDULING_FCFS, m_algorithm);
	DDX_Text(pDX, IDC_TIME_QUANTUM, m_time_quantum);
	DDX_Control(pDX, IDC_TIME_QUANTUM, m_time_quantum_control);
	DDX_Control(pDX, IDC_PROCESS_RANDOM_HELP, m_process_random_help);
	DDX_Control(pDX, IDC_PROCESS_INTER_BTN, m_btn_process_inter);
	DDX_Control(pDX, IDC_STATIC_OS, m_static_os);
	DDX_Control(pDX, IDC_STATIC_ALGO, m_static_algo);
	DDX_Control(pDX, IDC_STATIC_Simulator, m_static_simulator);
	DDX_Control(pDX, IDC_STATIC_TEAM, m_static_team);
	DDX_Control(pDX, IDC_STATIC_KOREATECH, m_static_koraetech);
	DDX_Control(pDX, IDC_SCHEDULING_FCFS, m_ctrl_fcfs);
	DDX_Control(pDX, IDC_SCHEDULING_RR, m_ctrl_rr);
	DDX_Control(pDX, IDC_SCHEDULING_SPN, m_ctrl_spn);
	DDX_Control(pDX, IDC_SCHEDULING_SRTN, m_ctrl_strn);
	DDX_Control(pDX, IDC_SCHEDULING_HRRN, m_ctrl_hrrn);
	DDX_Control(pDX, IDC_SCHEDULING_UDSM, m_ctrl_udsm);
	DDX_Control(pDX, IDC_CPU_TYPE_SELECT, m_ctrl_p);
	DDX_Control(pDX, IDC_RADIO2, m_ctrl_e);
	DDX_Control(pDX, IDC_TOTAL_POWER, m_static_total_power);
	DDX_Control(pDX, IDC_STATIC_NAME, m_static_name);
	DDX_Control(pDX, IDC_STATIC_AT, m_static_at);
	DDX_Control(pDX, IDC_STATIC_BT, m_static_bt);
	DDX_Control(pDX, IDC_STATIC_ALGO_SELECT, m_static_algo_select);
	DDX_Control(pDX, IDC_STATIC_PROCESS_REG, m_static_process_reg);
	DDX_Control(pDX, IDC_STATIC_PROCESS_INFO, m_static_process_info);
	DDX_Control(pDX, IDC_STATIC_PROCESSOR_REG, m_static_processor_reg);
	DDX_Control(pDX, IDC_STATIC_SCHEDULING, m_static_scheduling);
	DDX_Control(pDX, IDC_PROCESS_CLEAR_BTN, m_btn_process_clear);
	DDX_Control(pDX, IDC_CORE_INTER_BTN, m_btn_core_inter);
	DDX_Control(pDX, IDC_CORE_CLEAR_BTN, m_btn_core_clear);
	DDX_Control(pDX, IDC_PROCESS_RANDOM_BTN, m_btn_process_random);
	DDX_Control(pDX, IDC_RUN_BTN, m_run_btn);
	DDX_Control(pDX, IDC_PAUSE_BTN, m_pause_btn);
	DDX_Control(pDX, IDC_STOP_BTN, m_stop_btn);
	DDX_Control(pDX, IDC_PAUSE_START_BTN, m_pause_start_btn);
	DDX_Text(pDX, IDC_TIME_QUANTUM2, m_time_quantum2);
	DDX_Control(pDX, IDC_TIME_QUANTUM2, m_time_quantum2_control);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_VIEW, OnCustomdrawList)
	ON_BN_CLICKED(IDC_RUN_BTN, &CMFCApplication1Dlg::OnBnClickedRunBtn)
	ON_BN_CLICKED(IDC_PROCESS_INTER_BTN, &CMFCApplication1Dlg::OnBnClickedProcessInterBtn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROCESS_LIST, &CMFCApplication1Dlg::OnNMCustomdrawProcessList)
	ON_BN_CLICKED(IDC_PROCESS_CLEAR_BTN, &CMFCApplication1Dlg::OnBnClickedProcessClearBtn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROCESS_RESULT, &CMFCApplication1Dlg::OnNMCustomdrawProcessResult)
	ON_BN_CLICKED(IDC_CORE_INTER_BTN, &CMFCApplication1Dlg::OnBnClickedCoreInterBtn)
	ON_BN_CLICKED(IDC_CORE_CLEAR_BTN, &CMFCApplication1Dlg::OnBnClickedCoreClearBtn)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_CORE_LIST, &CMFCApplication1Dlg::OnNMCustomdrawCoreList)
	ON_BN_CLICKED(IDC_SCHEDULING_RR, &CMFCApplication1Dlg::OnBnClickedSchedulingRr)
	ON_BN_CLICKED(IDC_PROCESS_RANDOM_BTN, &CMFCApplication1Dlg::OnBnClickedProcessRandomBtn)
	ON_BN_CLICKED(IDC_PROCESS_RANDOM_HELP, &CMFCApplication1Dlg::OnBnClickedProcessRandomHelp)
	ON_BN_CLICKED(IDC_PAUSE_BTN, &CMFCApplication1Dlg::OnBnClickedPauseBtn)
	ON_BN_CLICKED(IDC_STOP_BTN, &CMFCApplication1Dlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_PAUSE_START_BTN, &CMFCApplication1Dlg::OnBnClickedPauseStartBtn)
END_MESSAGE_MAP()

// CMFCApplication1Dlg 메시지 처리기
BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			
	SetIcon(m_hIcon, FALSE);		
	

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	/* 배경, 폰트, 리스트 초기화 작업입니다. */
	m_bk_brush.CreateSolidBrush(RGB(36, 38, 51));
	m_hBitmap = LoadBitmap(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDB_BITMAP4));
	GetObject(m_hBitmap, sizeof(BITMAP), &m_bitmap);
	m_btn_process_inter.LoadBitmaps(IDB_BITMAP12, IDB_BITMAP13);
	m_btn_process_inter.SizeToContent();
	m_btn_process_clear.LoadBitmaps(IDB_BITMAP14, IDB_BITMAP15);
	m_btn_process_clear.SizeToContent();
	m_btn_core_clear.LoadBitmaps(IDB_BITMAP14, IDB_BITMAP15);
	m_btn_core_clear.SizeToContent();
	m_btn_core_inter.LoadBitmaps(IDB_BITMAP12, IDB_BITMAP13);
	m_btn_core_inter.SizeToContent();
	m_btn_process_random.LoadBitmaps(IDB_BITMAP8, IDB_BITMAP9);
	m_btn_process_random.SizeToContent();
	m_process_random_help.LoadBitmaps(IDB_BITMAP10, IDB_BITMAP11);
	m_process_random_help.SizeToContent();
	m_run_btn.LoadBitmaps(IDB_BITMAP19, IDB_BITMAP20);
	m_run_btn.SizeToContent();
	m_pause_btn.LoadBitmaps(IDB_BITMAP21, IDB_BITMAP22);
	m_pause_btn.SizeToContent();
	m_pause_start_btn.LoadBitmaps(IDB_BITMAP25, IDB_BITMAP26);
	m_pause_start_btn.SizeToContent();
	m_stop_btn.LoadBitmaps(IDB_BITMAP23, IDB_BITMAP24);
	m_stop_btn.SizeToContent();
	m_total_power.ModifyStyle(0, ES_CENTER, SWP_DRAWFRAME);
	myFont1.CreatePointFont(260, "Bahnschrift SemiBold Condensed");
	m_static_os.SetFont(&myFont1, TRUE);
	m_static_simulator.SetFont(&myFont1, TRUE);
	myFont2.CreatePointFont(400, "Bahnschrift SemiBold Condensed");
	m_static_algo.SetFont(&myFont2, TRUE);
	myFont3.CreatePointFont(210, "Bahnschrift SemiBold Condensed");
	m_static_team.SetFont(&myFont3, TRUE);
	m_static_koraetech.SetFont(&myFont3, TRUE);
	myFont4.CreatePointFont(180, "Bahnschrift SemiBold Condensed");
	m_ctrl_fcfs.SetFont(&myFont4, TRUE);
	m_ctrl_rr.SetFont(&myFont4, TRUE);
	m_ctrl_spn.SetFont(&myFont4, TRUE);
	m_ctrl_strn.SetFont(&myFont4, TRUE);
	m_ctrl_hrrn.SetFont(&myFont4, TRUE);
	m_ctrl_udsm.SetFont(&myFont4, TRUE);
	m_ctrl_p.SetFont(&myFont4, TRUE);
	m_ctrl_e.SetFont(&myFont4, TRUE);
	m_static_total_power.SetFont(&myFont4, TRUE);
	m_static_name.SetFont(&myFont4, TRUE);
	m_static_at.SetFont(&myFont4, TRUE);
	m_static_bt.SetFont(&myFont4, TRUE);
	m_static_algo_select.SetFont(&myFont3, TRUE);
	m_static_process_reg.SetFont(&myFont3, TRUE);
	m_static_process_info.SetFont(&myFont3, TRUE);
	m_static_processor_reg.SetFont(&myFont3, TRUE);
	m_static_scheduling.SetFont(&myFont3, TRUE);
	
	m_processList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_processResult.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_coreList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_running_processList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_running_processList.InsertColumn(0, _T(""), LVCFMT_RIGHT, 0, 1);
	
	m_processList.SetBkColor(myColor);
	m_processList.SetTextBkColor(myColor);
	m_processList.SetTextColor(RGB(255, 255, 255));
	
	m_processResult.SetBkColor(myColor);
	m_processResult.SetTextBkColor(myColor);
	m_processResult.SetTextColor(RGB(255, 255, 255));
	m_coreList.SetBkColor(myColor);
	m_coreList.SetTextBkColor(myColor);
	m_coreList.SetTextColor(RGB(255, 255, 255));
	m_running_processList.SetBkColor(myColor);
	m_running_processList.SetTextBkColor(myColor);
	m_running_processList.SetTextColor(RGB(255, 255, 255));
	
	/* 정수로 입력받아야 하는 Edit Control은 숫자만 받도록 합니다. */
	m_time_quantum_control.ModifyStyle(0, ES_NUMBER);
	m_time_quantum2_control.ModifyStyle(0, ES_NUMBER);
	m_arrival_time_control.ModifyStyle(0, ES_NUMBER);
	m_burst_time_control.ModifyStyle(0, ES_NUMBER);

	CRect processListRect;
	m_processList.GetClientRect(processListRect);
	m_processList.InsertColumn(0, _T("Process Name"), 0, processListRect.Width() / 3);
	m_processList.InsertColumn(1, _T("Arrival Time"), 0, processListRect.Width() / 3);
	m_processList.InsertColumn(2, _T("Burst Time"), 0, processListRect.Width() / 3);

	CRect processResultRect;
	m_processResult.GetClientRect(processResultRect);
	m_processResult.InsertColumn(0, _T("Process"), 0, processResultRect.Width() / 6);
	m_processResult.InsertColumn(1, _T("AT"), 0, processResultRect.Width() / 6);
	m_processResult.InsertColumn(2, _T("BT"), 0, processResultRect.Width() / 6);
	m_processResult.InsertColumn(3, _T("WT"), 0, processResultRect.Width() / 6);
	m_processResult.InsertColumn(4, _T("TT"), 0, processResultRect.Width() / 6);
	m_processResult.InsertColumn(5, _T("NTT"), 0, processResultRect.Width() / 6);

	CRect coreListRect;
	m_coreList.GetClientRect(coreListRect);
	m_coreList.InsertColumn(0, _T("Core"), 0, coreListRect.Width() / 3);
	m_coreList.InsertColumn(1, _T("Type"), 0, coreListRect.Width() / 3);
	m_coreList.InsertColumn(2, _T("PowerUsage"), 0, coreListRect.Width() / 3);
	return TRUE;  
}

void CMFCApplication1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CMFCApplication1Dlg::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetWindowRect(&rect);
	HDC hMemDC = CreateCompatibleDC(dc);
	SetStretchBltMode(hMemDC, HALFTONE);
	SelectObject(hMemDC, m_hBitmap);
	StretchBlt(dc, 0, 0, rect.Width(), rect.Height(), hMemDC, 0, 0, m_bitmap.bmWidth, m_bitmap.bmHeight, SRCCOPY);
	DeleteDC(hMemDC);

	// 그룹 Border를 그리는 작업입니다.
	CPen myPen(PS_SOLID, 5, RGB(255, 217, 102));
	dc.SelectObject(&myPen);
	SelectObject(dc, GetStockObject(NULL_BRUSH));

	dc.Rectangle(60, 195, 330, 620);
	dc.Rectangle(350, 195, 930, 620);
	dc.Rectangle(950, 195, 1485, 620);
	dc.Rectangle(60, 695, 330, 950);
	dc.Rectangle(350, 695, 1485, 950);

	// 가운데 점선을 그리는 작업입니다.
	LOGBRUSH lbr; 
	lbr.lbStyle = BS_SOLID;
	lbr.lbColor = RGB(255, 217, 102);
	lbr.lbHatch = 0;
	CPen myPen2(PS_GEOMETRIC | PS_DASH, 3, &lbr, 0, 0);
	dc.SelectObject(&myPen2);
	dc.MoveTo(60, 645);
	dc.LineTo(1485, 645);
}

HBRUSH CMFCApplication1Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	int nRet = pWnd->GetDlgCtrlID();
	if (nCtlColor == CTLCOLOR_STATIC) {
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255, 217, 102));
		hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
	}

	return hbr;
}

HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

UINT SchedulingThread(LPVOID pParam)
{
	CMFCApplication1Dlg* pDlg = (CMFCApplication1Dlg*)pParam;
	pDlg->m_running_processList.DeleteAllItems();
	pDlg->m_processResult.DeleteAllItems();
	// 모든 열 삭제
	int nColumnCount = pDlg->m_running_processList.GetHeaderCtrl()->GetItemCount();
	for (int i = nColumnCount - 1; i >= 0; i--)
		pDlg->m_running_processList.DeleteColumn(i);

	for (int j = 0; j < listview; j++)
		pDlg->m_running_processList.InsertItem(j, _T(""), NULL);

	/*설정용 변수... 이 변수들은 GUI로부터 전달받아야 합니다.*/
	int N = pDlg->processList.size();                //프로세스 개수
	int P = pDlg->coreList.size();                   // 코어 개수
	/* 예외 처리 */
	if (N * P == 0) {
		m_eThreadWork = THREAD_STOP;
		AfxMessageBox("모든 입력을 완료해주세요");
		m_pThread->SuspendThread();
		DWORD dwResult;
		::GetExitCodeThread(m_pThread->m_hThread, &dwResult);
		delete m_pThread;
		m_pThread = NULL;
		return 0;
	}

	vector<int> at(N);                               // 도착 시간 배열
	for (int i = 0; i < N; i++)
		at[i] = pDlg->processList[i].arrivalTime;

	vector<int> bt(N);                               // 버스트 타임 배열
	for (int i = 0; i < N; i++)
		bt[i] = pDlg->processList[i].burstTime;

	vector<int> core_types(P);                       // cpu 코어 타입 지정 배열
	for (int i = 0; i < P; i++)
		core_types[i] = pDlg->coreList[i];

	/* RR 전용 time_quantum, CBTA 전용 critical_time이므로 나머지 알고리즘의 이용 시 1으로 대입해줍니다.*/
	int time_setting = 1;
	if (pDlg->m_algorithm == SCHEDULING_RR) {
		if (pDlg->m_time_quantum < 1) {
			m_eThreadWork = THREAD_STOP;
			AfxMessageBox(_T("Time Quantum은 1초 이상으로 설정해주세요."));
			m_pThread->SuspendThread();
			DWORD dwResult;
			::GetExitCodeThread(m_pThread->m_hThread, &dwResult);
			delete m_pThread;
			m_pThread = NULL;
			return 0;
		}
		time_setting = pDlg->m_time_quantum;
	}
	if (pDlg->m_algorithm == SCHEDULING_CBTA) {
		if (pDlg->m_time_quantum2 < 1) {
			m_eThreadWork = THREAD_STOP;
			AfxMessageBox(_T("Critical Time은 1초 이상으로 설정해주세요."));
			m_pThread->SuspendThread();
			DWORD dwResult;
			::GetExitCodeThread(m_pThread->m_hThread, &dwResult);
			delete m_pThread;
			m_pThread = NULL;
			return 0;
		}
		time_setting = pDlg->m_time_quantum2;
	}

	int scheduling_method = pDlg->m_algorithm;         // 스케줄링 방법 결정
	int settings[3] = { time_setting, N, P };          // 스케줄링 프로세스 설정


	/*동작용 변수=> 임의 변경시 커널의 동작에 영향을 줄 수 있습니다.*/
	vector<int> running_process(P, CPU_NO_PROCESS);    // 현재 i번 코어에서 동작중인 프로세스
	running_process[0] = CPU_NO_PROCESS;

	vector<int> process_state(N);                      // 프로세스 상태 배열
	for (int i = 0; i < N; i++)
		process_state[i] = -1;

	vector<int> cpu_restarted(P, 0);                   // 재시작한 코어의 번호

	vector<double> weight(N, 0);                       // 스케줄링용 가중치 배열                        

	/*출력용 변수 => 값을 변경하더라도 커널의 동작에 영향을 주지 않습니다.*/
	vector<int> terminated_process(N, 0);              // i번 프로세스는 이미 종료된 바 있음.
	vector<int> newly_terminated_process(N, 0);        // i번 프로세스가 이번 동작에서 종료됨.
	vector<double> power_usage(P, 0.0);                // i번 코어에서 사용한 전력의 총량
	vector<int> waiting_time(N, 0);                    // i번 프로세스의 WT
	vector<int> actual_working_time(N, 0);             // i번 프로세스의 BT
	vector<int> turn_around_time(N, 0);                // i번 프로세스의 TT
	vector<double> normalized_turn_around_time(N, 0);  // i번 프로세스의 NTT
	vector<int> remaing_time(N, 0);                    // i번 프로세스의 RT

	/*GUI 구현에 사용되는 변수*/
	bool process_is_done = false;
	Kernel kernel(N, P, scheduling_method, at, bt, core_types, settings);   // 커널 초기화
	int index = 0;

	while (m_eThreadWork == THREAD_RUNNING) {

		/*스케줄링을 포함한 CPU의 연산을 한 차례 수행합니다. */
		process_is_done = kernel.stepProcess(running_process, cpu_restarted, newly_terminated_process, weight);
		// 1차례 프로세서 수행

		/*화면에 프로세싱 정보를 출력합니다.*/

		/*프로세스별 속성 확인*/
		kernel.get_wating_time(waiting_time);
		kernel.get_actual_working_time(actual_working_time);
		kernel.get_remaining_time(remaing_time);
		kernel.get_turn_around_time(turn_around_time);
		kernel.get_normalized_turn_around_time(normalized_turn_around_time);
		kernel.processor->get_power_usage(power_usage);

		/* 각 코어의 전력사용량과 총 전력사용량을 GUI에 출력합니다. */
		DispPowerConsumption(power_usage, P);
		
		/* 종료되는 process들을 GUI에 출력합니다. */
		RegistTerminatedProcess(newly_terminated_process, terminated_process, 
								actual_working_time, waiting_time, turn_around_time,
								normalized_turn_around_time, N);

		if (process_is_done) {
			m_eThreadWork = THREAD_STOP;
			break;
		}

		/* 각 코어별로 현재 처리중인 프로세스를 GUI에 출력합니다. */
		DispRunningProcess(running_process, P, index);

		pDlg->movingScroll();
		index++;

		Sleep(200);

	}
	AfxMessageBox("완료");
	return 0;
}

void CMFCApplication1Dlg::OnBnClickedSchedulingRr()
{
	m_time_quantum_control.SetFocus();
}


// RUN(Scheduling Start) Button 클릭 이벤트 핸들러입니다.
void CMFCApplication1Dlg::OnBnClickedRunBtn()
{
	UpdateData(true);
	if (m_eThreadWork == THREAD_RUNNING || m_eThreadWork == THREAD_PAUSE) {
		MessageBox(_T("스케줄링 진행중입니다."));
	}
	else {
		m_eThreadWork = THREAD_RUNNING;
		// 스레드 시작
		m_pThread = AfxBeginThread(SchedulingThread, this);
		m_pThread->m_bAutoDelete = FALSE;
	}
}
// PAUSE Button 클릭 이벤트 핸들러입니다.
void CMFCApplication1Dlg::OnBnClickedPauseBtn()
{
	if (m_eThreadWork == THREAD_RUNNING) {
		m_eThreadWork = THREAD_PAUSE;
		m_pThread->SuspendThread();
	}
}
// PAUSE->START Button 클릭 이벤트 핸들러입니다.
void CMFCApplication1Dlg::OnBnClickedPauseStartBtn()
{
	if (m_eThreadWork == THREAD_PAUSE) {
		m_eThreadWork = THREAD_RUNNING;
		m_pThread->ResumeThread();
	}
}
// Schduling Cancle Button 클릭 이벤트 핸들러입니다.
void CMFCApplication1Dlg::OnBnClickedStop()
{
	if (m_eThreadWork == THREAD_RUNNING || m_eThreadWork == THREAD_PAUSE) {
		m_pThread->SuspendThread();
		DWORD dwResult;
		::GetExitCodeThread(m_pThread->m_hThread, &dwResult);
		delete m_pThread;
		m_pThread = NULL;

		m_eThreadWork = THREAD_STOP;
		m_running_processList.DeleteAllItems();
		m_processResult.DeleteAllItems();
		// 모든 열 삭제
		int nColumnCount = m_running_processList.GetHeaderCtrl()->GetItemCount();
		for (int i = nColumnCount - 1; i >= 0; i--)
			m_running_processList.DeleteColumn(i);
	}
}

// Process Enter Button 클릭 이벤트 핸들러입니다.
void CMFCApplication1Dlg::OnBnClickedProcessInterBtn()
{
	if (m_eThreadWork == THREAD_RUNNING || m_eThreadWork == THREAD_PAUSE) {
		MessageBox(_T("스케줄링 진행중입니다."));
		return;
	}
	UpdateData(TRUE);   // GUI로 받은 에딧컨트롤 변수들 업데이트
	
	CString burst_time_str, arrival_time_str;
	m_burst_time_control.GetWindowText(burst_time_str);
	m_arrival_time_control.GetWindowText(arrival_time_str);
	
	if (burst_time_str.IsEmpty() || arrival_time_str.IsEmpty())
		return;

	if (m_processName.GetLength() == 0 || m_burstTime <= 0 || m_arrivalTime < 0) {
		MessageBox(_T("제대로 입력해주세요"));
		return;
	}
	GuiProcess newProcess = GuiProcess(m_processName, m_arrivalTime, m_burstTime);
	processList.push_back(newProcess);
	
	CString arrivalTimeStr; arrivalTimeStr.Format(_T("%d"), m_arrivalTime);
	CString burstTimeStr; burstTimeStr.Format(_T("%d"), m_burstTime);
	int nIndex = m_processList.InsertItem(m_processList.GetItemCount(), m_processName);
	m_processList.SetItemText(nIndex, 1, arrivalTimeStr);
	m_processList.SetItemText(nIndex, 2, burstTimeStr);

	m_processName = "";
	m_arrivalTime = 0;
	m_burstTime = 1;
	m_processList.EnsureVisible(m_processList.GetItemCount() - 1, FALSE);

	m_process_name_control.SetFocus();

	UpdateData(FALSE);
}

// Process Clear Button 이벤트 핸들러입니다.
void CMFCApplication1Dlg::OnBnClickedProcessClearBtn()
{
	if (m_eThreadWork == THREAD_RUNNING || m_eThreadWork == THREAD_PAUSE) {
		MessageBox(_T("스케줄링 진행중입니다."));
		return;
	}
	processList.clear();
	m_processList.DeleteAllItems();
	
}

// RANDOM Button 이벤트 핸들러입니다. 
void CMFCApplication1Dlg::OnBnClickedProcessRandomBtn()
{
	// 스케줄링 진행중일 시 예외처리
	if (m_eThreadWork == THREAD_RUNNING || m_eThreadWork == THREAD_PAUSE) {
		MessageBox(_T("스케줄링 진행중입니다."));
		return;
	}
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist1(0, 15);
	uniform_int_distribution<> dist2(1, 30);
	processList.clear();
	m_processList.DeleteAllItems();
	
	vector<int> arrival_times;
	for (int i = 0; i < 15; i++) {
		int num = dist1(gen);
		arrival_times.push_back(num);
	}
	sort(arrival_times.begin(), arrival_times.end());

	for (int i = 0; i < 15; i++) {
		CString p = "P"; CString i_str;
		i_str.Format("%d", i + 1);
		m_processName = p + i_str;
		m_arrivalTime = arrival_times[i];
		m_burstTime = dist2(gen);
		GuiProcess newProcess = GuiProcess(m_processName, m_arrivalTime, m_burstTime);
		processList.push_back(newProcess);
	}
	for (int i = 0; i < 15; i++) {
		CString arrivalTimeStr; arrivalTimeStr.Format(_T("%d"), processList[i].arrivalTime);;
		CString burstTimeStr; burstTimeStr.Format(_T("%d"), processList[i].burstTime);
		int nindex = m_processList.InsertItem(m_processList.GetItemCount(), processList[i].name);
		m_processList.SetItemText(nindex, 1, arrivalTimeStr);
		m_processList.SetItemText(nindex, 2, burstTimeStr);
	}
}

void CMFCApplication1Dlg::OnBnClickedCoreInterBtn()
{
	// 스케줄링 진행중일 시 예외처리
	if (m_eThreadWork == THREAD_RUNNING || m_eThreadWork == THREAD_PAUSE) {
		MessageBox(_T("스케줄링 진행중입니다."));
		return;
	}
	UpdateData(TRUE);

	CString coreIndex;
	coreIndex.Format(_T("%d"), coreList.size() + 1);
	int nIndex = m_coreList.InsertItem(m_coreList.GetItemCount(), coreIndex);
	
	switch (m_cpuType)
	{
	case 0:
		coreList.push_back(CPU_TYPE_P);
		m_coreList.SetItemText(nIndex, 1, _T("P_TYPE"));
		break;
	case 1:
		coreList.push_back(CPU_TYPE_E);
		m_coreList.SetItemText(nIndex, 1, _T("E_TYPE"));
		break;
	default:
		break;
	}
	m_coreList.SetItemText(nIndex, 2, _T("0.00"));
	listview++;
}


void CMFCApplication1Dlg::OnBnClickedCoreClearBtn()
{
	// 스케줄링 진행중일 시 예외처리
	if (m_eThreadWork == THREAD_RUNNING || m_eThreadWork == THREAD_PAUSE) {
		MessageBox(_T("스케줄링 진행중입니다."));
		return;
	}
	listview = 0;
	coreList.clear();
	m_coreList.DeleteAllItems();
}

BOOL CMFCApplication1Dlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)   // esc키
			return true;
		if (pMsg->wParam == VK_RETURN) { // enter키
			CWnd* pFocus = GetFocus();
			if (pFocus == &m_burst_time_control)
				OnBnClickedProcessInterBtn();
			return true;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/* 프로세스의 GUI 출력 시 P타입일 경우 다른 색을 출력해줍니다. */
void CMFCApplication1Dlg::OnNMCustomdrawCoreList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF clrNewBkColor = myColor;
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
		CString strTemp = m_coreList.GetItemText(nItem, pLVCD->iSubItem);
		if (pLVCD->iSubItem == 1) //col
		{
			if (strTemp == "P_TYPE")
				clrNewBkColor = RGB(120, 120, 120);
		}
		pLVCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_DODEFAULT;
	}
}
/* 프로세스의 GUI 출력 시 프로세스 결과 출력 리스트 컨트롤에서 프로세스에 맞춰 색을 출력합니다. */
void CMFCApplication1Dlg::OnNMCustomdrawProcessResult(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage) 
		*pResult = CDRF_NOTIFYITEMDRAW;
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage) 
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF clrNewBkColor = myColor;
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
		CString strTemp = m_processResult.GetItemText(nItem, pLVCD->iSubItem);
		if (pLVCD->iSubItem == 0) {
			for (int i = 0; i < processList.size(); i++) {
				if (strTemp == processList[i].name)
				{
					clrNewBkColor = processList[i].processColor;
					break;
				}
			}
		}
		pLVCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_NEWFONT;
	}
}
/* 프로세스의 GUI 출력 시 프로세스 입력 리스트 컨트롤에서 프로세스에 맞춰 색을 출력합니다. */
void CMFCApplication1Dlg::OnNMCustomdrawProcessList(NMHDR* pNMHDR, LRESULT* pResult)
{

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage) 
		*pResult = CDRF_NOTIFYITEMDRAW;
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF clrNewBkColor = myColor;
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
		CString strTemp = m_processList.GetItemText(nItem, pLVCD->iSubItem);
		if (pLVCD->iSubItem == 0) {
			for (int i = 0; i < processList.size(); i++) {
				if (strTemp == processList[i].name)
				{
					clrNewBkColor = processList[i].processColor;
					break;
				}
			}
		}
		pLVCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_NEWFONT;
	}
}
/* 프로세스의 GUI 출력 시 실행중인 프로세스 리스트 컨트롤에서 프로세스 이름에 맞춰 색을 출력합니다. */
void CMFCApplication1Dlg::OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF  clrNewBkColor = myColor;
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec); //row
		CString strTemp = m_running_processList.GetItemText(nItem, pLVCD->iSubItem);
		if (pLVCD->iSubItem >= 0) //col
		{
			for (int i = 0; i < processList.size(); i++)
				if (strTemp == processList[i].name)
				{
					clrNewBkColor = processList[i].processColor;
					break;
				}
		}
		pLVCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_DODEFAULT;
	}
}

void CMFCApplication1Dlg::OnBnClickedProcessRandomHelp()
{
	MessageBox(_T("프로세스 15개\n\narrival_time : 0초 ~ 15초\n\nburst_time : 1초 ~ 30초"), 
		_T("정보"), MB_ICONINFORMATION);
}

/* 리스트 컨트롤에서 내용이 다 차면 스크롤을 이동할 수 있도록 하는 함수입니다. */
void CMFCApplication1Dlg::movingScroll()
{
	int iScrollPos = m_running_processList.GetScrollPos(SB_VERT);
	int iNum = -1;
	POSITION pos = m_running_processList.GetFirstSelectedItemPosition();
	iNum = m_running_processList.GetNextSelectedItem(pos);

	CRect ref;
	m_running_processList.GetItemRect(0, ref, LVIR_BOUNDS);
	CSize szWidth(0, ref.Width() * iScrollPos);
	m_running_processList.Scroll(szWidth);
	m_running_processList.SetScrollPos(SB_VERT, iScrollPos, FALSE);

	if (iNum >= 0)
	{
		m_running_processList.SetItemState(-1, 0, LVIS_SELECTED | LVIS_FOCUSED);
		m_running_processList.SetItemState(iNum, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		m_running_processList.EnsureVisible(iNum, false);
		m_running_processList.SetFocus();
	}

	int nScrollPos = m_running_processList.GetScrollPos(SB_HORZ);
	int nScrollSize = m_running_processList.GetScrollLimit(SB_HORZ);

	// 현재 리스트 컨트롤이 가로 스크롤바를 가지고 있고, 현재 위치가 마지막 위치보다 작다면
	if (nScrollSize > 0 && nScrollPos < nScrollSize)
	{
		// 가로 스크롤바를 한 칸씩 이동시킵니다.
		m_running_processList.SendMessage(WM_HSCROLL, 3, 0);
	}
}