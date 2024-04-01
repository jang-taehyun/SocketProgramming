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
	struct sockaddr_in YourAddress;
	int FunctionResult;

	char msg1[] = "Hi!";
	char msg2[] = "I'm another UDP host";
	char msg3[] = "Nice to meet you!";

	// argument 검사
	if(3 != argc)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// socket 생성(socket)
	Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(-1 == Socket)
		ErrorHandling("socket() error");

	// address 정보 입력
	memset(&YourAddress, 0, sizeof(YourAddress));
	YourAddress.sin_family = AF_INET;
	YourAddress.sin_addr.s_addr = inet_addr(argv[1]);
	YourAddress.sin_port = htons(atoi(argv[2]));

	// UDP 통신
	FunctionResult = sendto(Socket, msg1, sizeof(msg1), 0, (struct sockaddr*)&YourAddress, sizeof(YourAddress));
	if(-1 == FunctionResult)
		ErrorHandling("sendto() 1 error");
	FunctionResult = sendto(Socket, msg2, sizeof(msg2), 0, (struct sockaddr*)&YourAddress, sizeof(YourAddress));
	if(-1 == FunctionResult)
		ErrorHandling("sendto() 2 error");
	FunctionResult = sendto(Socket, msg3, sizeof(msg3), 0, (struct sockaddr*)&YourAddress, sizeof(YourAddress));
	if(-1 == FunctionResult)
		ErrorHandling("sendto() 3 error");

	// socket 해제
	close(Socket);

	return 0;
}
