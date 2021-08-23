/*
* <<1. ��Ʈ��ũ ���α׷��ְ� ��Ĺ�� ������ ����>>
* ��Ʈ��ũ�� ����Ǿ� �ִ� ���� �ٸ� �� ��ǻ�Ͱ� �����͸� �ְ���� �� �ֵ��� �ϴ� ���� ��Ʈ��ũ ���α׷����̴�.
* �ü�� ���� �����ϴ� ������ ���������� ����� ��Ʈ��ũ�󿡼��� ������ �ۼ��ſ� ����� �� �ִ� ����Ʈ�������� ��ġ�� �ǹ��Ѵ�.
* �׷��� ��Ʈ��ũ ���α׷����� ���� ���α׷����̶�� �θ���.
* 
* <<2. ��ȭ�� �Ŵ� ������ ����>>
* ��ȭ�⵵ ũ�� �� ������ �����µ�, �� �� �츮�� ó�� �̾߱��� TCP �����̶�� ���� ��ȭ�⿡ ������ �� �ִ�.
* 1) ��ȭ�� �常�� �����Ǵ� socket �Լ�
* 2) ��ȭ�⿡ ��ȣ�� �ο��� �����Ǵ� bind �Լ�
* 3) ��ȭ���� ���̺� ���ῡ �����Ǵ� listen �Լ�
* 4) ��ȭ�⸦ ��� �Ϳ� �����Ǵ� accept �Լ�
* ���ݱ��� ������ ���� ���� ���� ��û�� ����ϴ� ������ ���� ������ ���α׷��� �غ���.
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

	//��ȭ�� �常
	hServSock = socket(PF_INET, SOCK_STREAM, 0);

	//���� �ּ� ���� �ʱ�ȭ
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(argv[1]));

	//��ȭ�⿡ ��ȣ �ο�
	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));

	//��ȭ�⸦ ���̺� ����
	listen(hServSock, 5);

	szClntAddr = sizeof(clntAdr);

	//��ȭ�� ���
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &szClntAddr);

	//�޽��� ������
	send(hClntSock, message, sizeof(message), 0);

	//����
	closesocket(hClntSock);
	closesocket(hServSock);

	return 0;
}

/*
* �ռ� ���� ���� ���α׷����� ������ ������ '���� ����' �Ǵ� '������ ����'�̶�� �Ѵ�.
* �ݸ鿡 �̹��� �Ұ��� ������ ������ ��û�ϴ� 'Ŭ���̾�Ʈ ����'�̴�.
* �տ��� ��ȭ�� �Ŵ� �Լ��� �Ұ��ϱ� �ʾҴ�. �̴� Ŭ���̾�Ʈ ������ ������� ȣ���ϴ� �Լ��̱� �����̴�.
* connect �Լ��� ��ȭ�� �Ŵ� �Լ��̴�.
* Ŭ���̾�Ʈ ���α׷������� socket �Լ� ȣ���� ���� ������ ������ connect �Լ� ȣ���� ���� �������� ���� ��û �������� �����Ѵ�.
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