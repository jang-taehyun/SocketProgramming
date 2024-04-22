#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int gval = 10;			// 전역 변수

int main()
{
	pid_t pid;
	int lval = 20;

	gval++;
	lval += 5;

	// child process 생성
	pid = fork();
	if(-1 == pid)
	{
		puts("fork() error");
		exit(1);
	}

	// parent process 실행
	if(pid)
	{
		gval -= 2;
		lval -= 2;
	}
	// child process 실행
	else
	{
		gval += 2;
		lval += 2;
	}

	// parent process 실행
	if(pid)
	{
		printf("Parent Process : [gval, lval] [%d, %d]\n", gval, lval);
		printf("Parent Process : pid %d\n", pid);
	}
	else
	{
		printf("Child Process : [gval, lval] [%d, %d]\n", gval, lval);
		printf("Child Process : pid %d\n", pid);
	}
	

	return 0;
}
