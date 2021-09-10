/*
* <<1. 동기화 기법의 분류와 CRITICAL_SECTION 동기화>>
* 윈도우에는 다양한 종류의 쓰레드 동기화 기법이 있다. 그러나 이들의 기본 개념은 거의 동일하고 상호 보완적이다.
* 따라서 익히는 데 큰 어려움이 없다.
* 
* 1. User 모드와 Kernal 모드
* 윈도우 운영체제의 연산 방식을 가르켜 '이중 연산 모드'라고 한다. 이는 연산을 하는데에 있어서 윈도우에는 두 가지 모드가 존재함을
* 의미한다.
* 1) 유저모드: 응용 프로그램이 실행되는 기본 모드. 물리적인 영역으로의 접근이 허용되지 않음. 접근 가능 메모리 영역도 제한
* 2) 커널모드: 운영체제가 실행될 때의 모드. 메모리, 하드웨어 접근 제한 없음
* 
* 이렇게 모드가 나뉘는 이유는 안전성 문제 때문이다.
* 
* 2. 유저 모드 동기화
* 운영체제의 도움 없이, 응용 프로그램상에서 진행되는 동기화이다. 빠르지만 기능이 제한된다.
* 
* 3. 커널 모드 동기화
* 유저 모드에 비해 제공되는 기능이 많다. Dead-Lock에 걸리지 않도록 타임아웃의 지정이 가능하다. 그러나 느리다. 또 프로세스간 동기화가 가능하다.
* 
* 4. CRITICAL_SECTION 기반의 동기화
* CRITICAL_SECTION 기반의 동기화에서는 'CRITICAL_SECTION 오브젝트'라는 것을 생성해서 이를 동기화에 활용한다. 이것은 커널 오브젝트가 아니다.
* 
* 	#include <windows.h> // Create and Delete object Function
	
	void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
	void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
	
	#include <windows.h> // Enter or Leave the Critical Section Function
	
	void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
	void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

	예제)
*/
#include <stdio.h>
#include <Windows.h>
#include <process.h>

#define NUM_THREAD 50

DWORD WINAPI threadInc(LPVOID arg);
DWORD WINAPI threadDes(LPVOID arg);

long long int num = 0;
CRITICAL_SECTION cs;

int main()
{
	HANDLE tHandles[NUM_THREAD];
	int i;
	InitializeCriticalSection(&cs);

	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);

	DeleteCriticalSection(&cs);
	printf("result : %lld \n", num);
	return 0;
}

DWORD WINAPI threadInc(LPVOID arg)
{
	int i;

	EnterCriticalSection(&cs);

	for (i = 0; i < 500000; i++)
		num += 1;

	LeaveCriticalSection(&cs);

	return 0;
}

DWORD WINAPI threadDes(LPVOID arg)
{
	int i;

	EnterCriticalSection(&cs);

	for (i = 0; i < 500000; i++)
		num -= 1;

	LeaveCriticalSection(&cs);

	return 0;
}

/*
* <<2. 커널 모드 동기화 기법>>
* 대표적인 커널모드 동기화 기법에는 Event, Semaphore, Mutex라는 커널 오브젝트 기반의 동기화가 있다.
* 
* 1. Mutex 오브젝트 기반 동기화
* 이것도 CRITICAL_SECTION 기반의 동기화와 비슷하다. 
* 
* 	#include <windows.h>
	
	HANDLE CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName);
	
	lpMutexAttributes	: NULL
	bInitialOwner		: TRUE -> Mutex is owned by Thread which call this function and non-signaled state, FALSE -> Mutex is not owned and signaled state
	lpName				: Mutex objects name, NULL -> no name

	Mutex 오브젝트는 소유자가 없는 경우 signaled 상태가 된다. 이를 이용해 동기화를 진행한다.
	그리고 이것은 커널 오브젝트이기 때문에 다음 함수를 통해 소멸된다.
	
	#include <windows.h>

	BOOL CloseHandle(HANDLE hObject);

	예제)
*/
#include <stdio.h>
#include <Windows.h>
#include <process.h>

#define NUM_THREAD 50

DWORD WINAPI threadInc(LPVOID arg);
DWORD WINAPI threadDes(LPVOID arg);

long long int num = 0;
HANDLE hMutex;

int main()
{
	HANDLE tHandles[NUM_THREAD];
	int i;

	hMutex = CreateMutex(NULL, FALSE, NULL);

	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);

	CloseHandle(hMutex);
	printf("result : %lld \n", num);
	return 0;
}

DWORD WINAPI threadInc(LPVOID arg)
{
	int i;

	WaitForSingleObject(hMutex, INFINITE);

	for (i = 0; i < 500000; i++)
		num += 1;

	ReleaseMutex(hMutex);

	return 0;
}

DWORD WINAPI threadDes(LPVOID arg)
{
	int i;

	WaitForSingleObject(hMutex, INFINITE);

	for (i = 0; i < 500000; i++)
		num -= 1;

	ReleaseMutex(hMutex);

	return 0;
}

/*
* 2. Semaphore 오브젝트 기반의 동기화
* 'Semaphore 오브젝트 기반 동기화'는 세마포어 값이라는 정수로 동기화가 이루어진다. 이 값은 0보자 작아질 수 없다.
* 
* 	#include <windows.h>
	
	HANDLE CreateSemaphore(
		LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, 
		LONG lMaximumCount, LPCTSTR lpName)
	lpSemaphoreAttributes	: NULL
	lInitialCount			: 세마포어 초기 값 지정, lMaximumCount보단 작아야 하고, 0 이상이어야 한다.
	lMaximumCount			: 최대 세마포어 값을 지정한다.
	lpName					: 이름 부여
* 
* 
* 세마포어의 값이 0인 경우 non-signaled 상태이고 그 이외의 값은 signaled 상태이다.
* 이러한 특성을 사용하여 동시에 여러개의 쓰레드를 동기화할 수 있다.
* 
* 	#include <windows.h>
	
	BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount);

	hSemaphore: 반납할 세마포어 오브젝트 핸들 전달
	lReleaseCount: 반납은 세마포어 값의 증가를 의미하는데, 이 매개변수를 통해서 증가되는 값의 크기를 지정할 수 있다. 그리고 이로 인해서 세마포어의 최대 값을 넘어서게 되면, 값은 증가하지 않고 FALSE를 반환한다.
	lpPreviousCount: 변경 이전의 세마포어 값 저장을 위한 변수의 주소 값 전달 불필요하면 NULL

	0보다 클때만 반환함
	WaitForSingleObject(hSemaphore, INFINITE); // 1 -> 0 (signaled->non-signaled)
	임계영역
	임계영역
	ReleaseSemaphore(hSemaphore, 1, NULL);     // 0 -> 1 (non-signaled->signaled)
*/

#include <stdio.h>
#include <Windows.h>
#include <process.h>

DWORD WINAPI Read(LPVOID arg);
DWORD WINAPI Accu(LPVOID arg);

static HANDLE semOne;
static HANDLE semTwo;
static int num;

int main()
{
	HANDLE hThread1, hThread2;

	semOne = CreateSemaphore(NULL, 0, 1, NULL);
	semTwo = CreateSemaphore(NULL, 1, 1, NULL);

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, Read, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, Accu, NULL, 0, NULL);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	CloseHandle(semOne);
	CloseHandle(semTwo);
	return 0;
}

DWORD WINAPI Read(LPVOID arg)
{
	int i;

	for (i = 0; i < 5; i++)
	{
		fputs("Input num : ", stdout);

		WaitForSingleObject(semTwo, INFINITE);
		scanf("%d", &num);
		ReleaseSemaphore(semOne, 1, NULL);
	}

	return 0;
}

DWORD WINAPI Accu(LPVOID arg)
{
	int sum = 0, i;

	for (i = 0; i < 5; i++)
	{
		WaitForSingleObject(semOne, INFINITE);
		sum += num;
		ReleaseSemaphore(semTwo, 1, NULL);
	}

	printf("Result: %d \n", sum);
	return 0;
}

/*
* 3. Event 기반의 동기화
* 이번에 소개하는 Event 동기화 오브젝트는 앞에서 소개한 것들과는 차이가 있다. 이는 오브젝트를 생성하는 과정에서 auto와 manual을 선택할 수 있다는 점이다.
* 
* 	#include <windows.h>
	
	HANDLE CreateEvent(
		LPSECURITY_ATTRIBUTES lpEventAttributes, 
		BOOL bManualReset,
		BOOL bInitialState,
		LPCTSTR lpName
	);

	lpEventAttributes	: NULL
	bManualReset		: TRUE -> manual-reset, FALSE -> auto-reset
	bInitialState		: TRUE -> signaled, FALSE -> non-signaled
	lpName				: NULL

	Event 오브젝트가 Manual 모드로 생성되면 다음 두 함수를 이용해서 명시적으로 오브젝트의 상태를 변경해야 한다.


	#include <windows.h>

	BOOL ResetEvent(HANDLE hEvent);   // signaled -> non-signaled
	BOOL SetEvent(HANDLE hEvent);     // non-signaled -> siganaled

*/

#include <stdio.h>
#include <windows.h>
#include <process.h>

#define STR_LEN 100

DWORD WINAPI NumberOfA(LPVOID arg);
DWORD WINAPI NumberOfOthers(LPVOID arg);

static char str[STR_LEN];
static HANDLE hEvent;

int main()
{
	HANDLE hThread1, hThread2;

	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); // manual-reset, non-signaled

	hThread1 = (HANDLE)_beginthreadex(NULL, 0, NumberOfA, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, NumberOfOthers, NULL, 0, NULL);

	fputs("Input string: ", stdout);
	fgets(str, STR_LEN, stdin);

	SetEvent(hEvent);   // non-signaled -> signaled

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	ResetEvent(hEvent); // signaled -> non-signaled

	CloseHandle(hEvent);
	return 0;
}

DWORD WINAPI NumberOfA(LPVOID arg)
{
	int i, cnt = 0;

	WaitForSingleObject(hEvent, INFINITE);

	for (i = 0; str[i]; i++)
	{
		if (str[i] == 'A')
			cnt++;
	}

	printf("Num of A: %d \n", cnt);
	return 0;
}

DWORD WINAPI NumberOfOthers(LPVOID arg)
{
	int i, cnt = 0;

	WaitForSingleObject(hEvent, INFINITE);

	for (i = 0; str[i]; i++)
	{
		if (str[i] != 'A')
			cnt++;
	}

	printf("Num of Others: %d \n", cnt - 1);
	return 0;
}

/*
* <<3. 윈도우 기반의 멀티 쓰레드 서버 구현>>
*/
// 서버

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#define BUF_SIZE 100
#define MAX_CLNT 256

DWORD WINAPI HandleClnt(LPVOID arg);
void SendMsg(char* msg, int len);

int clntCnt = 0;
SOCKET clntSocks[MAX_CLNT];
HANDLE hMutex;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSz;
	HANDLE hThread;

	if (argc != 2)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	hMutex = CreateMutex(NULL, FALSE, NULL);

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	listen(hServSock, 5);

	while (1)
	{
		clntAdrSz = sizeof(clntAdr);
		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, clntAdrSz);

		WaitForSingleObject(hMutex, INFINITE);
		clntSocks[clntCnt++] = hClntSock;
		ReleaseMutex(hMutex);

		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClntSock, 0, NULL);

		printf("Connected client IP: %s \n", inet_ntoa(clntAdr.sin_addr));
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

DWORD WINAPI HandleClnt(LPVOID arg)
{
	SOCKET hClntSock = *((SOCKET*)arg);
	int strLen = 0, i;
	char msg[BUF_SIZE];

	while ((strLen = recv(hClntSock, msg, sizeof(msg), 0)) != 0)
		SendMsg(msg, strLen);

	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++)
	{
		if (hClntSock == clntSocks[i])
		{
			while (i++ < clntCnt - 1)
				clntSocks[i] = clntSocks[i + 1];
			break;
		}
	}
	clntCnt--;
	ReleaseMutex(hMutex);

	closesocket(hClntSock);
	return 0;
}

void SendMsg(char* msg, int len)
{
	int i;

	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < clntCnt; i++)
		send(clntSocks[i], msg, len, 0);
	ReleaseMutex(hMutex);
}

//클라이언트
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#define BUF_SIZE 100
#define NAME_SIZE 20

DWORD WINAPI SendMsg(LPVOID arg);
DWORD WINAPI RecvMsg(LPVOID arg);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAdr;
	HANDLE hSndThread, hRcvThread;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sprintf(name, "[%s]", argv[3]);

	hSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	hSndThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (LPVOID)&hSock, 0, NULL);
	hRcvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (LPVOID)&hSock, 0, NULL);

	WaitForSingleObject(hSndThread, INFINITE);
	WaitForSingleObject(hRcvThread, INFINITE);

	closesocket(hSock);
	WSACleanup();
	return 0;
}

DWORD WINAPI SendMsg(LPVOID arg)
{
	SOCKET hSock = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUF_SIZE];

	while (1)
	{
		fgets(msg, BUF_SIZE, stdin);

		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			closesocket(hSock);
			exit(0);
		}

		sprintf(nameMsg, "%s %s", name, msg);
		send(hSock, nameMsg, strlen(nameMsg), 0);
	}

	return 0;
}

DWORD WINAPI RecvMsg(LPVOID arg)
{
	SOCKET hSock = *((SOCKET*)arg);
	char nameMsg[NAME_SIZE + BUF_SIZE];
	int strLen;

	while (1)
	{
		strLen = recv(hSock, nameMsg, NAME_SIZE + BUF_SIZE - 1, 0);
		if (strLen == -1)
			return -1;

		nameMsg[strLen] = 0;
		fputs(nameMsg, stdout);
	}

	return 0;
}