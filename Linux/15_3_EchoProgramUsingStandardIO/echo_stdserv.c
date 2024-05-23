#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_SIZE 1024

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

	char message[BUF_SIZE];
	int StringLength;

	FILE* Readfp;
        FILE* Writefp;
	
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
	AddressSize = sizeof(ClientAddress);
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	FunctionResult = listen(ServerSocket, 5);
	if(-1 == FunctionResult)
		ErrorHandling("listen() error");

	for(int i=0; i<5; i++)
	{
		ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &AddressSize);
		if(-1 == ClientSocket)
			ErrorHandling("accept() error");
		else
			printf("Connected client %d\n", i+1);

		Readfp = fdopen(ClientSocket, "r");
		Writefp = fdopen(ClientSocket, "w");

		while(!feof(Readfp))
		{
			fgets(message, BUF_SIZE, Readfp);
			fputs(message, Writefp);
			fflush(Writefp);
		}

		fclose(Readfp);
		fclose(Writefp);
		close(ClientSocket);
	}

	close(ServerSocket);

	return 0;
}
