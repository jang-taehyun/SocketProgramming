#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define BUF_SIZE 1024

void CALLBACK CompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(const char* message);

WSABUF dataBuf;
char buf[BUF_SIZE];
int ReceiveBytes = 0;

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hListenSocket, hReceiveSocket;
	SOCKADDR_IN ListenAddress, ReceiveAddress;
	int AddressSize;

	WSAOVERLAPPED Overlapped;
	WSAEVENT EventObject;
	int flags = 0, idx;

	int FunctionResult;

	if (2 != argc)
	{
		printf("Usgae : %s <port>\n", argv[0]);
		exit(1);
	}

	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	hListenSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	memset(&ListenAddress, 0, sizeof(ListenAddress));
	ListenAddress.sin_family = AF_INET;
	ListenAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ListenAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(hListenSocket, (SOCKADDR*)&ListenAddress, sizeof(ListenAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("bind() error");

	FunctionResult = listen(hListenSocket, 5);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("listen() error");

	AddressSize = sizeof(ReceiveAddress);
	hReceiveSocket = accept(hListenSocket, (SOCKADDR*)&ReceiveAddress, &AddressSize);
	if (INVALID_SOCKET == hReceiveSocket)
		ErrorHandling("accept() error");

	memset(&Overlapped, 0, sizeof(Overlapped));
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;
	EventObject = WSACreateEvent();

	if (SOCKET_ERROR == WSARecv(hReceiveSocket, &dataBuf, 1, &ReceiveBytes, &flags, &Overlapped, CompRoutine))
	{
		if (WSA_IO_PENDING == WSAGetLastError())
			puts("Background data receive");
	}

	idx = WSAWaitForMultipleEvents(1, &EventObject, FALSE, WSA_INFINITE, TRUE);
	if (WAIT_IO_COMPLETION == idx)
		puts("Overlapped IO Completed");
	else
		ErrorHandling("WSARecv() error");

	WSACloseEvent(EventObject);
	closesocket(hReceiveSocket);
	closesocket(hListenSocket);
	WSACleanup();
	
	return 0;
}

void CALLBACK CompRoutine(DWORD dwError, DWORD szRecvBytes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	if (dwError)
		ErrorHandling("CompRoutine error");
	else
	{
		ReceiveBytes = szRecvBytes;
		printf("Received message: %s\n", buf);
	}
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}