#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define CONNECT_COUNT 5

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int ServerSocket, ClientSocket;
	struct sockaddr_in ServerAddress, ClientAddress;
	socklen_t ClientAddressSize;
	int OperationResult;

	char SendMessage[] = "Hello world!";

	// argument 검사
	if(2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// socket 생성(socket)
	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ServerSocket)
		ErrorHandling("socket() error");

	// server 주소 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server socket에 IP, port 할당(bind)
	OperationResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == OperationResult)
		ErrorHandling("bind() error");

	// server socket를 연결 요청 대기 상태로 전환(listen)
	OperationResult = listen(ServerSocket, CONNECT_COUNT);
	if(-1 == OperationResult)
		ErrorHandling("listen error");

	// client으로 부터 연결 요청을 받는다.(accept)
	ClientAddressSize = sizeof(ClientAddress);
	ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
	if(-1 == ClientSocket)
		ErrorHandling("accept() error");

	// client에게 데이터(메시지) 전송(write)
	write(ClientSocket, SendMessage, sizeof(SendMessage));

	// client socket 해제
	close(ClientSocket);

	// server socket 해제
	close(ServerSocket);

	return 0;
}

