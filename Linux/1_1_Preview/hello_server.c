#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define CONNECT_ABLE_COUNT 5

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int ServerSocket, ClientSocket;				// server socket, client socket
	struct sockaddr_in ServerAddress, ClientAddress;	// server IP address, client IP address
	socklen_t ClientAddressSize;				// client 주소의 크기
	int OperationResult;					// 함수 동작 결과

	char SendMessage[] = "Hello World!";			// 보낼 메세지
	
	// parameter의 개수가 부족하면 error message 출력
	if(2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}	
	
	// server socket 생성
	ServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == ServerSocket)
	{
		ErrorHandling("socket() function error");
	}

	// server address 초기화
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server socket에 IP, port 할당
	OperationResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == OperationResult)
	{
		ErrorHandling("Bind() function error");
	}

	// server socket을 연결 가능 상태로 전환
	OperationResult = listen(ServerSocket, CONNECT_ABLE_COUNT);
	if(-1 == OperationResult)
	{
		ErrorHandling("listen() function error");
	}
	
	// client와 연결
	ClientAddressSize = sizeof(ClientAddress);
	ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
	if(-1 == ClientSocket)
	{
		ErrorHandling("accept() function error");
	}

	// client에 message를 전송
	write(ClientSocket, SendMessage, sizeof(SendMessage));

	// socket 제거
	close(ClientSocket);
	close(ServerSocket);	

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
