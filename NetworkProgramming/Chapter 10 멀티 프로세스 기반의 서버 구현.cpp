/*
* <<1. �ΰ��� ������ ����>>
* 1. ���� ���� ������ ���� �����
* 1) ��Ƽ ���μ��� ��� ����
*	�ټ��� ���μ����� �����ϴ� ������� ���� ����
* 2) ��Ƽ �÷��� ��� ����
*	����� ����� ��� �����ϴ� ������� ���� ����
* 3) ��Ƽ ������ ��� ����
*	Ŭ���̾�Ʈ�� ����ŭ �����带 �����ϴ� ������� ���� ����
* 
* �̹� ���Ϳ����� ��Ƽ ���μ��� ��� ������ ������ ���̴�. �׷��� �̴� �����쿡���� �������� �ʴ� ����̴�. ���� ���ɾ����� ���� 12�� �Ѿ��.
* 
* 2. ���μ����� ����
* "�޸� ������ ������ ���¿��� �������� ���α׷�"
* 
* 3. ���μ��� ID
* ��� ���μ����� �����Ǵ� ���¿� ������� �ü���κ��� ID�� �ο��޴´�. �̸� ������ '���μ��� ID'��� �Ѵ�.
* 
* 4. fork �Լ��� ���� ���μ����� ����
* ���μ��� �������� �� ���� ����� �ִ�. ���⼭�� fork�� ���� �˾ƺ���.
* 
* #include <unistd.h>
* 
* pit_t fork(void);
* 
* ���� �� ���μ��� ID, ���� �� -1 ��ȯ
* 
* fork �Լ��� ȣ���� ���μ����� ���纻�� �����Ѵ�. �׸��� �� ���μ����� ��� fork �Լ��� ȣ�� ������ ������ �����ϰ� �ȴ�.
* ������ ������ ���α׷� �帧�� fork �Լ��� ��ȯ ���� �������� �������� ���α׷��� �ؾ��Ѵ�.
* 1) �θ� ���μ��� - fork �Լ��� ��ȯ ���� �ڽ� ���μ����� ID
* 2) �ڽ� ���μ��� - fork �Լ��� ��ȯ ���� 0
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
* <<2. ���μ��� & ���� ���μ���>>
* ���μ����� �����ǰ� ���� �� ���� �� �ϸ�, ������� �ϴµ� ������� �ʰ� ���� �Ǿ� �ý����� �߿��� ���ҽ��� �����ϱ⵵ �Ѵ�.
* �� ���¿� �ִ� ���μ����� ������ '���� ���μ���'��� �Ѵ�.
* �̴� �ý��ۿ� �δ��� �ִ� �����̱⿡ �� ������� �Ѵ�.
* 
* 1. ���� ���μ����� ���� ����
* fork �Լ��� ȣ��� ������ ���μ����� ����Ǵ� ��Ȳ�� ������ ����.
* 1) ���ڷ� �����ϸ鼭 exit�� ȣ���ϴ� ���
* 2) main �Լ����� return���� �����ϸ鼭 ���� ��ȯ�ϴ� ���
* 
* exit �Լ��� ȣ��Ǵ� ���� ���� main �Լ����� return ���� ���� ��ȯ�Ǵ� �� ��� �ü���� ���޵ȴ�.
* �׸��� �ü���� �� ���� �ڽ� ���μ����� ������ �θ� ���μ������� ���޵� �� ���� �ڽ� ���μ����� �Ҹ��Ű�� �ʴ´�.
* �ٷ� �� ��Ȳ�� ���� ���μ����� ���� ���μ������ �Ѵ�.
* 
* "�ش� �ڽ� ���μ����� ������ �θ� ���μ������� exit �Լ��� ���� ���̳�, return ���� ��ȯ ���� ���޵Ǿ�� �Ѵ�"
* 
* �׷��ٸ� �̸� ��� �����ұ�? �̴� �θ� ���μ����� �������� ��û�� �־�� �ü���� �� ���� ������ �ش�.
* 
* 2. ���� ���μ����� �Ҹ� 1: wait �Լ��� ���
* 
* #include <sys/wait.h>
* 
* pid_t wait(int* statloc);
* ���� �� ����� �ڽ� ���μ����� ID, ���� �� -1 ��ȯ
* 
* �� �Լ��� ȣ��Ǿ��� ��, �̹� ����� �ڽ� ���μ����� �ִٸ� �ڽ� ���μ����� ����Ǹ鼭 ������ ���� �Ű������� ����.
* �׷��� �� �������� �ٸ� ������ ���ԵǾ� ������ ���� ��ũ�� �Լ��� ���ؼ� �� ������ �����ؾ� �Ѵ�.
* 
* 1) WIFEXITED		- �ڽ� ���μ����� ���� ����� ��� ���� ��ȯ
* 2) WEXITSTATUS	- �ڽ� ���μ����� ���� ���� ��ȯ
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
* �� wait �Լ��� ȣ��� �������� ����� �ڽ� ���μ����� ���ٸ�, ������ �ڽ� ���μ����� ����� �� ���� ���ŷ ���¿� ���δٴ� Ư¡�� �ִ�.
* 
* 3. ���� ���μ����� �Ҹ� 2: waitpid �Լ��� ���
* wait �Լ��� ���ŷ�� ������ �ȴٸ� waitpid �Լ��� ȣ���� ����ϸ� �ȴ�.
* 
* pit_t waitpit(pit_t pid, int* statloc, int options)
* ���� �� ����� �ڽ� ���μ��� ID, ���� �� -1 ��ȯ
* 
* pid		: ���Ḧ Ȯ���ϰ��� �ϴ� �ڽ� ���μ��� ID ����, �̸� ����� -1�� �����ϸ� wait�� ���������� ������ �ڽ� ���μ�����  ����Ǳ� ��ٸ�
* statloc	: wait�� ����
* options	: WNOHANG�� �����ϸ� ���ŷ ���¿� ���� �ʰ� 0�� ��ȯ�ϸ鼭 �Լ��� ��������
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
* <<3. �ñ׳� �ڵ鸵>>
* ���ݱ��� ���μ����� ���� �� �Ҹ꿡 ���ؼ� ����ôµ�, ���� ������ �ִ�.
* "����ü �ڽ� ���μ����� ���� ����� �� �˰� waitpid �Լ��� ��� ȣ���ϰ� �ɾ� ������ ���̾�??"
* 
* 1. �ü���� �ϰ� �� �˷���
* "���� �θ� ���μ���! �ϰ� ������ �ڽ� ���μ����� ����Ǿ���!"
* �׷��� �θ� ���μ����� �ϴ� ���� ��� ���߰� �ڽ� ���μ����� ����� ���õ� ���� �ϸ� �ȴ�.
* �̷��� �ó������� ���α׷� ������ ���ؼ� '�ñ׳� �ڵ鸵'�̶�� ���� �����Ѵ�.
* '�ñ׳�'�� Ư�� ��Ȳ�� �߻������� �˸��� ���� �ü���� ���μ����� �����ϴ� ��ȣ�̰� '�ڵ鸵'�� �� ��ȣ�� ���� �̸� ���ǵ� �۾��� �ǹ��Ѵ�.
* 
* 2. �ñ׳ΰ� signal �Լ�
* ���μ����� �ڽ� ���μ����� ������ ��Ȳ �߻� ��, Ư�� �Լ��� ȣ���� �ü������ �䱸�ϵ��� �ñ׳��� ����� �� �ִ�.
* �� �䱸�� ���� �Լ��� ȣ���� ���ؼ� �̷�����.
* 
* #include <signal.h>
* 
* void* signal(int signo, void func(int));
* �ñ׳� �߻� �� ȣ��ǵ��� ������ ��ϵ� �Լ� ������ ��ȯ
* 
* ���� �Լ��� ȣ���ϸ鼭 ù ��° ���ڷ� Ư�� ��Ȳ�� ���� ������, �ñ׳��� ó���� �Լ��� �����Ѵ�.
* 1. SIGALRM	: alarm �Լ� ȣ���� ���ؼ� ��ϵ� �ð��� �� �� ��Ȳ
* 2. SIGINT		: CTRL + C�� �Էµ� ��Ȳ
* 3. SIGCHLD	: �ڽ� ���μ����� ����� ��Ȳ
* 
* "�ڽ� ���μ����� ����Ǹ� mychild �Լ��� ȣ���� �޶�!"
* 
* signal(SIGCHLD, mychild);
* 
* �̷��� �ñ׳��� ��ϵǸ�, ��ϵ� �ñ׳� �߻� �� �ü���� ������ �Լ��� ȣ���� �ش�. ������ �����ֱ� ���� alaram �Լ��� �Ұ��ϰڴ�.
* 
* unsigned int alaram(unsigned int secondes);
* 0 �Ǵ� SIGALRM �ñ׳��� �߻��ϱ���� �����ִ� �ð��� �� ������ ��ȯ
* 
* �� �Լ��� ȣ���ϸ鼭 ���� ������ ���ڷ� �����ϸ�, �ð��� ������ SIGALRM �ñ׳��� �߻��Ѵ�.
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
* ���� �������� �ϳ� �˾ƾ� �� ����� �ñ׳��� �߻��ϸ� sleep �Լ��� ȣ��� ���ŷ ���¿� �ִ� ���μ����� ����ٴ� ���̴�.
* 
* 3. sigaction �Լ��� �̿��� �ñ׳� �ڵ鸵
* 
* "signal �Լ��� ���н� �迭�� �ü�� ���� ���� ��Ŀ� �־ ���̰� ������ sigaction �Լ��� ���̸� ������ �ʴ´�"
* 
* 4. �ñ׳� �ڵ鸵�� ���� ���� ���μ����� �Ҹ�
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
* <<4. ��Ƽ �½�ŷ ����� ���� ���� ����>>
* ���� �������� fork �Լ� ȣ���� ���� ���� ���� ������ ���� �غ� ���´�.
* 
* 1. ���μ��� ����� ���� ���� ������ ���� ��
* 1) ���� ������ accept �Լ� ȣ���� ���ؼ� ���� ��û�� �����Ѵ�.
* 2) �̶� ��� �Ǵ� ������ ���� ��ũ���͸� �ڽ� ���μ����� �����ؼ� �Ѱ��ش�.
* 3) �ڽ� ���μ����� ���޹޴� ���� ��ũ���͸� �������� ���񽺸� �����Ѵ�.
* 
* 2. ���� ���� ���� ���� ����
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
* 3. fork() �Լ� ȣ���� ���� ���� ��ũ������ ����
* ���ϵ� ����Ǵ� ������� ���� �ݾ��༭ �ڿ� ������ �� ����. ���� ������ ���� �� �� �ִ�.
* 
* <<5. TCP�� ����� ��ƾ ����>>
* 1. ����� ��ƾ ������ �ǹ̿� ����
* �θ� ���μ����� ������ ������ ����ϰ�, ������ ������ �ڽ� ���μ����� �������� �۽��� ����Ѵ�.
* �̷��� ������ ������ �Է°� ����� ����ϴ� ���μ����� ���� �ٸ��� ������
* �������� ���� ���ο� ��� ���� �����͸� ������ �� �ִ�. �̷� ��������� ���ϴ� ������ ���α׷��� ������ �Ѱ� ���������� ���� �ִ�.
* 
* 2. ���� Ŭ���̾�Ʈ�� ����� ��ƾ ����
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
	��

		pid = fork();

	if (pid == 0);
		write_routine(sock, buf);
	else
		read_routine(sock, buf);

	��
}