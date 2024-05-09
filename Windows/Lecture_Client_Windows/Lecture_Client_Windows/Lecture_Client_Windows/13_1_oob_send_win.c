#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
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
	SOCKET SendSocket;
	SOCKADDR_IN AcceptAddress;

	int FunctionResult;

	// argument �˻�
	if (3 != argc)
	{
		printf("Usage %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// ������ ���� ���̺귯�� �ʱ�ȭ
	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	// send socket ����(socket)
	SendSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == SendSocket)
		ErrorHandling("socket() error");

	// accept server address ���� �Է�
	memset(&AcceptAddress, 0, sizeof(AcceptAddress));
	AcceptAddress.sin_family = AF_INET;
	AcceptAddress.sin_addr.s_addr = inet_addr(argv[1]);
	AcceptAddress.sin_port = htons(atoi(argv[2]));

	// accept server address ������ �̿��� server ���� ��û(connect)
	FunctionResult = connect(SendSocket, (SOCKADDR*)&AcceptAddress, sizeof(AcceptAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("connect() error");

	// data ����(write)
	FunctionResult = send(SendSocket, "123", strlen("123"), 0);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("write() 1 error");
	FunctionResult = send(SendSocket, "4", strlen("4"), MSG_OOB);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("write() 2 error");
	FunctionResult = send(SendSocket, "567", strlen("567"), 0);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("write() 3 error");
	FunctionResult = send(SendSocket, "890", strlen("890"), MSG_OOB);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("write() 4 error");

	// send socket ����(closesocket)
	closesocket(SendSocket);

	// ������ ���� ���̺귯�� ����
	WSACleanup();

	return 0;
}