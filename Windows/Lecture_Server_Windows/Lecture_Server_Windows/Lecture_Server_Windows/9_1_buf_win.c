#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void ShowSocketBufferSize(SOCKET socket)
{
	int SendBufferSize, ReceiveBufferSize;
	int state, OptionLength;

	// output buffer의 크기 추출
	OptionLength = sizeof(SendBufferSize);
	state = getsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char*)&SendBufferSize, &OptionLength);
	if (SOCKET_ERROR == state)
		ErrorHandling("output buffer getsockopt() error");

	// input buffer의 크기 추출
	OptionLength = sizeof(ReceiveBufferSize);
	state = getsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char*)&ReceiveBufferSize, &OptionLength);
	if (SOCKET_ERROR == state)
		ErrorHandling("input buffer getsockopt() error");

	// output buffer, input buffer 크기 출력
	printf("Input buffer size : %d\n", ReceiveBufferSize);
	printf("Output buffer size : %d\n", SendBufferSize);
}

int main()
{
	WSADATA wsaData;
	SOCKET HandleSocket;
	int SendBufferSize = 1024 * 3;
	int ReceiveBufferSize = 1024 * 3;
	int state;

	// 윈도우 소켓 라이브러리 초기화
	state = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (state)
		ErrorHandling("WSAStartup() error");

	// socket 생성
	HandleSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleSocket)
		ErrorHandling("socket() error");

	// 현재 socket의 입출력 버퍼의 크기 출력
	ShowSocketBufferSize(HandleSocket);

	// socket의 입출력 버퍼의 크기 변경
	state = setsockopt(HandleSocket, SOL_SOCKET, SO_SNDBUF, (char*)&SendBufferSize, sizeof(SendBufferSize));
	if (SOCKET_ERROR == state)
		ErrorHandling("output buffer setsockopt() error");
	state = setsockopt(HandleSocket, SOL_SOCKET, SO_RCVBUF, (char*)&ReceiveBufferSize, sizeof(ReceiveBufferSize));
	if (SOCKET_ERROR == state)
		ErrorHandling("input buffer setsockopt() error");

	// 현재 socket의 입출력 버퍼의 크기 출력
	ShowSocketBufferSize(HandleSocket);

	// socket 해제
	closesocket(HandleSocket);

	// 윈도우 소켓 라이브러리 해제
	WSACleanup();

	return 0;
}