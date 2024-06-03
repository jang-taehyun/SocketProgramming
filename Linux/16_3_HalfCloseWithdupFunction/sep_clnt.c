#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int ClientSocket;
	struct sockaddr_in ServerAddress;

	FILE* readfp, * writefp;

	char buf[BUF_SIZE];
	int FunctionResult;

	if(3 != argc)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	ClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ClientSocket)
		ErrorHandling("socket() error");

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	FunctionResult = connect(ClientSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("connect() error");

	readfp = fdopen(ClientSocket, "r");
	writefp = fdopen(ClientSocket, "w");

	while(1)
	{
		if(NULL == fgets(buf, sizeof(buf), readfp))
			break;

		fputs(buf, stdout);
		fflush(stdout);
	}

	fputs("FROM CLIENT : Thank you!\n", writefp);
	fflush(writefp);
	
	fclose(writefp);
	fclose(readfp);

	return 0;
}
