#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define TTL 64
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
	struct sockaddr_in MultiAddress;
	int time_live=TTL;
	FILE* fp;
	char buf[BUF_SIZE];

	if(3 != argc)
	{
		printf("Usage: %s <GroupIP> <Port>\n", argv[0]);
		exit(1);
	}

	SendSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if(-1 == SendSocket)
		ErrorHandling("socket() error");

	memset(&MultiAddress, 0, sizeof(MultiAddress));
	MultiAddress.sin_family = AF_INET;
	MultiAddress.sin_addr.s_addr = inet_addr(argv[1]);
	MultiAddress.sin_port = htons(atoi(argv[2]));

	setsockopt(SendSocket, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
	
	fp = fopen("news.txt", "r");
	if(NULL == fp)
		ErrorHandling("fopen() error");

	while(!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(SendSocket, buf, strlen(buf), 0, (struct sockaddr*)&MultiAddress, sizeof(MultiAddress));
		sleep(2);
	}

	fclose(fp);
	close(SendSocket);

	return 0;
}
