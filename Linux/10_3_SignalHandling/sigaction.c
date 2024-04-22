#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig)
{
	if(SIGALRM == sig)
		puts("Time out!");
	alarm(2);
}

int main()
{
	// sigaction 생성 및 signal handling 함수 지정
	struct sigaction act;
	act.sa_handler = timeout;

	// signal 관련 옵션 초기화
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	// signal handling 등록
	sigaction(SIGALRM, &act, 0);

	alarm(2);
	for(int i=0; i<3; i++)
	{
		puts("wait...");
		sleep(100);
	}

	return 0;
}
