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
	int RecievSocket;
	struct sockaddr_in SenderAddress;
	struct ip_mreq JoinAddress;

	int StringLength;
	char buf[BUF_SIZE];
	int FunctionResult;

	if(3 != argc)
	{
		printf("Usage: %s <GroupIP> <Port>\n", argv[0]);
		exit(1);
	}

	RecievSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if(-1 == RecievSocket)
		ErrorHandling("socket() error");

	memset(&SenderAddress, 0, sizeof(SenderAddress));
	SenderAddress.sin_family = AF_INET;
	SenderAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	SenderAddress.sin_port = htons(atoi(argv[2]));

	FunctionResult = bind(RecievSocket, (struct sockaddr*)&SenderAddress, sizeof(SenderAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	JoinAddress.imr_multiaddr.s_addr = inet_addr(argv[1]);
	JoinAddress.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(RecievSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&JoinAddress, sizeof(JoinAddress));

	while(1)
	{
		StringLength = recvfrom(RecievSocket, buf, BUF_SIZE-1, 0, NULL, 0);
		if(StringLength < 0)
			break;

		buf[StringLength] = 0;
		fputs(buf, stdout);
	}

	close(RecievSocket);

	return 0;
}
