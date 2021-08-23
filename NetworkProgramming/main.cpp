/*
* <<1. 네트워크 프로그래밍과 소캣의 간단한 이해>>
* 네트워크로 연결되어 있는 서로 다른 두 컴퓨터가 데이터를 주고받을 수 있도록 하는 것이 네트워크 프로그래밍이다.
* 운영체제 에서 제공하는 소켓은 물리적으로 연결된 네트워크상에서의 데이터 송수신에 사용할 수 있는 소프트웨어적인 장치를 의미한다.
* 그래서 네트워크 프로그래밍을 소켓 프로그래밍이라고도 부른다.
* 
* <<2. 전화를 거는 소켓의 구현>>
* 전화기도 크게 두 가지로 나뉘는데, 그 중 우리가 처음 이야기할 TCP 소켓이라는 것은 전화기에 비유할 수 있다.
* 1) 전화기 장만에 비유되는 socket 함수
* 2) 전화기에 번호를 부여에 비유되는 bind 함수
* 3) 전화기의 케이블 연결에 비유되는 listen 함수
* 4) 수화기를 드는 것에 비유되는 accept 함수
* 지금까지 설명한 것을 토대로 연결 요청을 허용하는 소켓의 생성 과정을 프로그래밍 해보자.
*/

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAdr, clntAdr;

	int szClntAddr;
	char message[] = "hello world!";

	if (argc != 2) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//전화기 장만
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	//서버 주소 정보 초기화
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	//전화기에 번호 부여
	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	//전화기를 케이블에 연결
	listen(hServSock, 5);

	szClntAddr = sizeof(clntAdr);

	//수화기 들기
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &szClntAddr);

	//메시지 보내기
	send(hClntSock, message, sizeof(message), 0);

	//종료
	closesocket(hClntSock);
	closesocket(hServSock);

	return 0;
}

/*
* 앞서 보인 서버 프로그램에서 생성한 소켓을 '서버 소켓' 또는 '리스닝 소켓'이라고 한다.
* 반면에 이번에 소개할 소켓은 연결을 요청하는 '클라이언트 소켓'이다.
* 앞에서 전화를 거는 함수를 소개하기 않았다. 이는 클라이언트 소켓을 대상으로 호출하는 함수이기 때문이다.
* connect 함수가 전화를 거는 함수이다.
* 클라이언트 프로그램에서는 socket 함수 호출을 통한 소켓의 생성과 connect 함수 호출을 통한 서버로의 연결 요청 과정만이 존재한다.
*/

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAdr;

	char message[30];
	int strLen;

	if (argc != 3) {
		printf("Usage : %s <IP> <PORT> \n", argv[0]);
		exit(1);
	}

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));

	strLen = recv(hSocket, message, sizeof(message) - 1, 0);

	printf("Message From Server : %s \n", message);

	closesocket(hSocket);
}