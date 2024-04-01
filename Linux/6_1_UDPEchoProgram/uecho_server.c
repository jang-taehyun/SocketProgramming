#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
#define CONNECT_COUNT 3

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int ServerSocket;
	char message[BUF_SIZE];
	int StringLength;
	struct sockaddr_in ServerAddress, ClientAddress;
	socklen_t ClientAddressSize;
	int FunctionResult;

	// argument 검사
	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// server socket 생성(socket)
	ServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(-1 == ServerSocket)
		ErrorHandling("socket() error");

	// server address 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server socket에 server address 정보 할당(bind)
	FunctionResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	// UDP 통신
	ClientAddressSize = sizeof(ClientAddress);
	memset(&ClientAddress, 0, ClientAddressSize);
	for(int i=0; i<CONNECT_COUNT; i++)
	{
		StringLength = recvfrom(ServerSocket, message, BUF_SIZE, 0, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
		if(-1 == StringLength)
			ErrorHandling("recvfrom() error");

		StringLength = sendto(ServerSocket, message, BUF_SIZE, 0, (struct sockaddr*)&ClientAddress, ClientAddressSize);
		if(-1 == StringLength)
			ErrorHandling("sendto() error");
	}

	// server socket 해제
	close(ServerSocket);

	return 0;
}
