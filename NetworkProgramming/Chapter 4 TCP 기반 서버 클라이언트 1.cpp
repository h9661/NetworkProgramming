/*
* <<1. TCP�� UDP�� ���� ����>>
* ���ͳ� �������� ��� ������ ���, ������ ���� ��Ŀ� ���� TCP, UDP �������� �����ٰ� ���ߴ�.
* TCP�� ������ ���� ������ ��Ʈ�� �̶�� ���� ��� �ִ�. ������ �̸� ��Ȯ�� �����ϱ� ���ؼ��� ��Ʈ���� ����� ������ ���� �˾ƾ� �Ѵ�.
* 
* 1. TCP �������� ����
* TCP�� �̾߱��ϱ� �ռ� TCP�� �����ִ� TCP/IP �������� ������ ���� ���캸��
* 4)				APPLICATION ����
* 3)	TCP ����						UDP ����
* 2)					IP ����
* 1)					LINK ����
* ���� ���� �� �װ��� �������� ������.
* 
* 2. TCP/IP ���������� ź�� ���
* ū ������ ���� ������ ������, �� �������� ���������� ������ ����������� ���� ������ ���ؼ� ��ȣ�� ���踦 �ΰ� �Ǿ���.
* 
* 3. LINK ����
* ������ ������ ǥ��ȭ�� ���� ����̴�. LAN, WAN, MAN ���� ��Ʈ��ũ ǥ�ذ� ���õ� ���������� �����ϴ� �����̴�. ��ǻ�Ͱ� ������ ������ ����Ѵ�.
* 
* 4. IP ����
* ������ ������ �����Ǿ����� �����͸� ������ �����̴�. � ��η� �����͸� ������ �ϴ� ������ �ذ��ϴ� ���� IP �����̴�.
* IP ������ �� ������������ �ŷ��� �� ���� ���������̴�.
* 
* 5. TCP/UDP ����
* �������� ������ ���� ����� �˻��� IP �������� ���ִ�, �� ��θ� �������� �����͸� �����ϸ� �ȴ�. �� ������ ���� ������ �ۼ����� ����Ѵ�.
* ȣ��Ʈ �� ȣ��Ʈ�� ������ �ۼ��� ����� ����� ����ϴ� ���� TCP/UDP �����̴�.
* 
* 6. APPLICATION ����
* ����ڵ鰣�� ����̴�.
* 
* <<2. TCP ��� ����, Ŭ���̾�Ʈ ����>>
* ���� TCP ��� ������ �����غ���.
* 
* 1. TCP ���������� �⺻���� �Լ� ȣ�� ����
* 1) socket()
* 2) bind()
* 3) listen()
* 4) accept()
* 5) read()/write()
* 6) close()
* 
* socket �Լ� ȣ�� �� bind�� ���Ͽ� �ּ� �Ҵ��� �������, �� ���Ŀ� ���� �������.
* 
* 2. �����û ��� ���·� ����
* bind �Լ��� ���ؼ� ���Ͽ� �ּұ��� �Ҵ��ߴٸ�, listen �Լ� ȣ���� ���ؼ� '���� ��û ��� ����'�� ���� �Ѵ�.
* int listen(SOCKET socket, int backlog)
* �� ��° ������ ũ�⿡ �ش��ϴ� ������ ���������, �̸� ���� ��û ��� ť��� �Ѵ�.
* 
* 3. Ŭ���̾�Ʈ�� ���� ��û ����
* listen �Լ� ȣ�� ���� Ŭ���̾�Ʈ�� ���� ��û�� ����, ���� ������� ���� ��û�� �����ؾ� �Ѵ�.
* int accept(SOCKET socket, SOCKADDR* addr, socklen_t* addrlen)
* accept �Լ��� ���� ��û ��� ť�� �ִ� Ŭ���̾�Ʈ�� ���� ��û�� �����ϴ� �Լ��̴�.
* ���� accept �Լ��� ���������� ������ �����ϰ� �� ������ �ڵ��� ��ȯ�Ѵ�. �߿��� ���� ������ �ڵ����� �����Ǿ�, ���� ��û�� Ŭ���̾�Ʈ ���Ͽ�
* ������� �̷����ٴ� ���̴�.
* 
* 4. TCP Ŭ���̾�Ʈ�� �⺻���� �Լ� ȣ�� ����
* 1) socket()
* 2) connect()
* 3) read()/write()
* 4) close()
* 
* ���� ������ �������� connect �Լ��� ȣ���̴�.
* connect �Լ��� ȣ���ϸ� ������ ���� ��û ��� ť�� ��ϵȴ�. ���� �ٷ� ����Ǵ� ���� �ƴϴ�.
* 
* 5. TCP ��� ����/Ŭ���̾�Ʈ�� �Լ� ȣ�� ����
*	 [SERVER]			  [CLIENT]
* 1) socket()			  socket()
* 2) bind()	  <------��
* 3) listen() <------���� connect()
* 4) accept()
* 5) read()/write()		  write()/read()
* 6) close()		      close()
* 
* <<3. Iterative ����� ����, Ŭ���̾�Ʈ ����>>
* �̹����� ����ؼ� ������ Ŭ���̾�Ʈ�� ���� ��û�� �����ϴ� Iterative ������ ������ �� ���̴�.
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

	//���� �ʱ�ȭ
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//Ŭ���̾�Ʈ ���� ����
	hSocket = socket(PF_INET, SOCK_STREAM, 0);

	//������ ���� �ּ� �ʱ�ȭ
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.s_addr = inet_addr(argv[1]);
	servAdr.sin_port = htons(atoi(argv[2]));

	//�ʱ�ȭ�� ���� �ּҿ� ����
	connect(hSocket, (SOCKADDR*)&servAdr, sizeof(servAdr));

	while (1)
	{
		//�������� ���� ���ڿ� �Է�
		fputs("Input message(Q to quit):", stdout);
		fgets(message, 50, stdin);

		if (message[0] == 'q' || message[0] == 'Q')
			break;

		//�������� ���ڿ� ����
		send(hSocket, message, 50, 0);

		//�����κ��� ���ڿ� ����
		strLen = recv(hSocket, message, 50 - 1, 0);

		//���� ���� ���ڿ� ���� NULL���� �־ ���ڿ��� ���� �˸���.
		message[strLen] = 0;

		//���ڿ� ���
		printf("Message from server: %s \n", message);
	}


	//����
	closesocket(hSocket);
	WSACleanup();
}

/*
* �ؿ��� Ŭ���̾�Ʈ�� ��������
* ���� ���� �ڵ忡���� �� �ѹ��� read �Լ��� ȣ���Ѵ�. ������ tcp ������ Ư¡�� �����ΰ�? �ٷ� �������� ��谡 �������� �ʴ´ٴ� ���̴�.
* ���� ������ �����ϴ� ���ڿ��� ���̰� �� ���� ���� read �Լ��� ȣ���Ѵٸ� �̴� Ŭ���̾�Ʈ�κ��� ���۵Ǵ� �����͸� ��� �������� ���ϴ� ��Ȳ����
* �̾��� �� �ִ�. �̿� ���� �ش��� ���� ���Ϳ��� �˾ƺ���.
*/