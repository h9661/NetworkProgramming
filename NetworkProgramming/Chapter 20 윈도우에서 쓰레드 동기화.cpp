/*
* <<1. ����ȭ ����� �з��� CRITICAL_SECTION ����ȭ>>
* �����쿡�� �پ��� ������ ������ ����ȭ ����� �ִ�. �׷��� �̵��� �⺻ ������ ���� �����ϰ� ��ȣ �������̴�.
* ���� ������ �� ū ������� ����.
* 
* 1. User ���� Kernal ���
* ������ �ü���� ���� ����� ������ '���� ���� ���'��� �Ѵ�. �̴� ������ �ϴµ��� �־ �����쿡�� �� ���� ��尡 ��������
* �ǹ��Ѵ�.
* 1) �������: ���� ���α׷��� ����Ǵ� �⺻ ���. �������� ���������� ������ ������ ����. ���� ���� �޸� ������ ����
* 2) Ŀ�θ��: �ü���� ����� ���� ���. �޸�, �ϵ���� ���� ���� ����
* 
* �̷��� ��尡 ������ ������ ������ ���� �����̴�.
* 
* 2. ���� ��� ����ȭ
* �ü���� ���� ����, ���� ���α׷��󿡼� ����Ǵ� ����ȭ�̴�. �������� ����� ���ѵȴ�.
* 
* 3. Ŀ�� ��� ����ȭ
* ���� ��忡 ���� �����Ǵ� ����� ����. Dead-Lock�� �ɸ��� �ʵ��� Ÿ�Ӿƿ��� ������ �����ϴ�. �׷��� ������. �� ���μ����� ����ȭ�� �����ϴ�.
* 
* 4. CRITICAL_SECTION ����� ����ȭ
* CRITICAL_SECTION ����� ����ȭ������ 'CRITICAL_SECTION ������Ʈ'��� ���� �����ؼ� �̸� ����ȭ�� Ȱ���Ѵ�. �̰��� Ŀ�� ������Ʈ�� �ƴϴ�.
* 
* 	#include <windows.h> // Create and Delete object Function
	
	void InitializeCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
	void DeleteCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
	
	#include <windows.h> // Enter or Leave the Critical Section Function
	
	void EnterCriticalSection(LPCRITICAL_SECTION lpCriticalSection);
	void LeaveCriticalSection(LPCRITICAL_SECTION lpCriticalSection);

	����)
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
* <<2. Ŀ�� ��� ����ȭ ���>>
* ��ǥ���� Ŀ�θ�� ����ȭ ������� Event, Semaphore, Mutex��� Ŀ�� ������Ʈ ����� ����ȭ�� �ִ�.
* 
* 1. Mutex ������Ʈ ��� ����ȭ
* �̰͵� CRITICAL_SECTION ����� ����ȭ�� ����ϴ�. 
* 
* 	#include <windows.h>
	
	HANDLE CreateMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes, BOOL bInitialOwner, LPCTSTR lpName);
	
	lpMutexAttributes	: NULL
	bInitialOwner		: TRUE -> Mutex is owned by Thread which call this function and non-signaled state, FALSE -> Mutex is not owned and signaled state
	lpName				: Mutex objects name, NULL -> no name

	Mutex ������Ʈ�� �����ڰ� ���� ��� signaled ���°� �ȴ�. �̸� �̿��� ����ȭ�� �����Ѵ�.
	�׸��� �̰��� Ŀ�� ������Ʈ�̱� ������ ���� �Լ��� ���� �Ҹ�ȴ�.
	
	#include <windows.h>

	BOOL CloseHandle(HANDLE hObject);

	����)
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
* 2. Semaphore ������Ʈ ����� ����ȭ
* 'Semaphore ������Ʈ ��� ����ȭ'�� �������� ���̶�� ������ ����ȭ�� �̷������. �� ���� 0���� �۾��� �� ����.
* 
* 	#include <windows.h>
	
	HANDLE CreateSemaphore(
		LPSECURITY_ATTRIBUTES lpSemaphoreAttributes, LONG lInitialCount, 
		LONG lMaximumCount, LPCTSTR lpName)
	lpSemaphoreAttributes	: NULL
	lInitialCount			: �������� �ʱ� �� ����, lMaximumCount���� �۾ƾ� �ϰ�, 0 �̻��̾�� �Ѵ�.
	lMaximumCount			: �ִ� �������� ���� �����Ѵ�.
	lpName					: �̸� �ο�
* 
* 
* ���������� ���� 0�� ��� non-signaled �����̰� �� �̿��� ���� signaled �����̴�.
* �̷��� Ư���� ����Ͽ� ���ÿ� �������� �����带 ����ȭ�� �� �ִ�.
* 
* 	#include <windows.h>
	
	BOOL ReleaseSemaphore(HANDLE hSemaphore, LONG lReleaseCount, LPLONG lpPreviousCount);

	hSemaphore: �ݳ��� �������� ������Ʈ �ڵ� ����
	lReleaseCount: �ݳ��� �������� ���� ������ �ǹ��ϴµ�, �� �Ű������� ���ؼ� �����Ǵ� ���� ũ�⸦ ������ �� �ִ�. �׸��� �̷� ���ؼ� ���������� �ִ� ���� �Ѿ�� �Ǹ�, ���� �������� �ʰ� FALSE�� ��ȯ�Ѵ�.
	lpPreviousCount: ���� ������ �������� �� ������ ���� ������ �ּ� �� ���� ���ʿ��ϸ� NULL

	0���� Ŭ���� ��ȯ��
	WaitForSingleObject(hSemaphore, INFINITE); // 1 -> 0 (signaled->non-signaled)
	�Ӱ迵��
	�Ӱ迵��
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
* 3. Event ����� ����ȭ
* �̹��� �Ұ��ϴ� Event ����ȭ ������Ʈ�� �տ��� �Ұ��� �͵���� ���̰� �ִ�. �̴� ������Ʈ�� �����ϴ� �������� auto�� manual�� ������ �� �ִٴ� ���̴�.
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

	Event ������Ʈ�� Manual ���� �����Ǹ� ���� �� �Լ��� �̿��ؼ� ��������� ������Ʈ�� ���¸� �����ؾ� �Ѵ�.


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
* <<3. ������ ����� ��Ƽ ������ ���� ����>>
*/
// ����

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

//Ŭ���̾�Ʈ
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