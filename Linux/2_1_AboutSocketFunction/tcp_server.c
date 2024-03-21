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
	int ServerSocket, ClientSocket;
	struct sockaddr_in ServerAddress, ClientAddress;
	socklen_t ClientAddressSize;
	int OperationResult;

	char SendMessage[] = "Hello World!";

	if(2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ServerSocket)
		ErrorHandling("socket() function error");

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	OperationResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == OperationResult)
		ErrorHandling("bind() function error");

	OperationResult = listen(ServerSocket, CONNECT_ABLE_COUNT);
	if(-1 == OperationResult)
		ErrorHandling("listen() function error");


	ClientAddressSize = sizeof(ClientAddress);
	ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
	if(-1 == ClientSocket)
		ErrorHandling("accept() function error");
	
	printf("send message size : %ld\n", sizeof(SendMessage));
	write(ClientSocket, SendMessage, sizeof(SendMessage));

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
