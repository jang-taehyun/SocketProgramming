#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN SendAddress;

	WSABUF dataBuf;
	char msg[] = "Network is Computer!";
	int SendBytes = 0;

	WSAEVENT EventObject;
	WSAOVERLAPPED Overlapped;

	int FunctionResult;

	if (3 != argc)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&SendAddress, 0, sizeof(SendAddress));
	SendAddress.sin_family = AF_INET;
	SendAddress.sin_addr.s_addr = inet_addr(argv[1]);
	SendAddress.sin_port = htons(atoi(argv[2]));

	FunctionResult = connect(hSocket, (SOCKADDR*)&SendAddress, sizeof(SendAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("connect() error");

	EventObject = WSACreateEvent();
	memset(&Overlapped, 0, sizeof(Overlapped));
	Overlapped.hEvent = EventObject;
	dataBuf.len = strlen(msg) + 1;
	dataBuf.buf = msg;

	if (SOCKET_ERROR == WSASend(hSocket, &dataBuf, 1, &SendBytes, 0, &Overlapped, NULL))
	{
		if (WSA_IO_PENDING == WSAGetLastError())
		{
			puts("Background data send");
			WSAWaitForMultipleEvents(1, &EventObject, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(hSocket, &Overlapped, &SendBytes, FALSE, NULL);
		}
		else
			ErrorHandling("WSASend() error");
	}

	printf("Send data size: %d\n", SendBytes);
	WSACloseEvent(EventObject);
	closesocket(hSocket);
	WSACleanup();

	return 0;
}