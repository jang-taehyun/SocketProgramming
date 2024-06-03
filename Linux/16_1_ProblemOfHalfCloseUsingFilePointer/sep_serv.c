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
	socklen_t AddressSize;

	FILE* readfp, * writefp;

	char buf[BUF_SIZE] = {0, };
	int StringLength;
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
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	FunctionResult = listen(ServerSocket, 5);
	if(-1 == FunctionResult)
		ErrorHandling("listen() error");

	AddressSize = sizeof(ClientAddress);
	ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &AddressSize);
	if(-1 == ClientSocket)
		ErrorHandling("accept() error");

	readfp = fdopen(ClientSocket, "r");
	writefp = fdopen(ClientSocket, "w");

	fputs("FROM SERVER : Hi~ Client?\n", writefp);
	fputs("I love all of the world\n", writefp);
	fputs("Yor are awesome!\n", writefp);
	fflush(writefp);

	fclose(writefp);
	fgets(buf, sizeof(buf), readfp);
	fputs(buf, stdout);
	fclose(readfp);

	return 0;
}
