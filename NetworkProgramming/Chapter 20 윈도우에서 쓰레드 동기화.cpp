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

