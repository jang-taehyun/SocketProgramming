#include <stdio.h>
#include <arpa/inet.h>

typedef unsigned long ULong;

int main(int argc, char* argv[])
{
	ULong ConvertAddress;

	// argv[0]는 명령어의 첫번째 입력값(./파일이름)이므로 1부터 시작
	for(int i=1; i<argc; i++)
	{
		ConvertAddress = inet_addr(argv[i]);
		if(INADDR_NONE == ConvertAddress)
			printf("%s : Error occured!\n", argv[i]);
		else
			printf("Network ordered integer address : %#lx\n", ConvertAddress);
	}

	return 0;
}
