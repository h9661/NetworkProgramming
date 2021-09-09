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

