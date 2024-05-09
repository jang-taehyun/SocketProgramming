#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_SIZE 30

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int SendSocket;
	struct sockaddr_in ReceiveAddress;
	int FunctionResult;

	// argument 검사
	if(3 != argc)
	{
		printf("Usage %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// socket 생성(socket) 및 receive server address 정보 입력
	SendSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == SendSocket)
		ErrorHandling("socket() error");
	memset(&ReceiveAddress, 0, sizeof(ReceiveAddress));
	ReceiveAddress.sin_family = AF_INET;
	ReceiveAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ReceiveAddress.sin_port = htons(atoi(argv[2]));

	// receive server에 연결 요청(connect)
	FunctionResult = connect(SendSocket, (struct sockaddr*)&ReceiveAddress, sizeof(ReceiveAddress));
	if(-1 == FunctionResult)
		ErrorHandling("connect() error");

	// receive server에 데이터 전송(write, send)
	FunctionResult = write(SendSocket, "123", strlen("123"));
	if(-1 == FunctionResult)
		ErrorHandling("write() 1 error");
	FunctionResult = send(SendSocket, "4", strlen("4"), MSG_OOB);
	if(-1 == FunctionResult)
		ErrorHandling("send() 1 error");
	FunctionResult = write(SendSocket, "567", strlen("567"));
	if(-1 == FunctionResult)
		ErrorHandling("write() 2 error");
	FunctionResult = send(SendSocket, "890", strlen("890"), MSG_OOB);
	if(-1 == FunctionResult)
		ErrorHandling("send() 2 error");

	// send socket 해제(close)
	close(SendSocket);

	return 0;
}
