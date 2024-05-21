#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
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
	SOCKET ReceiveSocket;
	SOCKADDR_IN ReceiveAddress;
	struct ip_mreq JoinAddress;
	char buf[BUF_SIZE];
	int StringLength;
	int FunctionResult;

	if (3 != argc)
	{
		printf("Usage : %s <GroupIP> <port>\n", argv[0]);
		exit(1);
	}

	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	ReceiveSocket = socket(PF_INET, SOCK_DGRAM, 0);
	memset(&ReceiveAddress, 0, sizeof(ReceiveAddress));
	ReceiveAddress.sin_family = AF_INET;
	ReceiveAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ReceiveAddress.sin_port = htons(atoi(argv[2]));

	FunctionResult = bind(ReceiveSocket, (SOCKADDR*)&ReceiveAddress, sizeof(ReceiveAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("bind() error");

	JoinAddress.imr_multiaddr.s_addr = inet_addr(argv[1]);
	JoinAddress.imr_interface.s_addr = htonl(INADDR_ANY);

	FunctionResult = setsockopt(ReceiveSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&JoinAddress, sizeof(JoinAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("setsockopt() error");

	while (1)
	{
		StringLength = recvfrom(ReceiveSocket, buf, BUF_SIZE - 1, 0, NULL, 0);
		if (StringLength < 0)
			break;
		buf[StringLength] = 0;
		fputs(buf, stdout);
	}

	closesocket(ReceiveSocket);
	WSACleanup();

	return 0;
}