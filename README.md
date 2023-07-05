# 💻os-process-scheduling-simulator
**2023-1학기 KOREATECH 운영체제 프로세스 스케쥴링 알고리즘 과제**
<br>
<br>
본 프로그램은 MFC를 기반으로 작성한 응용 프로그램으로, 과제에 제시된 부가적인 수행 목표를 모두 수행할 수 있도록 UI를 구성하였습니다. 아래 사진에서 보이는 것처럼, 간단한 UI를 통하여 사용자가 스케줄링 방식과 프로세스 목록을 직접 조정할 수 있으며, 이를 토대로 초 단위로 프로세스 스케줄링을 수행할 수 있습니다. 
<p align="center">
  <img width="70%" src="https://github.com/hyeongjinnnnn/os-process-scheduling-simulator/assets/81033777/2c04e01a-e829-49e8-8259-269eb24e0679">
</p>
<h3>⚠️ [Window 10 이상]</h3>
Window 10이하의 OS버전에서는 폰트 문제로 깨져보일 수 있습니다.
<br>
<br>
<h3>⚠️ [1920 x 1080 이상 해상도]</h3> 
MFC의 특성 상 bmp파일로 디자인하여 DP를 고정시켜 정적 레이아웃으로 구현하였습니다. 

따라서 1920 x 1080이상의 해상도를 제공하는 모니터에서 실행해주세요.
## 🛠 기술
- `C++`
- `MFC`
## 🔍미리보기(사용예시)
https://github.com/hyeongjinnnnn/os-process-scheduling-simulator/assets/81033777/80b236ab-6472-4a8b-8b3b-21cb0145538c

## 🔧과제 요구사항으로 제시된 시스템 속성
- E core는 1초에 1의 일을 처리하며 1초에 전력 1W를 소비한다. (시동 전력 0.1W)
- P core는 1초에 2의 일을 처리하며 1초에 전력 3W를 소비한다. (시동 전력 0.5W)
- 시동 전력은 미사용 중이던 코어를 사용하는 경우에 발생한다.
- 스케줄링은 1초 단위로 이루어진다. (P코어에 할당된 작업의 남은 양이 1이어도, 1초를 소모)

#### Termiologies
- AT: Arrival Time
- BT: Burst Time 프로세스의 일을 처리하는 데 걸리는 시간
- WT: Waiting Time = TT - BT
- TT: Turnaround Time
- NTT: Normalized Turnaround Time = TT / BT

## 🔑알고리즘
#### FCFS

#### RR

#### SPN

#### SRTN

#### HRRN

#### CBTA(Critical Burst Time Allocation)
직접 정의한 스케줄링 알고리즘으로는 기존의 SRTN을 응용한 CBTA(Critical Burst Time Allocation) 방식을 고안하였습니다. CBTA 알고리즘에서는 사용자가 직접 임계 시간(cirtical time)을 지정할 수 있습니다. 스케줄러는 임계 시간 이하 BT를 갖는 프로세스들을 최우선으로 처리하며, 임계시간보다 긴 BT를 갖는 프로세스에 대해서는 가장 큰 BT를 갖는 프로세스를 우선적으로 처리합니다. 이 스케줄링 방식은 슈퍼 컴퓨터와 같이 무거운 프로세스를 주로 처리하는 특수 목적 컴퓨터에서 간헐적으로 요청되는 가벼운 프로세스를 기준에 따라서 먼저 처리하여 일부 프로세스의 응답성을 높이는 데에 사용할 수 있습니다.
