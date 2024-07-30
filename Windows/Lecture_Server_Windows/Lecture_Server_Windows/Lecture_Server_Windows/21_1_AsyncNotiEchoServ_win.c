#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#define BUF_SIZE 100

void CompressSockets(SOCKET hSocketArray[], int idx, int total);
void CompressEvents(WSAEVENT hEventArray[], int idx, int total);
void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServerSocket, ClientSocket;
	SOCKADDR_IN ServerAddress, ClientAddress;
	int AddressSize;

	SOCKET hSocketArray[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT hEventArray[WSA_MAXIMUM_WAIT_EVENTS];
	int NumOfClientSocket = 0;
	int PositionInfo, StartIdx;

	WSAEVENT NewEvent;
	WSANETWORKEVENTS NetworkEvent;

	char msg[BUF_SIZE];
	int StringLength;

	int FunctionResult;

	if (2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == ServerSocket)
		ErrorHandling("socket() error");

	AddressSize = sizeof(ClientAddress);
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(ServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("bind() error");

	FunctionResult = listen(ServerSocket, 5);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("listen() error");

	NewEvent = WSACreateEvent();
	FunctionResult = WSAEventSelect(ServerSocket, NewEvent, FD_ACCEPT);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("WSAEventSelect() error");

	hSocketArray[NumOfClientSocket] = ServerSocket;
	hEventArray[NumOfClientSocket] = NewEvent;
	NumOfClientSocket++;

	while (1)
	{
		PositionInfo = WSAWaitForMultipleEvents(NumOfClientSocket, hEventArray, FALSE, WSA_INFINITE, FALSE);
		StartIdx = PositionInfo - WSA_WAIT_EVENT_0;

		for (int i = StartIdx; i < NumOfClientSocket; i++)
		{
			int SignalEventIdx = WSAWaitForMultipleEvents(1, &hEventArray[i], TRUE, 0, FALSE);
			if (WSA_WAIT_FAILED == SignalEventIdx || WSA_WAIT_TIMEOUT == SignalEventIdx)
				continue;

			SignalEventIdx = i;
			WSAEnumNetworkEvents(hSocketArray[SignalEventIdx], hEventArray[SignalEventIdx], &NetworkEvent);

			if (NetworkEvent.lNetworkEvents & FD_ACCEPT)
			{
				if (NetworkEvent.iErrorCode[FD_ACCEPT_BIT])
				{
					puts("Accept Error");
					break;
				}

				ClientSocket = accept(hSocketArray[SignalEventIdx], (SOCKADDR*)&ClientAddress, &AddressSize);
				NewEvent = WSACreateEvent();

				WSAEventSelect(ClientSocket, NewEvent, FD_READ | FD_CLOSE);

				hEventArray[NumOfClientSocket] = NewEvent;
				hSocketArray[NumOfClientSocket] = ClientSocket;
				NumOfClientSocket++;

				puts("Connected new client...");
			}

			if (NetworkEvent.lNetworkEvents & FD_READ)
			{
				if (NetworkEvent.iErrorCode[FD_READ_BIT])
				{
					puts("Read error");
					break;
				}

				StringLength = recv(hSocketArray[SignalEventIdx], msg, sizeof(msg), 0);
				send(hSocketArray[SignalEventIdx], msg, StringLength, 0);
			}

			if (NetworkEvent.lNetworkEvents & FD_CLOSE)
			{
				if (NetworkEvent.iErrorCode[FD_CLOSE_BIT])
				{
					puts("Close error");
					break;
				}

				WSACloseEvent(hEventArray[SignalEventIdx]);
				closesocket(hSocketArray[SignalEventIdx]);

				NumOfClientSocket--;
				CompressSockets(hSocketArray, SignalEventIdx, NumOfClientSocket);
				CompressEvents(hEventArray, SignalEventIdx, NumOfClientSocket);

				puts("Close client...");
			}
		}
	}

	closesocket(ServerSocket);
	WSACleanup();

	return 0;
}

void CompressSockets(SOCKET hSocketArray[], int idx, int total)
{
	for (int i = idx; i < total; i++)
		hSocketArray[i] = hSocketArray[i + 1];
}

void CompressEvents(WSAEVENT hEventArray[], int idx, int total)
{
	for (int i = idx; i < total; i++)
		hEventArray[i] = hEventArray[i + 1];
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}