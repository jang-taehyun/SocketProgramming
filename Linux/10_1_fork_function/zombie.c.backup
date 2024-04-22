#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid = fork();
	if(-1 == pid)
	{
		puts("fork() error");
		exit(1);
	}

	// parent process execute
	if(pid)
	{
		printf("Child process ID : %d\n", pid);
		sleep(30);
	}
	// child process execute
	else
	{
		puts("Hi, I'm child process!!");
		printf("fork() function return value in Child process : %d\n", pid);
	}

	// parent process execute
	if(pid)
	{
		puts("End Parent process");
	}
	// child process execute
	else
	{
		puts("End Child process");
	}

	return 0;
}
