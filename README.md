# Operating-System
## Project 0
training before starting pintos coding
## Pintos 2_1 user programs
이번 프로젝트는 pintos2-1번 프로젝트로써 본 프로젝트의 궁극적인 개발 목표는 사용자에 의한 기본 명령어에 대해 프로그램의 실행 환경을 구축하는 것이다. 개발 전 기존 Pintos에는 기본적인 프로그램의 실행 환경만 구성되어 있을 뿐 메모리 저장, 시스템 콜 기능이 구현되어 있지 않으므로 커널 자체 수행 이외의 사용자 프로그램을 실행할 수 없다. 따라서 기본 명령어를 분리하는 Argument passing과 그 분리된 정보를 stack(memory)에 쌓는 Construct_esp를 구현한다. 또한 해당 명령어를 요구에 맞게 동작시키기 위해 System call을 구현하고, 이때에 Context switching을 하면서 발생하는 오류를 방지하기 위해 동기화 단계를 적용해 줌으로써 본 프로그램의 목표를 달성한다.
## Pintos 2_2 user programs
이번 프로젝트는 pintos2-1번 프로젝트에서 구현한 기존 시스템 콜 환경에서 파일 시스템 관련 시스템 콜을 구현하여 기초적인 user program이 pintos에서 구동할 수 있도록 만드는 것이다. 이 때 여러 파일 시스템 콜 사이에는 Critical section이 존재하게 되므로 2-1번 프로젝트와 같이 적절한 동기화 환경을 구현해야 하며, 오류 입력이 들어올 수 있는 경우가 다수이므로 예외의 경우가 발생하여 프로그램이 오작동 되는 경우가 없도록 그에 대한 정확한 처리를 하여야 한다. 또한 프로세스의 종료나 오류 처리 등을 수행할 때는 메모리의 낭비가 발생하지 않도록 deallocate를 하여 프로그램의 생산성 및 내구성을 확장시킴으로써 본 프로젝트의 목적을 달성한다. 
## Pintos 1 threads
이번 프로젝트는 기존에 구현한 pintos2-1번, pintos2-2번과는 별계로 alarm과 priority에 대한 처리를 바탕으로 한 스케줄러를 구현하는 것이다. 기존에 구현되어 있던 설정은 busy-waiting 방식으로 수행되게 되어있었는데, 이번 프로젝트에서는 block을 사용하여 cpu 이용시간의 낭비를 줄인다. Process마다 우선순위 선정 공식에 의거한 priority를 설정하여 우선순위에 따른 process의 수행이 가능하도록 하였으며 이 때 발생할 수 있는 Starvation문제를 방지하기 위해 aging 기법을 활용하여 이를 해결한다. 단, priority 값 설정에 있어서 float형 변수를 지원하지 않으므로 fixed-point 방식을 활용하여 int형을 가상의 float으로 변환하여 계산한다. 
