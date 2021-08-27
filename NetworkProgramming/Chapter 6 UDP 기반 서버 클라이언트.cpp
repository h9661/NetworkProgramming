/*
* <<1. UDP�� ���� ����>>
* 1. UDP ������ Ư��
* �帧 ��� UDP�� TCP�� ���� �����ִ� ���� ū �������̴�.
* 
* 2. UDP�� ���� ���� ����
* UDP�� ���� �� ���� �߿��� ���� ȣ��Ʈ�� ���ŵ� ��Ŷ�� PORT ��ȣ�� �����Ͽ� ���� �������� UDP ���Ͽ� �����ϴ� ���̴�.
* 
* 3. UDP�� ȿ���� ���
* �������� ���� �����鼭 ���� ������ �ʿ��� ���� UDP�� TCP���� ������ �����Ѵ�.
* 
* <<2. UDP ��� ����/Ŭ���̾�Ʈ ����>>
* 1. UDP������ ������ Ŭ���̾�Ʈ�� ����Ǿ� ���� �ʽ��ϴ�.
* UDP������ TCP�� ���� ����� ���·� �����͸� �ۼ������� �ʴ´�. ���� TCP ���� ���� �������� ���ƴ� listen �Լ��� accept �Լ��� ȣ���� ���ʿ��ϴ�.
* UDP ������ ������ �������� �ۼ��� �������� ������ ���̴�.
* 
* 2. UDP������ ������ Ŭ���̾�Ʈ�� �ϳ��� ���ϸ� ������ �ȴ�.
* 
* 3. UDP ����� ������ ����� �Լ�
* UDP ������ ���� ���¸� �������� �����Ƿ�, �����͸� ������ ������ �ݵ�� �������� �ּ� ������ ������ �߰��ؾ� �Ѵ�.
* ssize_t sendto(int sock, void* buff, size_t nbytes, int flags, struct sockaddr* to, socklen_t addrlen);
* 
* ssize_t recvfrom(int sock, void* buff, size_t nbytes, int flags, strcut sockaddr* from, socklen_t addrlen);
* 
* 4. UDP ����� ���� ������ ���� Ŭ���̾�Ʈ
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
* 5. UDP Ŭ���̾�Ʈ ������ �ּ� ���� �Ҵ�
* sendto �Լ��� ó�� ȣ��Ǵ� ������ �ش� ���Ͽ� IP�� PORT ��ȣ�� �ڵ����� �Ҵ�ȴ�. ���� UDP�� Ŭ���̾�Ʈ ���α׷������� 
* �ּ� ������ �Ҵ��ϴ� bind�� ȣ���� ���ʿ��ϴ�.
* 
* <<3. UDP�� ������ �ۼ��� Ư���� UDP������ connect �Լ� ȣ��>>
* 1. �������� ��谡 �����ϴ� UDP ����
* UDP�� �Է��Լ��� ȣ�� Ƚ���� ����Լ��� ȣ�� Ƚ���� �Ϻ��ϰ� ��ġ�ؾ߸� �ۼ��� �����͸� ���� ������ �� �ִ�.
* 
* 2. connected UDP ���ϰ� unconnected UDP ����
* connect�� ���� ���ϰ� ������ UDP ������ �ּ� ������ ����ؼ� �����ؾ� �ϴ� sendto/recvfrom �Լ��� �ϳ��� �ܰ踦 �ǳ� �����ν� �ӵ��� ���� �� �ְ�,
* send/recv �Լ��� �����͸� �ۼ��� �� �� �ִ�.
*/