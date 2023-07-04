
#include <iostream>
#include "simple_gui.h"
#include <vector>

void DispRunningProcess(std::vector<int>& running_process, int p, int index) {
    // 다이어로그 객체를 받아와서 이 함수에서 실행중인 프로세스 정보를 출력해주는 작업을 합니다.
    CMFCApplication1Dlg* pDlg = (CMFCApplication1Dlg*)AfxGetApp()->GetMainWnd();

    CString sec;
    sec.Format(_T("%d"), index + 1);
    pDlg->m_running_processList.InsertColumn(index + 1, sec, LVCFMT_RIGHT, 80, index + 1);

    for (int i = 0; i < p; i++) {
        if (running_process[i] != -1) {
            pDlg->m_running_processList.SetItemText(i, index, pDlg->processList[running_process[i]].name);
        }
    }
    return;
}

void RegistTerminatedProcess(std::vector<int>& newly_terminated_process,
    std::vector<int>& terminated_process,
    std::vector<int>& actual_burst_time,
    std::vector<int>& waiting_time,
    std::vector<int>& turn_around_time,
    std::vector<double>& normalized_turn_around_time,
    int n)
{
    // 다이어로그 객체를 받아와서 이 함수에서 종료되는 프로세스 정보를 출력해주는 작업을 합니다.
    CMFCApplication1Dlg* pDlg = (CMFCApplication1Dlg*)AfxGetApp()->GetMainWnd();
    for (int i = 0; i < n; i++) {
        if (newly_terminated_process[i]) {
            terminated_process[i] = 1;
            CString str;
            int nIndex = pDlg->m_processResult.InsertItem(pDlg->m_processResult.GetItemCount(), pDlg->processList[i].name);
            str.Format(_T("%d"), pDlg->processList[i].arrivalTime);
            pDlg->m_processResult.SetItemText(nIndex, 1, str);
            str.Format(_T("%d"), actual_burst_time[i]);
            pDlg->m_processResult.SetItemText(nIndex, 2, str);
            str.Format(_T("%d"), waiting_time[i]);
            pDlg->m_processResult.SetItemText(nIndex, 3, str);
            str.Format(_T("%d"), turn_around_time[i]);
            pDlg->m_processResult.SetItemText(nIndex, 4, str);
            str.Format(_T("%0.2f"), normalized_turn_around_time[i]);
            pDlg->m_processResult.SetItemText(nIndex, 5, str);
        }
    }
    pDlg->m_processResult.EnsureVisible(pDlg->m_processResult.GetItemCount() - 1, FALSE);
    return;
}

void DispPowerConsumption(std::vector<double>& power_usage, int p) {
    // 다이어로그 객체를 받아와서 이 함수에서 전력 정보를 출력해주는 작업을 합니다.
    CMFCApplication1Dlg* pDlg = (CMFCApplication1Dlg*)AfxGetApp()->GetMainWnd();
    double m_total_power_ = 0.0;   // 전체 전력 사용량 변수
    CString power_usage_str;
    /* 각각의 코어별로 전력 사용량이 GUI에 출력하면서 전체 전력에 합산됩니다. */
    for (int i = 0; i < p; i++) {
        power_usage_str.Format("%0.2f", power_usage[i]);
        power_usage_str += 'J';
        pDlg->m_coreList.SetItemText(i, 2, power_usage_str);
        m_total_power_ += power_usage[i];
    }
    CString total_power_str;
    total_power_str.Format("%.2f", m_total_power_);
    total_power_str += 'J';
    pDlg->m_total_power.SetWindowTextA(total_power_str);
}