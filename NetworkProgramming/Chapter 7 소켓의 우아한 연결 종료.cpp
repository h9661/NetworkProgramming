/*
* <<1. >>
* 1. �Ϲ����� ���� ������ ������
* �۽��� �����Ͱ� �Ҹ�ǰų� ������ �����Ͱ� �Ҹ�� �� ����.
* 
* 2. ������ ��Ʈ��
* ������ ���� �� ȣ��Ʈ�� ����Ǹ� ��Ʈ���� ������
* 
* 			(recv)�Է� ����	 <������������	��� ����(send)
* ����														   Ŭ���̾�Ʈ
*			(send)��� ����	 ������������>  �Է� ����(recv)
* 
* ����� ������ ���� �� ��Ʈ���� ��� ��������� ���� �ƴ϶�, �ϳ��� ��Ʈ���� ���� ���̴�.
* 
* 3. ����� ���Ḧ ���� shutdown �Լ�
* int shutdown(int sock, int howto)
* �� ��° �Ű������� ���޵Ǵ� ���ڿ� ���� ������ ����� �����ȴ�. ������ ������ ������ ����.
* SHUT_RD	: �Է� ��Ʈ�� ����
* SHUT_WR	: ��� ��Ʈ�� ����
* SHUT_RDWR	: ����� ��Ʈ�� ����
* 
* SHUT_RD�� ���޵Ǹ� �Է� ��Ʈ���� ����Ǿ� �� �̻� �����͸� ���� ���� �� ���� �ȴ�.
* SHUT_WR�� ���޵Ǹ� ��� ��Ʈ���� ����Ǿ� �� �̻� �����͸� �۽� �� �� ���� �ȴ�.
* SHUT_RDWR�� ���޵Ǹ� �Ѵ� ����Ǿ� �ۼ��� �� �� ���� �ȴ�.
* 
* 4. HALF-CLOSE�� �ʿ��� ����
* "��� ��Ʈ���� �����ϸ� ��� ȣ��Ʈ�� EOF�� ���۵˴ϴ�."
* ���� ������ ���´ٴ� ��ȣ�� �޴� ���̰�, �̿� ���� �����͸� ��� �����ϰų� �۽Ź��� �� �ְ� �ȴ�.
* 
* 5. HALF-CLOSE ����� ���� ���� ���α׷�
*/

// ����
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

// Ŭ���̾�Ʈ
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