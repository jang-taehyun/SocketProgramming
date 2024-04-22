#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	int status;
	pid_t pid1 = fork();
	pid_t pid2;
	pid_t check1, check2;

	// child process 실행
	if(0 == pid1)
	{
		return 3;
	}
	// parent process 실행
	else
	{
		printf("Child 1 PID : %d\n", pid1);
		pid2 = fork();

		// child process 실행
		if(0 == pid2)
		{
			exit(7);
		}
		// parent process 실행
		else
		{
			printf("Child 2 PID : %d\n", pid2);

			// 1번째 wait 함수 호출
			check1 = wait(&status);
			if(WIFEXITED(status))
			{
				printf("check 1 pid : %d\n", check1);
				printf("Child send one : %d\n", WEXITSTATUS(status));
			}

			// 2번째 wait 함수 호출
			check2 = wait(&status);
			if(WIFEXITED(status))
			{
				printf("check 2 pid : %d\n", check2);
				printf("Child send two : %d\n", WEXITSTATUS(status));
			}

			sleep(30);
		}
	}

	return 0;
}
