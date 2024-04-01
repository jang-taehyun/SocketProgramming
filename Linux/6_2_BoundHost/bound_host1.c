#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int Socket;
	char message[BUF_SIZE];
	struct sockaddr_in MyAddress, YourAddress;
	socklen_t AddressSize;
	int StringLength;
	int FunctionResult;

	// argument 검사
	if(2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// socket 생성(socket)
	Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(-1 == Socket)
		ErrorHandling("socket() error");

	// address 정보 입력
	memset(&MyAddress, 0, sizeof(MyAddress));
	memset(&YourAddress, 0, sizeof(MyAddress));
	MyAddress.sin_family = AF_INET;
	MyAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	MyAddress.sin_port = htons(atoi(argv[1]));

	// address 정보를 socket에 할당(bind)
	FunctionResult = bind(Socket, (struct sockaddr*)&MyAddress, sizeof(MyAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	// UDP 통신
	AddressSize = sizeof(YourAddress);
	for(int i=0; i<3; i++)
	{
		memset(message, 0, sizeof(message));
		sleep(5);
		StringLength = recvfrom(Socket, message, BUF_SIZE, 0, (struct sockaddr*)&YourAddress, &AddressSize);
		if(-1 == StringLength)
			ErrorHandling("recvfrom() error");

		printf("message %d : %s\n", i+1, message);
	}

	// socket 해제
	close(Socket);

	return 0;
}
