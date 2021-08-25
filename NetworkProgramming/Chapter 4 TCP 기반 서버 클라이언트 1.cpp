/*
* <<1. TCP와 UDP에 대한 이해>>
* 인터넷 프로토콜 기반 소켓일 경우, 데이터 전송 방식에 따라 TCP, UDP 소켓으로 나뉜다고 말했다.
* TCP는 데이터 전송 과정의 컨트롤 이라는 뜻을 담고 있다. 때문에 이를 정확히 이해하기 위해서는 컨트롤의 방법과 범위에 대해 알아야 한다.
* 
* 1. TCP 프로토콜 스택
* TCP를 이야기하기 앞서 TCP가 속해있는 TCP/IP 프로토콜 스택을 먼저 살펴보자
* 4)				APPLICATION 계층
* 3)	TCP 계층						UDP 계층
* 2)					IP 계층
* 1)					LINK 계층
* 위와 같이 총 네개의 스택으로 나뉜다.
* 
* 2. TCP/IP 프로토콜의 탄생 배경
* 큰 문제를 작은 문제로 나눴고, 이 과정에서 프로토콜이 여러개 만들어졌으며 계층 구조를 통해서 상호간 관계를 맺게 되었다.
* 
* 3. LINK 계층
* 물리적 영역의 표준화에 대한 결과이다. LAN, WAN, MAN 같은 네트워크 표준과 관련된 프로토콜을 정의하는 영역이다. 컴퓨터간 물리적 연결을 담당한다.
* 
* 4. IP 계층
* 물리적 연결이 형성되었으니 데이터를 전송할 차례이다. 어떤 경로로 데이터를 보낼까 하는 문제를 해결하는 것이 IP 계층이다.
* IP 계층은 비 연결지향적인 신뢰할 수 없는 프로토콜이다.
* 
* 5. TCP/UDP 계층
* 데이터의 전송을 위한 경로의 검색을 IP 계층에서 해주니, 그 경로를 기준으로 데이터를 전송하면 된다. 이 계층이 실제 데이터 송수신을 담당한다.
* 호스트 대 호스트간 데이터 송수신 방식을 방식을 약속하는 것이 TCP/UDP 계층이다.
* 
* 6. APPLICATION 계층
* 사용자들간의 약속이다.
* 
* <<2. TCP 기반 서버, 클라이언트 구현>>
* 이제 TCP 기반 서버를 구현해보자.
* 
* 1. TCP 서버에서의 기본적인 함수 호출 순서
* 1) socket()
* 2) bind()
* 3) listen()
* 4) accept()
* 5) read()/write()
* 6) close()
* 
* socket 함수 호출 후 bind로 소켓에 주소 할당을 배웠으니, 그 이후에 대해 배워보자.
* 
* 2. 연결요청 대기 상태로 진입
* bind 함수를 통해서 소켓에 주소까지 할당했다면, listen 함수 호출을 통해서 '연결 요청 대기 상태'로 들어가야 한다.
* int listen(SOCKET socket, int backlog)
* 두 번째 인자의 크기에 해당하는 대기실이 만들어지고, 이를 연결 요청 대기 큐라고 한다.
* 
* 3. 클라이언트의 연결 요청 수락
* listen 함수 호출 이후 클라이언트의 연결 요청이 오면, 들어온 순서대로 연결 요청을 수락해야 한다.
* int accept(SOCKET socket, SOCKADDR* addr, socklen_t* addrlen)
* accept 함수는 연결 요청 대기 큐에 있는 클라이언트의 연결 요청을 수락하는 함수이다.
* 따라서 accept 함수는 내부적으로 소켓을 생성하고 그 소켓의 핸들을 반환한다. 중요한 점은 소켓이 자동으로 생성되어, 연결 요청한 클라이언트 소켓에
* 연결까지 이뤄진다는 점이다.
* 
* 4. TCP 클라이언트의 기본적인 함수 호출 순서
* 1) socket()
* 2) connect()
* 3) read()/write()
* 4) close()
* 
* 서버 구현과 차이점은 connect 함수의 호출이다.
* connect 함수를 호출하면 서버의 연결 요청 대기 큐에 등록된다. 따라서 바로 연결되는 것이 아니다.
* 
* 5. TCP 기반 서버/클라이언트의 함수 호출 관계
*	 [SERVER]			  [CLIENT]
* 1) socket()			  socket()
* 2) bind()	  <------┐
* 3) listen() <------┴─ connect()
* 4) accept()
* 5) read()/write()		  write()/read()
* 6) close()		      close()
* 
* <<3. Iterative 기반의 서버, 클라이언트 구현>>
* 이번에는 계속해서 들어오는 클라이언트의 연결 요청을 수락하는 Iterative 서버를 구현해 볼 것이다.
*/

#include <iostream>
#include <WinSock2.h>
using namespace std;

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;

	int szClntAddr;
	char message[30];
	int recvDataLen, i;

	WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	listen(hServSock, 5);

	for (i = 0; i < 5; i++) {
		szClntAddr = sizeof(clntAdr);

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &szClntAddr);

		cout << "CONNECTED CLIENTL: " << i + 1 << endl;
		while (1) {
			recvDataLen = recv(hClntSock, message, 50, 0);

			if (message[0] == 'q' || message[0] == 'Q')
				break;

			send(hClntSock, message, recvDataLen, 0);
		}
		closesocket(hClntSock);
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;

	char message[30];
	int strLen, i;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <PORT> \n", argv[0]);
		exit(1);
	}

	//윈속 초기화
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//클라이언트 소켓 생성
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	//접속할 서버 주소 초기화
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	//초기화한 서버 주소에 연결
	connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while (1)
	{
		//서버에게 보낼 문자열 입력
		fputs("Input message(Q to quit):", stdout);
		fgets(message, 50, stdin);

		if (message[0] == 'q' || message[0] == 'Q')
			break;

		//서버에게 문자열 전송
		send(hSocket, message, 50, 0);

		//서버로부터 문자열 수신
		strLen = recv(hSocket, message, 50 - 1, 0);

		//수신 받은 문자열 끝에 NULL문자 넣어서 문자열의 끝을 알리기.
		message[strLen] = 0;

		//문자열 출력
		printf("Message from server: %s \n", message);
	}


	//종료
	closesocket(hSocket);
	WSACleanup();
}

/*
* ※에코 클라이언트의 문제점※
* 위의 서버 코드에서는 단 한번의 read 함수를 호출한다. 하지만 tcp 소켓의 특징은 무엇인가? 바로 데이터의 경계가 존재하지 않는다는 것이다.
* 만약 서버가 수신하는 문자열의 길이가 길어서 여러 번의 read 함수를 호출한다면 이는 클라이언트로부터 전송되는 데이터를 모두 수신하지 못하는 상황으로
* 이어질 수 있다. 이에 대한 해답을 다음 쳅터에서 알아보자.
*/