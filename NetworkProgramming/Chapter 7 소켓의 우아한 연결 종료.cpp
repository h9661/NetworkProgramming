/*
* <<1. >>
* 1. 일방적인 연결 종료의 문제점
* 송신한 데이터가 소멸되거나 수신할 데이터가 소멸될 수 있음.
* 
* 2. 소켓의 스트림
* 소켓을 통해 두 호스트가 연결되면 스트림이 형성됨
* 
* 			(recv)입력 버퍼	 <──────	출력 버퍼(send)
* 서버														   클라이언트
*			(send)출력 버퍼	 ──────>  입력 버퍼(recv)
* 
* 우아한 종료라는 것은 두 스트림을 모두 끊어버리는 것이 아니라, 하나의 스트림만 끊는 것이다.
* 
* 3. 우아한 종료를 위한 shutdown 함수
* int shutdown(int sock, int howto)
* 두 번째 매개변수에 전달되는 인자에 따라서 종료의 방법이 결정된다. 인자의 종류는 다음과 같다.
* SHUT_RD	: 입력 스트림 종료
* SHUT_WR	: 출력 스트림 종료
* SHUT_RDWR	: 입출력 스트림 종료
* 
* SHUT_RD가 전달되면 입력 스트림이 종료되어 더 이상 데이터를 수신 받을 수 없게 된다.
* SHUT_WR가 전달되면 출력 스트림이 종료되어 더 이상 데이터를 송신 할 수 없게 된다.
* SHUT_RDWR가 전달되면 둘다 종료되어 송수신 할 수 없게 된다.
* 
* 4. HALF-CLOSE가 필요한 이유
* "출력 스트림을 종료하면 상대 호스트로 EOF가 전송됩니다."
* 따라서 연결을 끊는다는 신호를 받는 것이고, 이에 따라 데이터를 모두 수신하거나 송신받을 수 있게 된다.
* 
* 5. HALF-CLOSE 기반의 파일 전송 프로그램
*/

// 서버
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	FILE* fp;
	char buf[BUF_SIZE];
	int readCnt;

	SOCKADDR_IN servAdr, clntAdr;
	int clntAdrSz;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	fp = fopen("file_server_win.c", "rb");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	listen(hServSock, 5);

	clntAdrSz = sizeof(clntAdr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &clntAdrSz);

	while (1)
	{
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);

		if (readCnt < BUF_SIZE)
		{
			send(hClntSock, (char*)&buf, readCnt, 0);
			break;
		}
		send(hClntSock, (char*)&buf, BUF_SIZE, 0);
	}

	shutdown(hClntSock, SD_SEND);

	recv(hClntSock, (char*)buf, BUF_SIZE, 0);
	printf("Message From client : %s \n", buf);

	fclose(fp);
	closesocket(hClntSock); closesocket(hServSock);
	WSACleanup();

	return 0;
}

// 클라이언트
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSock;
	FILE* fp;

	char buf[BUF_SIZE];
	int readCnt;
	SOCKADDR_IN servAdr;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	fp = fopen("receive.dat", "wb");

	hSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	connect(hSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while ((readCnt = recv(hSock, buf, BUF_SIZE, 0)) != EOF)
		fwrite((void*)buf, 1, readCnt, fp);

	puts("received file data");
	send(hSock, "Thank you", 10, 0);

	fclose(fp);
	closesocket(hSock);
	WSACleanup();

	return 0;
}