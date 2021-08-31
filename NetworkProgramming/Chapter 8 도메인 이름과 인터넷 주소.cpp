/*
* <<1. Domain-Name-System(DNS)>>
* IP 주소와 도메인 이름 사이의 변환을 수행해 주는 시스템이다.
* 
* 1. 도메인 이름이란?
* IP 주소는 사람이 알아보기 어렵기 때문에 이를 문자열로 치환한 것을 도메인 이름이라고 한다.
* 
* 2. DNS 서버
* 도메인 이름을 입력받으면 그것을 IP 주소로 치환해 주는 서버이다.
* 
* <<2. IP 주소와 도메인 이름 사이의 변환>>
* 1. 프로그램 상에서 도메인 이름을 쓸 필요가 있는가?
* 항상 IP 주소를 최신화 할 수 없기 때문에 도메인 이름을 사용하는 것이 더 간편함
* 
* 2. 도메인 이름을 이용해서 IP 주소 얻어오기
* 다음 함수를 이용하면 문자열 형태의 도메인 이름으로부터 IP 주소를 얻어올 수 있다.
* 
* struct hostent* gethostbyname(const char* hostname);
* struct hostent* gethostbyaddr
* 
* 단 주소 정보는 hostent 구조체에 담겨서 반환이 되는데, 이 구조체는 다음과 같이 정의되어 있다.
* 
* struct hostent{
*	char* h_name;		//공식 도메인 이름이 저장된다.
*	char** h_aliases;	//도메인 이름의 별명이 저장된다.
*	int h_addrtype;		//도메인 이름의 IP 주소 체계가 저장된다.
*	int h_length;		//IP 주소의 크기 정보가 저장된다.
*	char** h_addr_list;	//도메인 이름의 IP 주소들이 저장된다.
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

