#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUF_SIZE 30
#define CONNECT_COUNT 3

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET ServerSocket;
	SOCKADDR_IN ServerAddress, ClientAddress;
	int ClientAddressSize;

	char message[BUF_SIZE];
	int StringLength;
	int FunctionResult;

	// argument �˻�
	if (2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// ������ ���� ���̺귯�� �ʱ�ȭ
	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	// server socket ����(socket)
	ServerSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == ServerSocket)
		ErrorHandling("UDP socket creation error");
	
	// server address ���� �Է�
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	memset(&ClientAddress, 0, sizeof(ClientAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server socket�� server address ���� �Ҵ�(bind)
	FunctionResult = bind(ServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("bind() error");

	// UDP ���
	ClientAddressSize = sizeof(ClientAddress);
	for (int i = 0; i < CONNECT_COUNT; i++)
	{
		StringLength = recvfrom(ServerSocket, message, BUF_SIZE, 0, (SOCKADDR*)&ClientAddress, &ClientAddressSize);
		if (SOCKET_ERROR == StringLength)
			ErrorHandling("recvfrom() error");

		StringLength = sendto(ServerSocket, message, BUF_SIZE, 0, (SOCKADDR*)&ClientAddress, ClientAddressSize);
		if (SOCKET_ERROR == StringLength)
			ErrorHandling("sendto() error");
	}

	// server socket ����
	closesocket(ServerSocket);

	// ������ ���� ���̺귯�� ����
	WSACleanup();

	return 0;
}