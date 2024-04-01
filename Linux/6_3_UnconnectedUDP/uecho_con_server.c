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

	if(argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	ServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(-1 == ServerSocket)
		ErrorHandling("socket() error");

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	memset(&ClientAddress, 0, sizeof(ClientAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");
	
	ClientAddressSize = sizeof(ClientAddress);
	for(int i=0; i<CONNECT_COUNT; i++)
	{
		StringLength = recvfrom(ServerSocket, message, BUF_SIZE, 0, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
		if(-1 == StringLength)
			ErrorHandling("recvfrom() error");

		StringLength = sendto(ServerSocket, message, BUF_SIZE, 0, (struct sockaddr*)&ClientAddress, ClientAddressSize);
		if(-1 == StringLength)
			ErrorHandling("sendto() error");

		memset(message, 0, sizeof(message));
	}

	close(ServerSocket);


	return 0;
}
