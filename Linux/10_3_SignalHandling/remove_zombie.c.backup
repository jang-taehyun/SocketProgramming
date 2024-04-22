#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
	int status;
	pid_t id = waitpid(-1, &status,  WNOHANG);
	if(WIFEXITED(status))
	{
		printf("Removed process ID : %d\n", id);
		printf("Child send : %d\n", WEXITSTATUS(status));
	}
}

int main()
{
	pid_t pid1, pid2;
	
	// signal handling 함수 지정
	struct sigaction act;
	act.sa_handler = read_childproc;

	// option값 초기화
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	// signal handling 등록
	sigaction(SIGCHLD, &act, 0);

	// child process 1 생성
	pid1 = fork();
	if(-1 == pid1)
	{
		puts("fork() error");
		return 1;
	}

	// child process 1 실행
	if(0 == pid1)
	{
		puts("Hi, I'm child process 1");
		sleep(10);
		return 12;
	}
	// parent process 실행
	else
	{
		printf("Child process 1 ID : %d\n", pid1);
		
		// child process 2 생성
		pid2 = fork();
		if(-1 == pid2)
		{
			puts("fork() 2 error");
			return 1;
		}

		// child process 2 실행
		if(0 == pid2)
		{
			puts("Hi! I'm child process 2");
			sleep(10);
			exit(24);
		}
		// parent process 실행
		else
		{
			printf("Child process 2 ID : %d\n", pid2);
			for(int i=0; i<5; i++)
			{
				puts("wait...");
				sleep(5);
			}
		}
	}

	return 0;
}
