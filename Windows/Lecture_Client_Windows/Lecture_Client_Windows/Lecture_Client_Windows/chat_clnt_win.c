#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#define BUF_SIZE 100
#define NAME_SIZE 20

unsigned WINAPI SendMsg(void* arg);
unsigned WINAPI RecvMsg(void* arg);

void ErrorHandling(const char* message);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN ServerAddress;

	HANDLE hSendThread, hRecvThread;

	int FunctionResult;

	if (4 != argc)
	{
		printf("Usage : %s <IP> <port> <name>\n", argv[0]);
		exit(1);
	}

	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	sprintf(name, "[%s]", argv[3]);

	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == hSocket)
		ErrorHandling("socket() error");

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	FunctionResult = connect(hSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("connect() error");

	hSendThread = (HANDLE)_beginthreadex(NULL, 0, SendMsg, (void*)&hSocket, 0, NULL);
	hRecvThread = (HANDLE)_beginthreadex(NULL, 0, RecvMsg, (void*)&hSocket, 0, NULL);

	WaitForSingleObject(hSendThread, INFINITE);
	WaitForSingleObject(hRecvThread, INFINITE);

	closesocket(hSocket);
	WSACleanup();

	return 0;
}

unsigned WINAPI SendMsg(void* arg)
{
	SOCKET hSocket = *((SOCKET*)arg);
	char NameMsg[NAME_SIZE + BUF_SIZE];

	while (1)
	{
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			closesocket(hSocket);
			exit(0);
		}

		sprintf(NameMsg, "%s %s", name, msg);
		send(hSocket, NameMsg, strlen(NameMsg), 0);
	}

	return 0;
}

unsigned WINAPI RecvMsg(void* arg)
{
	SOCKET hSocket = *((SOCKET*)arg);
	char NameMsg[NAME_SIZE + BUF_SIZE];
	int StringLength;

	while (1)
	{
		StringLength = recv(hSocket, NameMsg, NAME_SIZE + BUF_SIZE - 1, 0);
		if (-1 == StringLength)
			return -1;

		NameMsg[StringLength] = 0;
		fputs(NameMsg, stdout);
	}

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}