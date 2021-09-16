/*
* <<1. 비동기 Notification IO 모델의 이해>>
* 
* 1. 동기와 비동기에 대한 이해
* 비동기 입출력이란 입출력 함수의 반환 시점과 데이터 송수신의 완료 시점이 일치하지 않는 경우를 뜻한다.
* 동기 입출력은 함수의 반환 시점과 데이터의 송수신 완료 시점이 일치한다.
* 
* 2. 동기화된 입출력의 단점과 비동기의 해결책
* "입출력이 진행되는 동안 호출된 함수가 반환을 하지 않으니 다른 일을 할 수 없다!"
* 따라서 비동기 모델은 다른 함수가 바로 반환하고 다른 일을 할 수 있다는 장점이 있다.
* 
* 3. 비동기 Notification 입출력 모델에 대한 이해
* "입력버퍼에 데이터가 수신되어서 데이터의 수신이 필요하거나, 출력버퍼가 비어서 데이터의 전송이 가능한 상황을 알림"
* 이러한 상황이 발생하면 함수를 호출해서 확인하고 그에 대한 조치를 취할 수 있다. 이것이 비동기 알림 모델이다.
* 
* <<2. 비동기 알림 입출력 모델의 이해와 구현>>
* 1. WSAEventSelect 함수와 Notification
* 
* #include <winsock2.h>
* int WSAEventSelect(SOCKET s, WSAEVENT hEventObject, long InetworkEvents);
* 성공 시 0, 실패 시 SOCKET_ERROR 반환
* 
* s				: 관찰 대상 소켓 핸들 전달
* hEventObject	: 이벤트 발생 유무 확인을 위한 Event 오브젝트 핸들 전달
* INetworkEvents: 감시하고자 하는 이벤트의 유형 정보 전달
* 
* 이 함수는 s에서 INetworkEvents에 전달된 이벤트 중 하나가 발생하면, hEventObject에 전달된 핸들의 커널 오브젝트를 signaled 상태로 바꾼다.
* 그리고 이 함수는 이벤트 발생과 상관 없이 바로 반환하기에 다른 일을 바로 할수있다.
* 
* 위 함수에 세 번째 인자로 전달될 수 있는 이벤트의 종류에 대해 살펴보자
* 
* FD_READ	: 수신할 데이터가 존재하는가?
* FD_WRITE	: 블로킹 없이 데이터 전송이 가능한가?
* FD_OOB	: Out-of-band 데이터가 수신되었는가?
* FD_ACCEPT	: 연결요청이 있었는가?
* FD_CLOSE	: 연결의 종료가 요청되었는가?
* 
* 
* 2. manual-reset 모드 Event 오브젝트의 또 다른 생성 방법
* 
* #include <winsock2.h>
* 
* WSAEVENT WSACreateEvent(void);
* manual reset 모드이면서 non-signaled 상태인 Event 오브젝트 생성
* 
* BOOL WSACloseEvent(WSAEVENT hEvent);
* 오브젝트 제거
* 
* 3. 이벤트의 발생 유무 확인
* 이벤트의 발생 유무를 확인하기 위해서는 Event 오브젝트를 확인해야 한다. 이때 사용하는 함수는 다음과 같다.
* 
* #include <winsock2.h>
* 
* DWORD WSAWaitForMultipleEvents(DWORD cEvents, const WSAEVENT* lphEvents, BOOL fWaitALL, DWORD dwTimeout, BOOL fAlertable);
* 성공 시 이벤트 발생 오브젝트 관련 정보를, 실패 시 WSA_INVALID_EVENT 반환
* 
* cEvents	: signaled 상태로의 전이 여부를 확인할 Event 오브젝트의 개수 정보 전달
* lphEvents	: Event 오브젝트의 핸들을 저장하고 있는 배열의 주소 값 전달.
* fWaitAll	: TRUE 전달 시 모든 Event 오브젝트가 siganeld 상태일 때 반환, FALSE는 하나라도 signaled 시 반환
* dwTimeout	: 전달한 시간 이후 반환. WSA_INFINITE 전달 시 signaled 상태가 될 때 까지 블로킹
* fAlertable: TRUE 전달 시, alertable wait 상태로의 진입
* 반환 값	: 반환된 정수 값에서 상수 값 WSA_WAIT_EVENT_0을 빼면, 두 번재 매개변수로 전달된 배열을 기준으로 signaled 상태가 된 
*			  Event 오브젝트의 핸들이 저장된 인덱스가 계산된다
* 
* 이는 소켓의 이벤트 발생에 의해서 Event 오브젝트가 siganled 상태가 되어야 반환하는 함수이므로 소켓의 이벤트 발생여부를 확인하기에 좋은 함수다.
* 
* 4. 이벤트 종류의 구분
* WSAWaitForMultipleEvents 함수를 통해서 signaled 상태로 전이된 Event 오브젝트까지 알아 낼 수 있게 되었다
* 이제 마지막으로 해당 오브젝트가 signaled 상태가 된 원인을 확인해야 한다. 그리고 이를 알아내기 위한 함수를 소개한다.
* 이 함수의 호출을 위해선 signaled 상태의 event 오브젝트 핸들뿐만 아니라, 이와 연결된 소켓의 핸들도 필요하다.
* 
* #include <winsock2.h>
* 
* int WSAEnumNetworkEvents(SOCKET s, WSAEVENT hEventObject, LPWSANETWORKEVENTS lpNetworkEvents);
* 성공 시 0, 실패 시 SOCKET_ERROR 반환
* 
* s					: 이벤트가 발생한 소켓의 핸들 전달
* hEventObject		: 소켓과 연결된 signaled 상태인 Event 오브젝트의 핸들 전달
* lpNetworkEvents	: 발생한 이벤트의 유형정보와 오류정보로 채워질 WSANETWORKEVENTS 구조체 변수의 주소값 전달
* 
* 이 함수는 menual reset모드의 signaled Event 오브젝트를 non-signaled 상태로 변경까지 해준다.
* 이제 위 함수와 관련있는 구조체 WSANETWORKEVENTS를 소개하겠다.
* 
* typedef struct _WSANETWORKEVENTS{
*	long lNetworkEvents;
*	int iErrorCode[FD_MAX_EVENTS];
*}WSANETWORKEVENTS, *LPWSANETWORKEVENTS;
* 
* 위의 구조체 맴버 lNetworkEvents에는 발생한 이벤트의 정보가 담긴다. WSAEventSelect 함수의 세 번째 인자로 전달되는 상수와 마찬가지로
* 수신할 데이터가 존재하면 FD_READ가 저장되고, 연결요청이 있는 경우에는 FD_ACCEPT가 담긴다. 따라서 다음과 같은 방식으로 이벤트의 종류를 확인할 수 있다.
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
* 그리고 오류코드는 iErrorCode에 담긴다. 이제 예제를 작성해보자.
*/
// 비동기 알림 입출력 모델 에코서버 구현

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