/*
* <<1. 두가지 유형의 서버>>
* 1. 다중 접속 서버의 구현 방법들
* 1) 멀티 프로세스 기반 서버
*	다수의 프로세스를 생성하는 방식으로 서비스 제공
* 2) 멀티 플렉싱 기반 서버
*	입출력 대상을 묶어서 관리하는 방식으로 서비스 제공
* 3) 멀티 쓰레딩 기반 서버
*	클라이언트의 수만큼 쓰레드를 생성하는 방식으로 서비스 제공
* 
* 이번 쳅터에서는 멀티 프로세스 기반 서버를 설명할 것이다. 그러나 이는 윈도우에서는 지원하지 않는 방식이다. 따라서 관심없으면 쳅터 12로 넘어가라.
* 
* 2. 프로세스의 이해
* "메모리 공간을 차지한 상태에서 실행중인 프로그램"
* 
* 3. 프로세스 ID
* 모든 프로세스는 생성되는 형태에 상관없이 운영체제로부터 ID를 부여받는다. 이를 가르켜 '프로세스 ID'라고 한다.
* 
* 4. fork 함수를 통한 프로세스의 생성
* 프로세스 생성에는 몇 가지 방법이 있다. 여기서는 fork에 대해 알아보자.
* 
* #include <unistd.h>
* 
* pit_t fork(void);
* 
* 성공 시 프로세스 ID, 실패 시 -1 반환
* 
* fork 함수는 호출한 프로세스의 복사본을 생성한다. 그리도 두 프로세스는 모두 fork 함수의 호출 이후의 문장을 실행하게 된다.
* 때문에 이후의 프로그램 흐름은 fork 함수의 반환 값을 기준으로 나뉘도록 프로그래밍 해야한다.
* 1) 부모 프로세스 - fork 함수의 반환 값은 자식 프로세스의 ID
* 2) 자식 프로세스 - fork 함수의 반환 값은 0
*/

#include <stdio.h>
#include <unistd.h>

int gval = 10;
int main()
{
	pid_t pid;
	int lval = 20;
	gval++, lval += 5;

	pid = fork();

	if (pid == 0) // child
		gval += 2, lval += 2;
	else         // parent
		gval -= 2, lval -= 2;

	if (pid == 0)
		printf("child proc : %d, %d", gval, lval);
	else
		printf("parent proc : %d, %d", gval, lval);

	return 0;
}

/*
* <<2. 프로세스 & 좀비 프로세스>>
* 프로세스가 생성되고 나서 할 일을 다 하면, 사라져야 하는데 사라지지 않고 좀비가 되어 시스템의 중요한 리소스를 차지하기도 한다.
* 이 상태에 있는 프로세스를 가르켜 '좀비 프로세스'라고 한다.
* 이는 시스템에 부담을 주는 원인이기에 꼭 없애줘야 한다.
* 
* 1. 좀비 프로세스의 생성 이유
* fork 함수의 호출로 생성된 프로세스가 종료되는 상황은 다음과 같다.
* 1) 인자로 전달하면서 exit를 호출하는 경우
* 2) main 함수에서 return문을 실행하면서 값을 반환하는 경우
* 
* exit 함수로 호출되는 인자 값과 main 함수에서 return 문에 의해 반환되는 값 모두 운영체제로 전달된다.
* 그리고 운영체제는 이 값이 자식 프로세스를 생성한 부모 프로세스에게 전달될 때 까지 자식 프로세스를 소멸시키지 않는다.
* 바로 이 상황에 놓인 프로세스를 좀비 프로세스라고 한다.
* 
* "해당 자식 프로세스를 생성한 부모 프로세스에게 exit 함수의 인자 값이나, return 문의 반환 값이 전달되어야 한다"
* 
* 그렇다면 이를 어떻게 전달할까? 이는 부모 프로세스의 적극적인 요청이 있어야 운영체제는 그 값을 전달해 준다.
* 
* 2. 좀비 프로세스의 소멸 1: wait 함수의 사용
* 
* #include <sys/wait.h>
* 
* pid_t wait(int* statloc);
* 성공 시 종료된 자식 프로세스의 ID, 실패 시 -1 반환
* 
* 위 함수가 호출되었을 때, 이미 종료된 자식 프로세스가 있다면 자식 프로세스가 종료되면서 전달한 값이 매개변수로 들어간다.
* 그런데 이 변수에는 다른 정보도 포함되어 있으니 다음 메크로 함수를 통해서 이 정보를 추출해야 한다.
* 
* 1) WIFEXITED		- 자식 프로세스가 정상 종료된 경우 참을 반환
* 2) WEXITSTATUS	- 자식 프로세스의 전달 값을 반환
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int status;

	pid_t pid = fork();

	if (pid == 0) // child one
	{
		return 3;
	}
	else         // parent
	{
		printf("child PID : %d", pid);

		pid = fork();

		if (pid == 0)  // child two
		{
			exit(7);
		}
		else          // parent
		{
			printf("child PID : %d", pid);

			wait(&status);

			if (WIFEXITED(status))
				printf("child send one : %d, WEXITSTATUS(status));

			wait(&status)

			if (WIFEXITED(status))
				printf("child send two : %d, WEXITSTATUS(status));

			sleep(30);
		}
	}

	return 0;
}

/*
* 이 wait 함수는 호출된 시점에서 종료된 자식 프로세스가 없다면, 임의의 자식 프로세스가 종료될 때 까지 블로킹 상태에 놓인다는 특징이 있다.
* 
* 3. 좀비 프로세스의 소멸 2: waitpid 함수의 사용
* wait 함수의 블로킹이 문제가 된다면 waitpid 함수의 호출을 고려하면 된다.
* 
* pit_t waitpit(pit_t pid, int* statloc, int options)
* 성공 시 종료된 자식 프로세스 ID, 실패 시 -1 반환
* 
* pid		: 종료를 확인하고자 하는 자식 프로세스 ID 전달, 이를 대신해 -1을 전달하면 wait와 마찬가지로 임의의 자식 프로세스가  종료되길 기다림
* statloc	: wait와 같음
* options	: WNOHANG을 전달하면 블로킹 상태에 있지 않고 0을 반환하면서 함수를 빠져나옴
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int status;

	pid_t pid = fork();

	if (pid == 0)
	{
		sleep(15);
		return 24;
	}
	else
	{
		// if terminated child process does not exist, it return 0 else it 
		// return not 0
		while (!waitpid(-1, &status, WNOHANG)
		{
			sleep(3);
			puts("sleep 3sec");
		}

		if (WIFEXITED(status))
			printf("child send : %d", WEXITSTATUS(status));
	}

	return 0;
}

/*
* <<3. 시그널 핸들링>>
* 지금까지 프로세스의 생성 및 소멸에 대해서 살펴봤는데, 아직 문제가 있다.
* "도대체 자식 프로세스가 언제 종료될 줄 알고 waitpid 함수를 계속 호출하고 앉아 있으란 말이야??"
* 
* 1. 운영체제야 니가 좀 알려줘
* "어이 부모 프로세스! 니가 생성한 자식 프로세스가 종료되었어!"
* 그러면 부모 프로세스는 하던 일을 잠시 멈추고 자식 프로세스의 종료와 관련된 일을 하면 된다.
* 이러한 시나리오의 프로그램 구현을 위해서 '시그널 핸들링'이라는 것이 존재한다.
* '시그널'은 특정 상황이 발새했음을 알리기 위한 운영체제가 프로세스에 전달하는 신호이고 '핸들링'은 그 신호에 따른 미리 정의된 작업을 의미한다.
* 
* 2. 시그널과 signal 함수
* 프로세스는 자식 프로세스의 종료라는 상황 발생 시, 특정 함수의 호출을 운영체제에게 요구하도록 시그널을 등록할 수 있다.
* 이 요구는 다음 함수의 호출을 통해서 이뤄진다.
* 
* #include <signal.h>
* 
* void* signal(int signo, void func(int));
* 시그널 발생 시 호출되도록 이전에 등록된 함수 포인터 반환
* 
* 위의 함수를 호출하면서 첫 번째 인자로 특정 상황에 대한 정보를, 시그널을 처리할 함수를 전달한다.
* 1. SIGALRM	: alarm 함수 호출을 통해서 등록된 시간이 다 된 상황
* 2. SIGINT		: CTRL + C가 입력된 상황
* 3. SIGCHLD	: 자식 프로세스가 종료된 상황
* 
* "자식 프로세스가 종료되면 mychild 함수를 호출해 달라!"
* 
* signal(SIGCHLD, mychild);
* 
* 이렇게 시그널이 등록되면, 등록된 시그널 발생 시 운영체제는 전달한 함수를 호출해 준다. 예제를 보여주기 전에 alaram 함수를 소개하겠다.
* 
* unsigned int alaram(unsigned int secondes);
* 0 또는 SIGALRM 시그널이 발생하기까지 남아있는 시간을 초 단위로 반환
* 
* 위 함수를 호출하면서 양의 정수를 인자로 전달하면, 시간이 지나서 SIGALRM 시그널이 발생한다.
*/

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if (sig == SIGALRM)
		puts("time out!");
	alarm(2);
}

void keycontrol(int sig)
{
	if (sig == SIGINT)
		puts("CTRL + C pressed");
}

int main()
{
	int i;
	signal(SIGALRM, timeout);
	signal(SIGINT, keycontrol);
	alarm(2);

	for (i = 0; i < 3; i++)
	{
		puts("wait..");
		sleep(100);
	}

	return 0;
}

/*
* 위의 예제에서 하나 알아야 할 사실은 시그널이 발생하면 sleep 함수의 호출로 블로킹 상태에 있던 프로세스가 깨어난다는 것이다.
* 
* 3. sigaction 함수를 이용한 시그널 핸들링
* 
* "signal 함수는 유닉스 계열의 운영체제 별로 동작 방식에 있어서 차이가 있지만 sigaction 함수는 차이를 보이지 않는다"
* 
* 4. 시그널 핸들링을 통한 좀비 프로세스의 소멸
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
	int status;
	pid_t id = waitpid(-1, &status, WNOHANG);
	if (WIFEXITED(status))
	{
		printf("Removed proc id : % \n", id);
		printf("Child send : %d \n", WEXITSTATUS(status));
	}
}

int main()
{
	pid_t pid;
	signal(SIGCHLD, read_childproc);

	pid = fork();

	if (pid == 0)
	{
		puts("Hi! I'm child process");
		sleep(10);
		return 12;
	}
	else
	{
		printf("Child proc id : %d \n", pid);

		pid = fork();

		if (pid == 0);
		{
			puts("Hi! I'm child process");
			sleep(10);
			exit(24);
		}
		else
		{
			int i;
			printf("Child proc id : %d \n", pid);

			for (i = 0; i < 5; i++)
			{
				puts("wait..");
				sleep(5);
			}
		}
	}

	return 0;
}

/*
* <<4. 멀티 태스킹 기반의 다중 접속 서버>>
* 드디어 여러분은 fork 함수 호출을 통한 다중 접속 서버의 구현 준비를 끝냈다.
* 
* 1. 프로세스 기반의 다중 접속 서버의 구현 모델
* 1) 에코 서버는 accept 함수 호출을 통해서 연결 요청을 수락한다.
* 2) 이때 얻게 되는 소켓의 파일 디스크럽터를 자식 프로세스를 생성해서 넘겨준다.
* 3) 자식 프로세스는 전달받는 파일 디스크럽터를 바탕으로 서비스를 제공한다.
* 
* 2. 다중 접속 에코 서버 구현
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void read_childproc(int sig)

int main(int argc, char* argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;

	pid_t pid;
	struct sigaction act;
	socklen_t adr_sz;
	int str_len, state;
	char buf[BUF_SIZE];

	act.sa_handler = read_childproc;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	state = sigaction(SIGCHLD, &act, 0);

	serv_sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1]));

	bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));

	listen(serv_sock, 5);

	while (1)
	{
		adr_sz = sizeof(clnt_adr);

		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);

		pid = fork();

		if (pid == 0)
		{
			close(serv_sock);

			while (str_len = read(clnt_sock, buf, BUF_SIZE)) != 0)
				write(clnt_sock, buf, str_len);

			close(clnt_sock);
			puts("client disconnected..");
			return 0;
		}
		else
			close(clnt_sock);
	}

	close(serv_sock);
	return 0;
}

void read_childproc(int sig)
{
	pit_t pid;
	int status;

	pid = waitpid(-1, &status, WNOHANG);

	printf("removed proc id : %d \n", pid);
}
/*
* 3. fork() 함수 호출을 통한 파일 디스크럽터의 복사
* 소켓도 복사되니 상관없는 것은 닫아줘서 자원 관리를 잘 하자. 위의 예제를 보면 알 수 있다.
* 
* <<5. TCP의 입출력 루틴 분할>>
* 1. 입출력 루틴 분할의 의미와 이점
* 부모 프로세스는 데이터 수신을 담당하고, 별도로 생성된 자식 프로세스는 데이터의 송신을 담당한다.
* 이렇게 구현해 놓으면 입력과 출력을 담당하는 프로세스가 각각 다르기 때문에
* 데이터의 수신 여부에 상관 없이 데이터를 전송할 수 있다. 이런 구현방식을 택하는 이유는 프로그램의 구현이 한결 수월해지는 데에 있다.
* 
* 2. 에코 클라이언트의 입출력 루틴 분할
*/
void read_routine(int sock, char* buf)
{
	while (1)
	{
		int str_len = read(sock, buf, BUF_SIZE);

		if (str_len == 0)
			return;

		buf[str_len] = 0;

		printf("Message From Server : %s \n", buf);
	}
}

void write_routine(int sock, char* buf)
{
	while (1)
	{
		fgets(buf, BUF_SIZE, stdin);

		if (!strcmp(buf, "q\n" || !strcmp(buf, "Q\n"))
		{
			shutdown(sock, SHUT_WR);
				return;
		}

		write(sock, buf, strlen(buf));
	}
}

int main(int argc, char* argv[])
{
	…

		pid = fork();

	if (pid == 0);
		write_routine(sock, buf);
	else
		read_routine(sock, buf);

	…
}