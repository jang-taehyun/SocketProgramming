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
	int SendSocket;
	struct sockaddr_in BroadcastAddress;

	int SocketOptionBroadcast = 1;

	FILE* fp;
	
	char buf[BUF_SIZE];

	int FunctionResult;

	if(3 != argc)
	{
		printf("Usage: %s <Broadcast IP> <port>\n", argv[0]);
		exit(1);
	}

	int StringLength;
	SendSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(-1 == SendSocket)
		ErrorHandling("socket() error");

	memset(&BroadcastAddress, 0, sizeof(BroadcastAddress));
	BroadcastAddress.sin_family = AF_INET;
	BroadcastAddress.sin_addr.s_addr = inet_addr(argv[1]);
	BroadcastAddress.sin_port = htons(atoi(argv[2]));

	setsockopt(SendSocket, SOL_SOCKET, SO_BROADCAST, (void*)&SocketOptionBroadcast, sizeof(SocketOptionBroadcast));

	fp = fopen("news.txt", "r");
	if(NULL == fp)
		ErrorHandling("fopen() error");

	while(!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(SendSocket, buf, strlen(buf), 0, (struct sockaddr*)&BroadcastAddress, sizeof(BroadcastAddress));
		sleep(2);
	}

	close(SendSocket);

	return 0;
}
