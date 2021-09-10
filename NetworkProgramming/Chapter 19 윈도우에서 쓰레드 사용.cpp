/*
* <<1. Ŀ�� ������Ʈ>>
* 1. Ŀ�� ������Ʈ��?
* �ü���� ���ؼ� �����Ǵ� ���ҽ����� ������ �������� ������ ����ϱ� ���� ���������� ������ ���(����ü ����)�� �����Ѵ�.
* �׸��� �� ������ ����� ������ 'Ŀ�� ������Ʈ'��� �Ѵ�.
* 
* 2. Ŀ�� ������Ʈ�� �����ִ� �ü���̴�.
* "Ŀ�� ������Ʈ�� ����, ����, �׸��� �Ҹ� ������ �����ϴ� �ͱ��� ��� �ü���� ���̴�!"
* 
* <<2. ������ ����� ������ ����>>
* 1. ���μ����� �������� ����
* ���μ����� �����带 ��� �ٱ����̴�.
* 
* 2. �����쿡���� ������ ���� ���
* �� �Լ��� ȣ��Ǹ� �����尡 �����ǰ�, �ü���� ���� ������ ���ؼ� Ŀ�� ������Ʈ�� �����Ѵ�.
* �׸��� ����������, �� Ŀ�� ������Ʈ�� ������ ������ �ϴ� ������ ǥ���Ǵ� �ڵ��� ��ȯ�Ѵ�.
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

	���� �� ������ �ڵ�, ���� �� NULL ��ȯ
	lpThreadAttribute	: NULL
	dwStackSize			: 0
	lpStartAddress		: �������� main �Լ� ���� ����
	lpParameter			: �������� main �Լ� ȣ�� �� ������ ���� ���� ����
	dwCreationFlags		: ������ ���� ������ �ൿ�� ����, 0�� �����ϸ� ������ ���ÿ� ���� ������ ����
	lpThreadId			: ������ ID�� ������ ���� ������ �ּ� �� ����

	3. ��Ƽ ������ ����� ���α׷� �ۼ��� ���� ȯ�� ����

	4. �����忡 ������ C ǥ�� �Լ��� ȣ���� ���� ������ ����
	�ռ� ������ �Լ��� C/C++ ǥ�� �Լ��� ���ؼ� ���������� �������� ���Ѵ�. ���� ���� �Լ��� �Ұ��Ѵ�.

	#include <process.h>

	uintptr_t _beginThreadex(
		void* security,
		unsigned stack_size,
		unsigned (*start_address)(void),
		void* arglist,
		unsigned initflag,
		unsigned* thrdaddr
	);

	�� �Լ��� ������� ������ �ۼ��� ���ڴ�.
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
* <<3. Ŀ�� ������Ʈ�� �� ���� ����>>
* Ŀ�� ������Ʈ���� �ش� ���ҽ��� ���ݿ� ���� ���� ������ ����.
* �׷��� �� �߿����� '����'��� ���� �ο��Ѵ�. ���� ���, �������� Ŀ�� ������Ʈ�� �־ ū ���ɻ�� ���� �����̴�.
* ������ ����� ���¸� ������ 'signaled ����'��� �ϰ�, ������� ���� ���¸� ������ 'non-signaled ����'��� �Ѵ�.
* 
* 1. Ŀ�� ������Ʈ�� ����, �׸��� ������ Ȯ��
* �ü���� �츮���� ������ ���� ����ϰ� �ִ�.
* 
* "���μ����� �����尡 ����Ǹ� �ش� Ŀ�� ������Ʈ�� signaled ���·� ������ ���ڴ�!"
* 
* �� ���� ���μ����� �������� Ŀ�� ������Ʈ�� �ʱ⿡�� non-signaled ���¶�� ���̴�.
* Ŀ�� ������Ʈ�� boolean ���� �ϳ��� �� ���¸� ǥ���Ѵ�. �� TRUE�̸� signaled FASLE�̸� non-signaled ���¸� ���Ѵ�.
* 
* "�� Ŀ�� ������Ʈ�� signaled �����ΰ���?"
* 
* �̷� ������ �ϱ� ���� ���ǵ� �Լ��� WaitForSingleObject & WaitForMultipleObject�̴�
* 
* 2. WaitForSingleObject & WaitForMultipleObject
* ���� WaitForSingleObject �Լ��� �Ұ��ϰڴ�. �̴� �ϳ��� Ŀ�� ������Ʈ�� ���ؼ� signaled ���������� Ȯ���ϱ� ���� ȣ���ϴ� �Լ��̴�.
* 
* 	#include <windows.h>
	
	DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMiliseconds);
	���� �� �̺�Ʈ ����, ���� �� WAIT_FAILED ��ȯ
	
	hHandle			: ���� Ȯ���� ����� �Ǵ� Ŀ�� ������Ʈ�� �ڵ� ����
	dwMiliseconds	: INFINITE ���� ��, Ŀ�� ������Ʈ�� signaled ���°� �Ǳ� ������ ��ȯ���� �ʴ´�.
	��ȯ ��			: signaled ���·� ��ȯ ��, WAIT_OBJECT_0 ��ȯ, Ÿ�� �ƿ����� ���� ��ȯ �� WAIT_TIMEOUT ��ȯ

	�� �Լ��� �̺�Ʈ �߻��� ���ؼ� ��ȯ�Ǹ�, �ش� Ŀ�� ������Ʈ�� �ٽ� non-signaled ���·� �ǵ����⵵ �Ѵ�.
	�׸��� �̷��� �ٽ� non-signaled ���°� �Ǵ� Ŀ�� ������Ʈ�� ������ 'auto-reset ���'��� �Ѵ�. �ݴ�� 'menual reset ���'

	�̹����� �� �̻��� Ŀ�� ������Ʈ�� ������� ���¸� Ȯ���ϴ� ��쿡 ȣ���ϴ� �Լ��̴�.

	#include <windows.h>

	DWORD WaitForMultipleObjects(DWORD nCount, const HANDLE* lpHandles, BOOL bWaitAll, DWORD dwMiliseconds);

	nCount			: �˻��� Ŀ�� ������Ʈ�� �� ����
	lpHandles		: �ڵ� ������ ��� �ִ� �迭�� �ּ� �� ����
	bWaitAll		: TRUE ���� ��, ��� �˻� ����� signaled ���°� �Ǿ�� ��ȯ, FALSE�� �ϳ��� signaled �ŵ� ��ȯ
	dwMiliseconds	: ����

	�׷� ������ ����.
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
