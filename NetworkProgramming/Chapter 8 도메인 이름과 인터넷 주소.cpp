/*
* <<1. Domain-Name-System(DNS)>>
* IP �ּҿ� ������ �̸� ������ ��ȯ�� ������ �ִ� �ý����̴�.
* 
* 1. ������ �̸��̶�?
* IP �ּҴ� ����� �˾ƺ��� ��Ʊ� ������ �̸� ���ڿ��� ġȯ�� ���� ������ �̸��̶�� �Ѵ�.
* 
* 2. DNS ����
* ������ �̸��� �Է¹����� �װ��� IP �ּҷ� ġȯ�� �ִ� �����̴�.
* 
* <<2. IP �ּҿ� ������ �̸� ������ ��ȯ>>
* 1. ���α׷� �󿡼� ������ �̸��� �� �ʿ䰡 �ִ°�?
* �׻� IP �ּҸ� �ֽ�ȭ �� �� ���� ������ ������ �̸��� ����ϴ� ���� �� ������
* 
* 2. ������ �̸��� �̿��ؼ� IP �ּ� ������
* ���� �Լ��� �̿��ϸ� ���ڿ� ������ ������ �̸����κ��� IP �ּҸ� ���� �� �ִ�.
* 
* struct hostent* gethostbyname(const char* hostname);
* struct hostent* gethostbyaddr
* 
* �� �ּ� ������ hostent ����ü�� ��ܼ� ��ȯ�� �Ǵµ�, �� ����ü�� ������ ���� ���ǵǾ� �ִ�.
* 
* struct hostent{
*	char* h_name;		//���� ������ �̸��� ����ȴ�.
*	char** h_aliases;	//������ �̸��� ������ ����ȴ�.
*	int h_addrtype;		//������ �̸��� IP �ּ� ü�谡 ����ȴ�.
*	int h_length;		//IP �ּ��� ũ�� ������ ����ȴ�.
*	char** h_addr_list;	//������ �̸��� IP �ּҵ��� ����ȴ�.
* }
*/

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

int main()
{
	WSADATA wsaData;
	int i;
	struct hostent* host;

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	host = gethostbyname("www.naver.com");

	printf("official name : %s \n", host->h_name);

	for (i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);

	printf("Address Type : %s \n", host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");

	for (i = 0; host->h_addr_list[i]; i++)
		printf("IP addr : %d : %s \n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	WSACleanup();
	return 0;
}

