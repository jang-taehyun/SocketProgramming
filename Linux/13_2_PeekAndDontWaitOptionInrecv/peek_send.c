#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int SendSocket;
	struct sockaddr_in AcceptAddress;
	int FunctionResult;

	// argument 검사
	if(3 != argc)
	{
		printf("Usage %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// send socket 생성(socket)
	SendSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == SendSocket)
		ErrorHandling("socket() error");

	// accept server address 정보 입력
	memset(&AcceptAddress, 0, sizeof(AcceptAddress));
	AcceptAddress.sin_family = AF_INET;
	AcceptAddress.sin_addr.s_addr = inet_addr(argv[1]);
	AcceptAddress.sin_port = htons(atoi(argv[2]));

	// accept server address 정보를 이용해 server에 연결 요청(connect)
	FunctionResult = connect(SendSocket, (struct sockaddr*)&AcceptAddress, sizeof(AcceptAddress));
	if(-1 == FunctionResult)
		ErrorHandling("accept() error");

	// accept server에 data 전송(write)
	FunctionResult = write(SendSocket, "123", strlen("123"));
	if(-1 == FunctionResult)
		ErrorHandling("write() error");

	// send socket 해제(close)
	close(SendSocket);

	return 0;
}
