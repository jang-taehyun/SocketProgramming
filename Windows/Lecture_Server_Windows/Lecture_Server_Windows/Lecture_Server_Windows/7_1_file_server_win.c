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
	SOCKET HandleServerSocket, HandleClientSocket;
	SOCKADDR_IN ServerAddress, ClientAddress;
	int ClientAddressSize = sizeof(ClientAddress);

	FILE* fp;
	char buf[BUF_SIZE];
	int ReadCount;
	int FunctionResult;

	if (2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	fp = fopen("./tmp.txt", "rb");
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	memset(&ClientAddress, 0, sizeof(ClientAddress));

	HandleServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleServerSocket)
		ErrorHandling("socket() error");

	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(HandleServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("bind() error");

	FunctionResult = listen(HandleServerSocket, 5);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("listen() error");

	HandleClientSocket = accept(HandleServerSocket, (SOCKADDR*)&ClientAddress, &ClientAddressSize);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("accept() error");

	while (1)
	{
		ReadCount = fread((void*)buf, 1, BUF_SIZE, fp);
		if (ReadCount < BUF_SIZE)
		{
			FunctionResult = send(HandleClientSocket, (char*)&buf, ReadCount, 0);
			if (SOCKET_ERROR == FunctionResult)
				ErrorHandling("send() 1 error");

			break;
		}

		FunctionResult = send(HandleClientSocket, (char*)&buf, BUF_SIZE, 0);
		if (SOCKET_ERROR == FunctionResult)
			ErrorHandling("send() 2 error");
	}

	FunctionResult = shutdown(HandleClientSocket, SD_SEND);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("shutdown() client error");

	FunctionResult = recv(HandleClientSocket, (char*)buf, BUF_SIZE, 0);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("recv() error");

	printf("Message from client : %s\n", buf);

	fclose(fp);
	closesocket(HandleClientSocket);
	closesocket(HandleServerSocket);
	WSACleanup();

	return 0;
}