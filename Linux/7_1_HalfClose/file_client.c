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
	int ClientSocket;
	struct sockaddr_in ServerAddress;

	FILE* fp;
	char buf[BUF_SIZE];
	int ReadCount;
	int FunctionResult;

	if(3 != argc)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	fp = fopen("receive.dat", "wb");
	memset(&ServerAddress, 0, sizeof(ServerAddress));

	ClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ClientSocket)
		ErrorHandling("socket() error");

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	FunctionResult = connect(ClientSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("connect() error");

	while(1)
	{
		ReadCount = read(ClientSocket, buf, BUF_SIZE);
		if(!ReadCount)
			break;
		else if(-1 == ReadCount)
			ErrorHandling("read() error");

		fwrite((void*)buf, 1, ReadCount, fp);
	}

	puts("Received file data");
	write(ClientSocket, "Thank you", 10);

	fclose(fp);
	close(ClientSocket);

	return 0;
}
