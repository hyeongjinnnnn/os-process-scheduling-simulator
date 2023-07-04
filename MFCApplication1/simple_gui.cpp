
#include <iostream>
#include "simple_gui.h"
#include <vector>

void DispRunningProcess(std::vector<int>& running_process, int p, int index) {
    // ���̾�α� ��ü�� �޾ƿͼ� �� �Լ����� �������� ���μ��� ������ ������ִ� �۾��� �մϴ�.
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
    // ���̾�α� ��ü�� �޾ƿͼ� �� �Լ����� ����Ǵ� ���μ��� ������ ������ִ� �۾��� �մϴ�.
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
    // ���̾�α� ��ü�� �޾ƿͼ� �� �Լ����� ���� ������ ������ִ� �۾��� �մϴ�.
    CMFCApplication1Dlg* pDlg = (CMFCApplication1Dlg*)AfxGetApp()->GetMainWnd();
    double m_total_power_ = 0.0;   // ��ü ���� ��뷮 ����
    CString power_usage_str;
    /* ������ �ھ�� ���� ��뷮�� GUI�� ����ϸ鼭 ��ü ���¿� �ջ�˴ϴ�. */
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