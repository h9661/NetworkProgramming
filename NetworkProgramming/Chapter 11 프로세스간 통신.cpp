/*
* <<1. 프로세스간 통신의 기본 개념>>
* 프로세스간 통신이 가능하다는 것은 서로 다른 두 프로세스가 데이터를 주고받을 수 있다는 의미이다. 이렇게 되기 위해선 두 프로세스가
* 동시에 접근 가능한 메모리 공간이 있어야 한다.
* 
* 1. 프로세스간 통신의 기본 이해
* 프로세스는 서로 완전히 별개의 메모리 구조를 지닌다. 따라서 서로 통신하려면 별도로 마련된 방법을 통해서만 이뤄질 수 있다.
* 
* 2. 파이프(Pipe) 기반의 프로세스간 통신
* 프로세스간 통신을 위해서는 파이프라는 것을 생성해야 한다. 이 파이프는 운영체제에 속하는 자원이다.
* 
* #include <unistd.h>
* int pipe(int fields[2]);
* 성공 시 0, 실패 시 -1 반환
* 
* fields[0] : 파이프로부터 데이터를 수신하는데 사용되는 파일 디스크럽터 즉 출구
* fields[1] : 파이프로부터 데이터를 전송하는데 사용되는 파일 디스크럽터 즉 입구
* 
* 예제)
*/
#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	int fds[2];
	char str[] = "who are you?";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds);

	pid = fork();

	if (pid == 0)
	{
		write(fds[1], str, sizeof(str));
	}
	else
	{
		read(fds[0], buf, BUF_SIZE);
		puts(buf);
	}

	return 0;
}

/*
* 여기서 중요한 점은 파이프에는 단방향으로만 데이터를 보낼 수 있다는 것이다.
* 
* 3. 파이프 기반의 프로세스간 양방향 통신
* 그렇다면 어떻게 양방향 통신을 할 수 있을까?
* 
* "파이프를 두 개 생성합니다!"
*/
#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
	int fds1[2], int fds2[2];
	char str1[] = "who are you?";
	char str2[] = "Thank you for message";
	char buf[BUF_SIZE];
	pid_t pid;

	pipe(fds1), pipe(fds2);

	pid = fork()

		if (pid == 0)
		{
			write(fds[1], str1, sizeof(str1));
			read(fds2[0], buf, BUF_SIZE);
			printf("Child proc output : %s \n", buf);
		}
		else
		{
			read(fds1[2], buf, BUF_SIZE);
			write(fds2[1], str2, sizeof(str2));
			printf("Parent proc output : %s \n", buf);
			sleep(3);
		}

	return 0;
}

/*
* <<2. 프로세스간 통신의 적용>>
* 프로세스간 통신은 서버의 구현에 직접적인 연관은 없다. 그러나 운영체제를 이해한다는 측면에서 나름의 의미가 있다.
* 
* 1. 메시지를 저장하는 형태의 에코 서버
* 
* 2. 
*/