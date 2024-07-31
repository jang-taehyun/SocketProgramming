#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define BUF_SIZE 1024

void Errorhandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hListenSocket, hReceiveSocket;
	SOCKADDR_IN ListenAddress, ReceiveAddress;
	int AddressSize;

	WSABUF dataBuf;
	WSAEVENT EventObject;
	WSAOVERLAPPED Overlapped;

	char buf[BUF_SIZE];
	int ReceiveByte = 0, flags = 0;

	int FunctionResult;

	if (2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		Errorhandling("WSAStartup() error");

	hListenSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&ListenAddress, 0, sizeof(ListenAddress));
	ListenAddress.sin_family = AF_INET;
	ListenAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ListenAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(hListenSocket, (SOCKADDR*)&ListenAddress, sizeof(ListenAddress));
	if (SOCKET_ERROR == FunctionResult)
		Errorhandling("bind() error");

	FunctionResult = listen(hListenSocket, 5);
	if (SOCKET_ERROR == FunctionResult)
		Errorhandling("listen() error");

	AddressSize = sizeof(ReceiveAddress);
	hReceiveSocket = accept(hListenSocket, (SOCKADDR*)&ReceiveAddress, &AddressSize);

	EventObject = WSACreateEvent();
	memset(&Overlapped, 0, sizeof(Overlapped));
	Overlapped.hEvent = EventObject;
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;

	if (SOCKET_ERROR == WSARecv(hReceiveSocket, &dataBuf, 1, &ReceiveByte, &flags, &Overlapped, NULL))
	{
		if (WSAGetLastError() == WSA_IO_PENDING)
		{
			puts("Background data receive");
			WSAWaitForMultipleEvents(1, &EventObject, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(hReceiveSocket, &Overlapped, &ReceiveByte, FALSE, NULL);
		}
		else
			Errorhandling("WSARecv() error");
	}

	printf("Received message: %s\n", buf);
	WSACloseEvent(EventObject);
	closesocket(hReceiveSocket);
	closesocket(hListenSocket);
	WSACleanup();
	
	return 0;
}