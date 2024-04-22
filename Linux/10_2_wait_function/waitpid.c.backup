#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int status;
	int check;

	// child process 생성
	pid_t pid = fork();
	if(-1 == pid)
	{
		puts("fork() error");
		return 1;
	}

	// child process execute
	if(0 == pid)
	{
		sleep(15);
		return 24;
	}
	// parent process execute
	else
	{
		printf("Child PID : %d\n", pid);

		while(!(check = waitpid(-1, &status, WNOHANG)))
		{
			if(-1 == check)
			{
				printf("waitpid() error\n");
				sleep(30);
				continue;
			}

			puts("start sleep 3sec.");
			printf("sleep ");
			for(int i=1; i<=3; i++)
			{
				printf("%d ", i);
				sleep(1);
			}
			puts("\n");
		}

		if(WIFEXITED(status))
		{
			printf("check %d\n", check);
			printf("Child send %d\n", WEXITSTATUS(status));
		}
	}

	return 0;
}
