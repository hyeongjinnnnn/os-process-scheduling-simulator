#include <iostream>
#include <Windows.h>
#include "software.hpp"
#include "simple_gui.hpp"
#include<vector>


int main(void) {

	/*������ ����... �� �������� GUI�κ��� ���޹޾ƾ� �մϴ�.*/
	int N = 0;													//���μ��� ����
	std::cin >> N;
	int P = 1;													// �ھ� ����
	std::cin >> P;

	std::vector<int> at(N);										// ���� �ð� �迭
	for (int i = 0; i < N; i++)
		std::cin >> at[i];		

	std::vector<int> bt(N);										// ����Ʈ Ÿ�� �迭
	for (int i = 0; i < N; i++)
		std::cin >> bt[i];

	std::vector<int> core_types(P);								// cpu �ھ� Ÿ�� ���� �迭
	for (int i = 0; i < P; i++)
		core_types[i] = { CPU_TYPE_E };

	int scheduling_method = SCHEDULING_RR;						// �����ٸ� ��� ����
	int settings[3] = { 3, N, P };								// �����ٸ� ���μ��� ����

	/*���ۿ� ����=> ���� ����� Ŀ���� ���ۿ� ������ �� �� �ֽ��ϴ�.*/
	std::vector<int> running_process(P, 0);					// ���� i�� �ھ�� �������� ���μ���
	// running_process[0] = CPU_NO_PROCESS;	

	std::vector<int> process_state(N);						// ���μ��� ���� �迭
	for (int i = 0; i < N; i++)
		process_state[i] = -1;

	std::vector<int> cpu_restarted(N, 0);						// ������� �ھ��� ��ȣ

	std::vector<double> weight(N, 0);							// �����ٸ��� ����ġ �迭								

	/*��¿� ���� => ���� �����ϴ��� Ŀ���� ���ۿ� ������ ���� �ʽ��ϴ�.*/
	std::vector<int> terminated_process(N, 0);					// i�� ���μ����� �̹� ����� �� ����.
	std::vector<int> newly_terminated_process(N, 0);			// i�� ���μ����� �̹� ���ۿ��� �����.
	std::vector<double> power_usage(P, 0.0);					// i�� �ھ�� ����� ������ �ѷ�
	std::vector<int> waiting_time(N, 0);						// i�� ���μ����� WT
	std::vector<int> turn_around_time(N, 0);					// i�� ���μ����� TT
	std::vector<double> normalized_turn_around_time(N, 0);		// i�� ���μ����� NTT
	std::vector<int> remaing_time(N, 0);						// i�� ���μ����� RT



	/*main �Լ� ���ۿ� ���Ǵ� ����*/
	bool process_is_done = false;
	Kernel kernel(N, P, scheduling_method, at, bt, core_types, settings);				// Ŀ�� ����



	while (true) {
		std::cout << "\n\n\n\n" << std::endl;
		ManualSleep();		// ��� ���


		/*�����ٸ��� ������ CPU�� ������ �� ���� �����մϴ�. */
		process_is_done= kernel.stepProcess(running_process, cpu_restarted, newly_terminated_process, weight);			
			// 1���� ���μ��� ����
		


		/*ȭ�鿡 ���μ��� ������ ����մϴ�.*/
		disp_clk(kernel.clk);
		// ���� Ŭ�� ��ȣ�� ǥ���մϴ�.

		/*���μ����� �Ӽ� Ȯ��*/
		kernel.get_wating_time(waiting_time);
		kernel.get_remaining_time(remaing_time);
		kernel.get_turn_around_time(turn_around_time);
		kernel.get_normalized_turn_around_time(normalized_turn_around_time);
		kernel.processor->get_power_usage(power_usage);


		/*���μ����� �Ӽ� ǥ��*/
		disp_array("WT", waiting_time, N);
		disp_array("RT", remaing_time, N);
		disp_array("TT", turn_around_time, N);
		disp_array("NTT", normalized_turn_around_time, N);
		disp_array("power consumption", power_usage, P);

		disp_array("running_process", running_process, P);
		disp_array("cpu_restarted", cpu_restarted, N);
		disp_array("newly_terminated_process", newly_terminated_process, N);

		Sleep(1000);
		if (process_is_done)
			break;
	}
	return 0;
}
