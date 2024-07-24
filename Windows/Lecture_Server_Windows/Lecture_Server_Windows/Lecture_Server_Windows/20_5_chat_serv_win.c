#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <process.h>
#define BUF_SIZE 100
#define MAX_CLNT 256

unsigned WINAPI HandleClnt(void* arg);

void SendMsg(char* msg, int len);
void ErrorHandling(const char* message);

int clntCnt;
SOCKET clntSocks[MAX_CLNT];
HANDLE hMutex;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServerSoskcet, hClientSocket;
	SOCKADDR_IN ServerAddress, ClientAddress;
	int AddressSize;

	HANDLE hThread;

	int FunctionResult;

	if (2 != argc)
	{
		printf("Usage %s : <port>\n", argv[0]);
		exit(1);
	}

	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	hMutex = CreateMutex(NULL, FALSE, NULL);

	hServerSoskcet = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == hServerSoskcet)
		ErrorHandling("socket() error");

	AddressSize = sizeof(ClientAddress);
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(hServerSoskcet, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("bind() error");

	FunctionResult = listen(hServerSoskcet, 5);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("listen() error");

	while (1)
	{
		hClientSocket = accept(hServerSoskcet, (SOCKADDR*)&ClientAddress, &AddressSize);
		if (INVALID_SOCKET == hClientSocket)
		{
			fputs("accept() error", stdout);
			break;
		}

		WaitForSingleObject(hMutex, INFINITE);
		clntSocks[clntCnt++] = hClientSocket;
		ReleaseMutex(hMutex);

		hThread = (HANDLE)_beginthreadex(NULL, 0, HandleClnt, (void*)&hClientSocket, 0, NULL);
		printf("Connected client IP: %s\n", inet_ntoa(ClientAddress.sin_addr));
	}

	closesocket(hServerSoskcet);
	WSACleanup();

	return 0;
}

unsigned WINAPI HandleClnt(void* arg)
{
	SOCKET hClientSocket = *((SOCKET*)arg);

	int StringLength = 0;
	char msg[BUF_SIZE];

	while (1)
	{
		StringLength = recv(hClientSocket, msg, sizeof(msg), 0);
		if (!StringLength)
			break;

		SendMsg(msg, StringLength);
	}

	WaitForSingleObject(hMutex, INFINITE);

	for (int i = 0; i < clntCnt; i++)
	{
		if (hClientSocket == clntSocks[i])
		{
			while (i++ < clntCnt - 1)
				clntSocks[i] = clntSocks[i + 1];
			break;
		}
	}

	clntCnt--;

	ReleaseMutex(hMutex);
	closesocket(hClientSocket);

	return 0;
}

void SendMsg(char* msg, int len)
{
	WaitForSingleObject(hMutex, INFINITE);
	for (int i = 0; i < clntCnt; i++)
		send(clntSocks[i], msg, len, 0);
	ReleaseMutex(hMutex);
}

void ErrorHandling(const char* message)
{
	fputs(message, stdout);
	fputc('\n', stdout);
	exit(1);
}