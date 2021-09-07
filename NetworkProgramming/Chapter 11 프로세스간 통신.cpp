/*
* <<1. ���μ����� ����� �⺻ ����>>
* ���μ����� ����� �����ϴٴ� ���� ���� �ٸ� �� ���μ����� �����͸� �ְ���� �� �ִٴ� �ǹ��̴�. �̷��� �Ǳ� ���ؼ� �� ���μ�����
* ���ÿ� ���� ������ �޸� ������ �־�� �Ѵ�.
* 
* 1. ���μ����� ����� �⺻ ����
* ���μ����� ���� ������ ������ �޸� ������ ���Ѵ�. ���� ���� ����Ϸ��� ������ ���õ� ����� ���ؼ��� �̷��� �� �ִ�.
* 
* 2. ������(Pipe) ����� ���μ����� ���
* ���μ����� ����� ���ؼ��� ��������� ���� �����ؾ� �Ѵ�. �� �������� �ü���� ���ϴ� �ڿ��̴�.
* 
* #include <unistd.h>
* int pipe(int fields[2]);
* ���� �� 0, ���� �� -1 ��ȯ
* 
* fields[0] : �������κ��� �����͸� �����ϴµ� ���Ǵ� ���� ��ũ���� �� �ⱸ
* fields[1] : �������κ��� �����͸� �����ϴµ� ���Ǵ� ���� ��ũ���� �� �Ա�
* 
* ����)
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
* ���⼭ �߿��� ���� ���������� �ܹ������θ� �����͸� ���� �� �ִٴ� ���̴�.
* 
* 3. ������ ����� ���μ����� ����� ���
* �׷��ٸ� ��� ����� ����� �� �� ������?
* 
* "�������� �� �� �����մϴ�!"
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
* <<2. ���μ����� ����� ����>>
* ���μ����� ����� ������ ������ �������� ������ ����. �׷��� �ü���� �����Ѵٴ� ���鿡�� ������ �ǹ̰� �ִ�.
* 
* 1. �޽����� �����ϴ� ������ ���� ����
* 
* 2. 
*/