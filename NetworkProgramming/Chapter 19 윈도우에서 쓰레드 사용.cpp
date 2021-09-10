/*
* <<1. 커널 오브젝트>>
* 1. 커널 오브젝트란?
* 운영체제에 의해서 생성되는 리소스들은 관리를 목적으로 정보를 기록하기 위해 내부적으로 데이터 블록(구조체 변수)을 생성한다.
* 그리고 이 데이터 블록을 가르켜 '커널 오브젝트'라고 한다.
* 
* 2. 커널 오브젝트의 소유주는 운영체제이다.
* "커널 오브젝트의 생성, 관리, 그리고 소멸 시점을 결정하는 것까지 모두 운영체제의 몫이다!"
* 
* <<2. 윈도우 기반의 쓰레드 생성>>
* 1. 프로세스와 스레드의 관계
* 프로세스는 스레드를 담는 바구니이다.
* 
* 2. 윈도우에서의 스레드 생성 방법
* 이 함수가 호출되면 스레드가 생성되고, 운영체제는 이의 관리를 위해서 커널 오브젝트를 생성한다.
* 그리고 마지막으로, 이 커널 오브젝트의 구분자 역할을 하는 정수로 표현되는 핸들을 반환한다.
* 
* 	#include <windows.h>
	
	HANDLE CreateHandle(
		LPSECURITY_ATTRIBUTES lpThreadAttributes,
		SIZE_T dwStackSize,
		LPTHREAD_START_ROUTINE lpStartAddress,
		LPVOID lpParameter,
		DWORD dwCreationFlags,
		LPDWORD lpThreadId
	};

	성공 시 쓰레드 핸들, 실패 시 NULL 반환
	lpThreadAttribute	: NULL
	dwStackSize			: 0
	lpStartAddress		: 쓰레드의 main 함수 정보 전달
	lpParameter			: 쓰레드의 main 함수 호출 시 전달할 인자 정보 전달
	dwCreationFlags		: 쓰레드 생성 이후의 행동을 결정, 0을 전달하면 생성과 동시에 실행 가능한 상태
	lpThreadId			: 쓰레드 ID의 저장을 위한 변수의 주소 값 전달

	3. 멀티 스레드 기반의 프로그램 작성을 위한 환경 설정

	4. 스레드에 안전한 C 표준 함수의 호출을 위한 스레드 생성
	앞서 설명한 함수는 C/C++ 표준 함수에 대해서 안정적으로 동작하지 못한다. 따라서 다음 함수를 소개한다.

	#include <process.h>

	uintptr_t _beginThreadex(
		void* security,
		unsigned stack_size,
		unsigned (*start_address)(void),
		void* arglist,
		unsigned initflag,
		unsigned* thrdaddr
	);

	위 함수를 기반으로 예제를 작성해 보겠다.
*/
#include <stdio.h>
#include <windows.h>
#include <process.h>

DWORD WINAPI ThreadFunc(LPVOID arg);

int main(int argc, char* argv[])
{
	HANDLE hThread;
	DWORD threadID;
	int param = 5;

	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (LPVOID)param, 0, &threadID);

	Sleep(3000);

	puts("end of main");
	return 0;
}

DWORD WINAPI ThreadFunc(LPVOID arg)
{
	int i;
	int cnt = (int)arg

		for (i = 0; i < cnt; i++)
		{
			Sleep(1000);
			puts("running thread");
		}

	return 0;
}

/*
* <<3. 커널 오브젝트의 두 가지 상태>>
* 커널 오브젝트에는 해당 리소스의 성격에 따라서 많은 정보가 담긴다.
* 그런데 그 중에서도 '상태'라는 것을 부여한다. 예를 들어, 쓰레드의 커널 오브젝트에 있어서 큰 관심사는 종료 여부이다.
* 때문에 종료된 상태를 가리켜 'signaled 상태'라고 하고, 종료되지 않은 상태를 가르켜 'non-signaled 상태'라고 한다.
* 
* 1. 커널 오브젝트의 상태, 그리고 상태의 확인
* 운영체제는 우리에게 다음과 같이 약속하고 있다.
* 
* "프로세스나 스레드가 종료되면 해당 커널 오브젝트를 signaled 상태로 변경해 놓겠다!"
* 
* 이 얘기는 프로세스와 스레드의 커널 오브젝트가 초기에는 non-signaled 상태라는 뜻이다.
* 커널 오브젝트는 boolean 변수 하나로 이 상태를 표현한다. 즉 TRUE이면 signaled FASLE이면 non-signaled 상태를 뜻한다.
* 
* "이 커널 오브젝트는 signaled 상태인가요?"
* 
* 이런 질문을 하기 위해 정의된 함수가 WaitForSingleObject & WaitForMultipleObject이다
* 
* 2. WaitForSingleObject & WaitForMultipleObject
* 먼저 WaitForSingleObject 함수를 소개하겠다. 이는 하나의 커널 오브젝트에 대해서 signaled 상태인지를 확인하기 위해 호출하는 함수이다.
* 
* 	#include <windows.h>
	
	DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMiliseconds);
	성공 시 이벤트 정보, 실패 시 WAIT_FAILED 반환
	
	hHandle			: 상태 확인의 대상이 되는 커널 오브젝트의 핸들 전달
	dwMiliseconds	: INFINITE 전달 시, 커널 오브젝트가 signaled 상태가 되기 전에는 반환하지 않는다.
	반환 값			: signaled 상태로 변환 시, WAIT_OBJECT_0 반환, 타임 아웃으로 인한 반환 시 WAIT_TIMEOUT 반환

	위 함수는 이벤트 발생에 의해서 반환되면, 해당 커널 오브젝트를 다시 non-signaled 상태로 되돌리기도 한다.
	그리고 이렇게 다시 non-signaled 상태가 되는 커널 오브젝트를 가리켜 'auto-reset 모드'라고 한다. 반대는 'menual reset 모드'

	이번에는 둘 이상의 커널 오브젝트를 대상으로 상태를 확인하는 경우에 호출하는 함수이다.

	#include <windows.h>

	DWORD WaitForMultipleObjects(DWORD nCount, const HANDLE* lpHandles, BOOL bWaitAll, DWORD dwMiliseconds);

	nCount			: 검사할 커널 오브젝트의 수 전달
	lpHandles		: 핸들 정보를 담고 있는 배열의 주소 값 전달
	bWaitAll		: TRUE 전달 시, 모든 검사 대상이 signaled 상태가 되어야 반환, FALSE는 하나만 signaled 돼도 반환
	dwMiliseconds	: 같음

	그럼 예제를 보자.
*/
#include <stdio.h>
#include <windows.h>
#include <process.h>

DWORD WINAPI ThreadFunc(LPVOID arg);

int main()
{
	HANDLE hThread;
	DWORD wr;
	DWORD threadID;
	int param = 5;

	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (LPVOID)param, 0, &threadID);

	wr = WaitForSingleObject(hThread, INFINITE);

	if (wr == WAIT_FAILED)
	{
		puts("thread wait failed");
		return -1;
	}

	printf("wait result : %s \n", wr == WAIT_OBJECT_0 ? "signaled" : "timeout");
	puts("end of main");
	return 0;
}

DWORD WINAPI ThreadFunc(LPVOID arg)
{
	int i;
	int cnt = (int)arg;

		for (i = 0; i < cnt; i++)
		{
			Sleep(1000);
			puts("running thread");
		}

	return 0;
}
