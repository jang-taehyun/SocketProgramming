#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
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
	SOCKET HandleClientSocket;
	SOCKADDR_IN ServerAddress;

	FILE* fp;
	char buf[BUF_SIZE];
	int ReadCount;
	int FunctionResult;

	if (3 != argc)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	fp = fopen("receive.dat", "wb");
	memset(&ServerAddress, 0, sizeof(ServerAddress));

	HandleClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("socket() error");

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	FunctionResult = connect(HandleClientSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("connect() error");

	while (1)
	{
		ReadCount = recv(HandleClientSocket, buf, BUF_SIZE, 0);
		if (SOCKET_ERROR == ReadCount)
			ErrorHandling("recv() error");
		else if (!ReadCount)
			break;

		fwrite((void*)buf, 1, ReadCount, fp);
	}

	puts("Received file data");
	FunctionResult = send(HandleClientSocket, "Thank you", 10, 0);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("send() error");

	fclose(fp);
	closesocket(HandleClientSocket);
	WSACleanup();

	return 0;
}