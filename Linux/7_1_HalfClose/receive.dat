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
	int ServerSocket, ClientSocket;
	struct sockaddr_in ServerAddress, ClientAddress;
	socklen_t ClientAddressSize;

	FILE* fp;
	char buf[BUF_SIZE];
	int ReadCount;
	int FunctionResult;

	if(2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ServerSocket)
		ErrorHandling("socket() error");

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	memset(&ClientAddress, 0, sizeof(ClientAddress));
	ClientAddressSize = sizeof(ClientAddress);

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	FunctionResult = listen(ServerSocket, 5);
	if(-1 == FunctionResult)
		ErrorHandling("listen() error");

	ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
	if(-1 == ClientSocket)
		ErrorHandling("accept() error");

	fp = fopen("file_server.c", "rb");
	while(1)
	{
		ReadCount = fread((void*)buf, 1, BUF_SIZE, fp);
		if(ReadCount < BUF_SIZE)
		{
			FunctionResult = write(ClientSocket, buf, ReadCount);
			if(-1 == FunctionResult)
				ErrorHandling("write() 1 error");

			break;
		}

		FunctionResult = write(ClientSocket, buf, BUF_SIZE);
		if(-1 == FunctionResult)
			ErrorHandling("write() 2 error");

	}

	shutdown(ClientSocket, SHUT_WR);
	FunctionResult = read(ClientSocket, buf, BUF_SIZE);
	if(-1 == FunctionResult)
		ErrorHandling("read() error");

	printf("Message from client : %s\n", buf);

	fclose(fp);
	close(ClientSocket);
	close(ServerSocket);

	return 0;
}
