#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define BUF_SIZE 30

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int fds[2];
	pid_t pid;
	char buf[BUF_SIZE];

	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";
	
	int FunctionResult;

	FunctionResult = pipe(fds);
	if(FunctionResult)
		ErrorHandling("pipe() error");

	pid = fork();
	if(-1 == pid)
		ErrorHandling("fork() error");

	if(pid)
	{
		read(fds[0], buf, BUF_SIZE);
		printf("Parent process output : %s\n", buf);
		write(fds[1], str2, sizeof(str2));
		sleep(3);
	}
	else
	{
		write(fds[1], str1, sizeof(str1));
		//sleep(2);
		read(fds[0], buf, BUF_SIZE);
		printf("Child process output : %s\n", buf);
	}

	return 0;
}