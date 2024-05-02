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
	int fds1[2], fds2[2];
	pid_t pid;
	char buf[BUF_SIZE];

	char str1[] = "Who are you?";
	char str2[] = "Thank you for your message";

	int FunctionResult;

	FunctionResult = pipe(fds1);
	if(FunctionResult)
		ErrorHandling("pipe() 1 error");
	FunctionResult = pipe(fds2);
	if(FunctionResult)
		ErrorHandling("pipe() 2 error");

	pid = fork();
	if(-1 == pid)
		ErrorHandling("fork() error");

	if(pid)
	{
		read(fds1[0], buf, BUF_SIZE);
		printf("Parent process output : %s\n", buf);
		write(fds2[1], str2, sizeof(str2));
		sleep(3);
	}
	else
	{
		write(fds1[1], str1, sizeof(str1));
		read(fds2[0], buf, BUF_SIZE);
		printf("Child process output : %s\n", buf);
	}

	return 0;
}
