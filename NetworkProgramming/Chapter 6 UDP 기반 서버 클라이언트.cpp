/*
* <<1. UDP에 대한 이해>>
* 1. UDP 소켓의 특성
* 흐름 제어가 UDP와 TCP를 구분 지어주는 가장 큰 차이점이다.
* 
* 2. UDP의 내부 동작 원리
* UDP의 역할 중 가장 중요한 것은 호스트로 수신된 패킷을 PORT 번호를 참조하여 최종 목적지인 UDP 소켓에 전달하는 것이다.
* 
* 3. UDP의 효율적 사용
* 데이터의 양이 적으면서 잦은 연결이 필요할 때는 UDP가 TCP보다 빠르게 동작한다.
* 
* <<2. UDP 기반 서버/클라이언트 구현>>
* 1. UDP에서의 서버와 클라이언트는 연결되어 있지 않습니다.
* UDP서버는 TCP와 같이 연결된 상태로 데이터를 송수신하지 않는다. 따라서 TCP 서버 구현 과정에서 거쳤던 listen 함수와 accept 함수의 호출은 불필요하다.
* UDP 소켓의 생성과 데이터의 송수신 과정만이 존재할 뿐이다.
* 
* 2. UDP에서는 서버건 클라이언트건 하나의 소켓만 있으면 된다.
* 
* 3. UDP 기반의 데이터 입출력 함수
* UDP 소켓은 연결 상태를 유지하지 않으므로, 데이터를 전송할 때마다 반드시 목적지의 주소 정보를 별도로 추가해야 한다.
* ssize_t sendto(int sock, void* buff, size_t nbytes, int flags, struct sockaddr* to, socklen_t addrlen);
* 
* ssize_t recvfrom(int sock, void* buff, size_t nbytes, int flags, strcut sockaddr* from, socklen_t addrlen);
* 
* 4. UDP 기반의 에코 서버와 에코 클라이언트
*/

//SERVER

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET servSock;
	char message[500];
	int strLen;
	int clntAdrSz;
	SOCKADDR_IN servAdr, clntAdr;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	servSock = socket(PF_INET, SOCK_DGRAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	bind(servSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while (1)
	{
		clntAdrSz = sizeof(clntAdrSz);

		strLen = recvfrom(servSock, message, 500, 0, (SOCKADDR*)&clntAdr, &clntAdrSz);

		sendto(servSock, message, strLen, 0, (SOCKADDR*)&clntAdr, sizeof(clntAdrSz));
	}

	closesocket(servSock);
	WSACleanup();
	return 0;
}

//CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string.h>

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET sock;
	char message[500];
	int strLen;
	SOCKADDR_IN servAdr;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	sock = socket(PF_INET, SOCK_DGRAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	while (1)
	{
		fgets(message, 500, stdin);

		if (strcmp(message, "Q\n") == 0 || strcmp(message, "q\n") == 0)
			break;

		sendto(sock, message, 500, 0, (SOCKADDR*)&servAdr, sizeof(servAdr));

		int servAdrSz = sizeof(servAdr);

		strLen = recvfrom(sock, message, 500, 0, (SOCKADDR*)&servAdr, &servAdrSz);

		message[strLen] = 0;
		printf("MESSAGE : %s \n", message);
	}

	closesocket(sock);
	WSACleanup();
	return 0;
}
/*
* 5. UDP 클라이언트 소켓의 주소 정보 할당
* sendto 함수가 처음 호출되는 시점에 해당 소켓에 IP와 PORT 번호가 자동으로 할당된다. 따라서 UDP의 클라이언트 프로그램에서는 
* 주소 정보를 할당하는 bind의 호출이 불필요하다.
* 
* <<3. UDP의 데이터 송수신 특성과 UDP에서의 connect 함수 호출>>
* 1. 데이터의 경계가 존재하는 UDP 소켓
* UDP는 입력함수의 호출 횟수와 출력함수의 호출 횟수가 완벽하게 일치해야만 송수신 데이터를 전부 수신할 수 있다.
* 
* 2. connected UDP 소켓과 unconnected UDP 소켓
* connect로 생대 소켓과 연결한 UDP 소켓은 주소 정보를 계속해서 전달해야 하는 sendto/recvfrom 함수의 하나의 단계를 건너 뜀으로써 속도를 높힐 수 있고,
* send/recv 함수로 데이터를 송수신 할 수 있다.
*/