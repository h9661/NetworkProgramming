/*
* <<1. �񵿱� Notification IO ���� ����>>
* 
* 1. ����� �񵿱⿡ ���� ����
* �񵿱� ������̶� ����� �Լ��� ��ȯ ������ ������ �ۼ����� �Ϸ� ������ ��ġ���� �ʴ� ��츦 ���Ѵ�.
* ���� ������� �Լ��� ��ȯ ������ �������� �ۼ��� �Ϸ� ������ ��ġ�Ѵ�.
* 
* 2. ����ȭ�� ������� ������ �񵿱��� �ذ�å
* "������� ����Ǵ� ���� ȣ��� �Լ��� ��ȯ�� ���� ������ �ٸ� ���� �� �� ����!"
* ���� �񵿱� ���� �ٸ� �Լ��� �ٷ� ��ȯ�ϰ� �ٸ� ���� �� �� �ִٴ� ������ �ִ�.
* 
* 3. �񵿱� Notification ����� �𵨿� ���� ����
* "�Է¹��ۿ� �����Ͱ� ���ŵǾ �������� ������ �ʿ��ϰų�, ��¹��۰� �� �������� ������ ������ ��Ȳ�� �˸�"
* �̷��� ��Ȳ�� �߻��ϸ� �Լ��� ȣ���ؼ� Ȯ���ϰ� �׿� ���� ��ġ�� ���� �� �ִ�. �̰��� �񵿱� �˸� ���̴�.
* 
* <<2. �񵿱� �˸� ����� ���� ���ؿ� ����>>
* 1. WSAEventSelect �Լ��� Notification
* 
* #include <winsock2.h>
* int WSAEventSelect(SOCKET s, WSAEVENT hEventObject, long InetworkEvents);
* ���� �� 0, ���� �� SOCKET_ERROR ��ȯ
* 
* s				: ���� ��� ���� �ڵ� ����
* hEventObject	: �̺�Ʈ �߻� ���� Ȯ���� ���� Event ������Ʈ �ڵ� ����
* INetworkEvents: �����ϰ��� �ϴ� �̺�Ʈ�� ���� ���� ����
* 
* �� �Լ��� s���� INetworkEvents�� ���޵� �̺�Ʈ �� �ϳ��� �߻��ϸ�, hEventObject�� ���޵� �ڵ��� Ŀ�� ������Ʈ�� signaled ���·� �ٲ۴�.
* �׸��� �� �Լ��� �̺�Ʈ �߻��� ��� ���� �ٷ� ��ȯ�ϱ⿡ �ٸ� ���� �ٷ� �Ҽ��ִ�.
* 
* �� �Լ��� �� ��° ���ڷ� ���޵� �� �ִ� �̺�Ʈ�� ������ ���� ���캸��
* 
* FD_READ	: ������ �����Ͱ� �����ϴ°�?
* FD_WRITE	: ���ŷ ���� ������ ������ �����Ѱ�?
* FD_OOB	: Out-of-band �����Ͱ� ���ŵǾ��°�?
* FD_ACCEPT	: �����û�� �־��°�?
* FD_CLOSE	: ������ ���ᰡ ��û�Ǿ��°�?
* 
* 
* 2. manual-reset ��� Event ������Ʈ�� �� �ٸ� ���� ���
* 
* #include <winsock2.h>
* 
* WSAEVENT WSACreateEvent(void);
* manual reset ����̸鼭 non-signaled ������ Event ������Ʈ ����
* 
* BOOL WSACloseEvent(WSAEVENT hEvent);
* ������Ʈ ����
* 
* 3. �̺�Ʈ�� �߻� ���� Ȯ��
* �̺�Ʈ�� �߻� ������ Ȯ���ϱ� ���ؼ��� Event ������Ʈ�� Ȯ���ؾ� �Ѵ�. �̶� ����ϴ� �Լ��� ������ ����.
* 
* #include <winsock2.h>
* 
* DWORD WSAWaitForMultipleEvents(DWORD cEvents, const WSAEVENT* lphEvents, BOOL fWaitALL, DWORD dwTimeout, BOOL fAlertable);
* ���� �� �̺�Ʈ �߻� ������Ʈ ���� ������, ���� �� WSA_INVALID_EVENT ��ȯ
* 
* cEvents	: signaled ���·��� ���� ���θ� Ȯ���� Event ������Ʈ�� ���� ���� ����
* lphEvents	: Event ������Ʈ�� �ڵ��� �����ϰ� �ִ� �迭�� �ּ� �� ����.
* fWaitAll	: TRUE ���� �� ��� Event ������Ʈ�� siganeld ������ �� ��ȯ, FALSE�� �ϳ��� signaled �� ��ȯ
* dwTimeout	: ������ �ð� ���� ��ȯ. WSA_INFINITE ���� �� signaled ���°� �� �� ���� ���ŷ
* fAlertable: TRUE ���� ��, alertable wait ���·��� ����
* ��ȯ ��	: ��ȯ�� ���� ������ ��� �� WSA_WAIT_EVENT_0�� ����, �� ���� �Ű������� ���޵� �迭�� �������� signaled ���°� �� 
*			  Event ������Ʈ�� �ڵ��� ����� �ε����� ���ȴ�
* 
* �̴� ������ �̺�Ʈ �߻��� ���ؼ� Event ������Ʈ�� siganled ���°� �Ǿ�� ��ȯ�ϴ� �Լ��̹Ƿ� ������ �̺�Ʈ �߻����θ� Ȯ���ϱ⿡ ���� �Լ���.
* 
* 4. �̺�Ʈ ������ ����
* WSAWaitForMultipleEvents �Լ��� ���ؼ� signaled ���·� ���̵� Event ������Ʈ���� �˾� �� �� �ְ� �Ǿ���
* ���� ���������� �ش� ������Ʈ�� signaled ���°� �� ������ Ȯ���ؾ� �Ѵ�. �׸��� �̸� �˾Ƴ��� ���� �Լ��� �Ұ��Ѵ�.
* �� �Լ��� ȣ���� ���ؼ� signaled ������ event ������Ʈ �ڵ�Ӹ� �ƴ϶�, �̿� ����� ������ �ڵ鵵 �ʿ��ϴ�.
* 
* #include <winsock2.h>
* 
* int WSAEnumNetworkEvents(SOCKET s, WSAEVENT hEventObject, LPWSANETWORKEVENTS lpNetworkEvents);
* ���� �� 0, ���� �� SOCKET_ERROR ��ȯ
* 
* s					: �̺�Ʈ�� �߻��� ������ �ڵ� ����
* hEventObject		: ���ϰ� ����� signaled ������ Event ������Ʈ�� �ڵ� ����
* lpNetworkEvents	: �߻��� �̺�Ʈ�� ���������� ���������� ä���� WSANETWORKEVENTS ����ü ������ �ּҰ� ����
* 
* �� �Լ��� menual reset����� signaled Event ������Ʈ�� non-signaled ���·� ������� ���ش�.
* ���� �� �Լ��� �����ִ� ����ü WSANETWORKEVENTS�� �Ұ��ϰڴ�.
* 
* typedef struct _WSANETWORKEVENTS{
*	long lNetworkEvents;
*	int iErrorCode[FD_MAX_EVENTS];
*}WSANETWORKEVENTS, *LPWSANETWORKEVENTS;
* 
* ���� ����ü �ɹ� lNetworkEvents���� �߻��� �̺�Ʈ�� ������ ����. WSAEventSelect �Լ��� �� ��° ���ڷ� ���޵Ǵ� ����� ����������
* ������ �����Ͱ� �����ϸ� FD_READ�� ����ǰ�, �����û�� �ִ� ��쿡�� FD_ACCEPT�� ����. ���� ������ ���� ������� �̺�Ʈ�� ������ Ȯ���� �� �ִ�.
* 
* WSANETWORKEVENTS netEvents;
* ...
* WSAEnumNetworkEvents(hSock, hEvent, &netEvents);
* if(netEvents.lNetworkEvents & FD_ACCEPT){
* ...
* }
* if(netEvents.lNetworkEvents & FD_READ){
* ...
* }
* 
* �׸��� �����ڵ�� iErrorCode�� ����. ���� ������ �ۼ��غ���.
*/
// �񵿱� �˸� ����� �� ���ڼ��� ����

#include <stdio.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 100

void CompressSockets(SOCKET hSockArr[], int idx, int total);
void CompressEvents(WSAEVENT hEventArr[], int idx, int total);
void ErrorHandling(char* msg);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;

	SOCKET hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT hEventArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT newEvent;
	WSANETWORKEVENTS netEvents;

	int numOfClntSock = 0;
	int strLen, i;
	int posInfo, startIdx;
	int clntAdrLen;
	char msg[BUF_SIZE];

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	hServSock = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	listen(hServSock, 5);

	newEvent = WSACreateEvent();

	WSAEventSelect(hServSock, newEvent, FD_ACCEPT);

	hSockArr[numOfClntSock] = hServSock;
	hEventArr[numOfClntSock] = newEvent;
	numOfClntSock++;


	while (1) {
		posInfo = WSAWaitForMultipleEvents(numOfClntSock, hEventArr, FALSE, WSA_INFINITE, FALSE);

		startIdx = posInfo - WSA_WAIT_EVENT_0;

		for (i = startIdx; i < numOfClntSock; i++) {
			int sigEventIdx = WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);

			if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT))
				continue;
			else {
				sigEventIdx = i;
				WSAEnumNetworkEvents(hSockArr[sigEventIdx], hEventArr[sigEventIdx], &netEvents);

				if (netEvents.lNetworkEvents & FD_ACCEPT) {
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0) {
						puts("Accept Error!");
						break;
					}

					clntAdrLen = sizeof(clntAdr);
					hClntSock = accept(hSockArr[sigEventIdx], (SOCKADDR*)&clntAdr, &clntAdrLen);
					newEvent = WSACreateEvent;
					WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);

					hEventArr[numOfClntSock] = newEvent;
					hSockArr[numOfClntSock] = hClntSock;
					numOfClntSock++;
					puts("connected new client");
				}

				if (netEvents.lNetworkEvents & FD_READ) {
					if (netEvents.iErrorCode[FD_READ_BIT] != 0) {
						puts("Read Error");
						break;
					}

					strLen = recv(hSockArr[sigEventIdx], msg, sizeof(msg), 0);
					send(hSockArr[sigEventIdx], msg, strLen, 0);
				}

				if (netEvents.lNetworkEvents & FD_CLOSE) {
					if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0) {
						puts("Close Error");
						break;
					}

					WSACloseEvent(hEventArr[sigEventIdx]);
					closesocket(hSockArr[sigEventIdx]);
					numOfClntSock--;
					CompressSockets(hSockArr, sigEventIdx, numOfClntSock);
					CompressEvents(hEventArr, sigEventIdx, numOfClntSock);
				}
			}
		}
	}
	WSACleanup();
	return 0;
}

void CompressSockets(SOCKET hSockArr[], int idx, int total) {
	int i;

	for (i = idx; i < total; i++)
		hSockArr[i] = hSockArr[i + 1];
}

void CompressEvents(WSAEVENT hEventArr[], int idx, int total) {
	int i;

	for (i = idx; i < total; i++)
		hEventArr[i] = hEventArr[i + 1];
}