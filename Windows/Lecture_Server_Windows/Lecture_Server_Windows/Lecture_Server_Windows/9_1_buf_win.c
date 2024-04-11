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

	// output buffer�� ũ�� ����
	OptionLength = sizeof(SendBufferSize);
	state = getsockopt(socket, SOL_SOCKET, SO_SNDBUF, (char*)&SendBufferSize, &OptionLength);
	if (SOCKET_ERROR == state)
		ErrorHandling("output buffer getsockopt() error");

	// input buffer�� ũ�� ����
	OptionLength = sizeof(ReceiveBufferSize);
	state = getsockopt(socket, SOL_SOCKET, SO_RCVBUF, (char*)&ReceiveBufferSize, &OptionLength);
	if (SOCKET_ERROR == state)
		ErrorHandling("input buffer getsockopt() error");

	// output buffer, input buffer ũ�� ���
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

	// ������ ���� ���̺귯�� �ʱ�ȭ
	state = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (state)
		ErrorHandling("WSAStartup() error");

	// socket ����
	HandleSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleSocket)
		ErrorHandling("socket() error");

	// ���� socket�� ����� ������ ũ�� ���
	ShowSocketBufferSize(HandleSocket);

	// socket�� ����� ������ ũ�� ����
	state = setsockopt(HandleSocket, SOL_SOCKET, SO_SNDBUF, (char*)&SendBufferSize, sizeof(SendBufferSize));
	if (SOCKET_ERROR == state)
		ErrorHandling("output buffer setsockopt() error");
	state = setsockopt(HandleSocket, SOL_SOCKET, SO_RCVBUF, (char*)&ReceiveBufferSize, sizeof(ReceiveBufferSize));
	if (SOCKET_ERROR == state)
		ErrorHandling("input buffer setsockopt() error");

	// ���� socket�� ����� ������ ũ�� ���
	ShowSocketBufferSize(HandleSocket);

	// socket ����
	closesocket(HandleSocket);

	// ������ ���� ���̺귯�� ����
	WSACleanup();

	return 0;
}