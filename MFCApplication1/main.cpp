#include <iostream>
#include <Windows.h>
#include "software.hpp"
#include "simple_gui.hpp"
#include<vector>


int main(void) {

	/*설정용 변수... 이 변수들은 GUI로부터 전달받아야 합니다.*/
	int N = 0;													//프로세스 개수
	std::cin >> N;
	int P = 1;													// 코어 개수
	std::cin >> P;

	std::vector<int> at(N);										// 도착 시간 배열
	for (int i = 0; i < N; i++)
		std::cin >> at[i];		

	std::vector<int> bt(N);										// 버스트 타임 배열
	for (int i = 0; i < N; i++)
		std::cin >> bt[i];

	std::vector<int> core_types(P);								// cpu 코어 타입 지정 배열
	for (int i = 0; i < P; i++)
		core_types[i] = { CPU_TYPE_E };

	int scheduling_method = SCHEDULING_RR;						// 스케줄링 방법 결정
	int settings[3] = { 3, N, P };								// 스케줄링 프로세스 설정

	/*동작용 변수=> 임의 변경시 커널의 동작에 영향을 줄 수 있습니다.*/
	std::vector<int> running_process(P, 0);					// 현재 i번 코어에서 동작중인 프로세스
	// running_process[0] = CPU_NO_PROCESS;	

	std::vector<int> process_state(N);						// 프로세스 상태 배열
	for (int i = 0; i < N; i++)
		process_state[i] = -1;

	std::vector<int> cpu_restarted(N, 0);						// 재시작한 코어의 번호

	std::vector<double> weight(N, 0);							// 스케줄링용 가중치 배열								

	/*출력용 변수 => 값을 변경하더라도 커널의 동작에 영향을 주지 않습니다.*/
	std::vector<int> terminated_process(N, 0);					// i번 프로세스는 이미 종료된 바 있음.
	std::vector<int> newly_terminated_process(N, 0);			// i번 프로세스가 이번 동작에서 종료됨.
	std::vector<double> power_usage(P, 0.0);					// i번 코어에서 사용한 전력의 총량
	std::vector<int> waiting_time(N, 0);						// i번 프로세스의 WT
	std::vector<int> turn_around_time(N, 0);					// i번 프로세스의 TT
	std::vector<double> normalized_turn_around_time(N, 0);		// i번 프로세스의 NTT
	std::vector<int> remaing_time(N, 0);						// i번 프로세스의 RT



	/*main 함수 동작에 사용되는 변수*/
	bool process_is_done = false;
	Kernel kernel(N, P, scheduling_method, at, bt, core_types, settings);				// 커널 정의



	while (true) {
		std::cout << "\n\n\n\n" << std::endl;
		ManualSleep();		// 잠시 대기


		/*스케줄링을 포함한 CPU의 연산을 한 차례 수행합니다. */
		process_is_done= kernel.stepProcess(running_process, cpu_restarted, newly_terminated_process, weight);			
			// 1차례 프로세서 수행
		


		/*화면에 프로세싱 정보를 출력합니다.*/
		disp_clk(kernel.clk);
		// 현재 클럭 번호를 표시합니다.

		/*프로세스별 속성 확인*/
		kernel.get_wating_time(waiting_time);
		kernel.get_remaining_time(remaing_time);
		kernel.get_turn_around_time(turn_around_time);
		kernel.get_normalized_turn_around_time(normalized_turn_around_time);
		kernel.processor->get_power_usage(power_usage);


		/*프로세스별 속성 표시*/
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
