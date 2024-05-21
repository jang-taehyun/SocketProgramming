#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <WS2tcpip.h>
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
	WSADATA wsaData;
	SOCKET SendSocket;
	SOCKADDR_IN MulticastAddress;
	int time_live = TTL;
	FILE* fp;
	char buf[BUF_SIZE];
	int FunctionResult;

	if (3 != argc)
	{
		printf("Usage : %s <GroupIP> <port>\n", argv[0]);
		exit(1);
	}

	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	SendSocket = socket(PF_INET, SOCK_DGRAM, 0);
	
	memset(&MulticastAddress, 0, sizeof(MulticastAddress));
	MulticastAddress.sin_family = AF_INET;
	MulticastAddress.sin_addr.s_addr = inet_addr(argv[1]);
	MulticastAddress.sin_port = htons(atoi(argv[2]));

	setsockopt(SendSocket, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

	fp = fopen("news.txt", "r");
	if (NULL == fp)
		ErrorHandling("fopen() error");

	while (!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(SendSocket, buf, strlen(buf), 0, (SOCKADDR*)&MulticastAddress, sizeof(MulticastAddress));
		Sleep(2000);
	}

	closesocket(SendSocket);
	WSACleanup();

	return 0;
}