#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main()
{
	fd_set reads, temps;
	int result, StringLength;
	char buf[BUF_SIZE];
	struct timeval timeout;

	// fd_set 변수 초기화 및 standard input을fd_set 변수에 등록
	FD_ZERO(&reads);
	FD_SET(0, &reads);

	while(1)
	{
		temps = reads;
		
		// timeout 설정
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		// event 발생 확인(select)
		result = select(1, &temps, 0, 0, &timeout);
		if(-1 == result)
		{
			puts("select() error");
			break;
		}

		// event가 발생하지 않는 경우(timeout이 된 경우)
		if(0 == result)
		{
			puts("Time-out!");
		}
		// event가 발생한 경우
		else if(result > 0)
		{
			// fd_set에 등록한 standard input event가 발생한 경우
			if(FD_ISSET(0, &temps))
			{
				StringLength = read(0, buf, BUF_SIZE);
				buf[StringLength] = 0;
				printf("message from console : %s", buf);
			}
		}
	}

	return 0;
}
