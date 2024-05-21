#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
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
	int ReceiveSocket;
	struct sockaddr_in ReceiveAddress;

	char buf[BUF_SIZE];
	int StringLength;

	int FunctionResult;

	if(2 != argc)
	{
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}

	ReceiveSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(-1 == ReceiveSocket)
		ErrorHandling("socket() error");

	memset(&ReceiveAddress, 0, sizeof(ReceiveAddress));
	ReceiveAddress.sin_family = AF_INET;
	ReceiveAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ReceiveAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(ReceiveSocket, (struct sockaddr*)&ReceiveAddress, sizeof(ReceiveAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	while(1)
	{
		StringLength = recvfrom(ReceiveSocket, buf, BUF_SIZE-1, 0, NULL, 0);
		if(StringLength < 0)
			break;

		buf[StringLength] = 0;
		fputs(buf, stdout);
	}

	close(ReceiveSocket);

	return 0;
}
