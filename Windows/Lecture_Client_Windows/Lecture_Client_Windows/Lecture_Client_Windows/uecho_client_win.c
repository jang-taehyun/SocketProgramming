#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	SOCKET ClientSocket;
	SOCKADDR_IN ServerAddress;
	int StringLength;

	char message[BUF_SIZE];
	int FunctionResult;

	// argument 검사
	if (3 != argc)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// 윈도우 소켓 라이브러리 초기화
	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	// client socket 생성(socket)
	ClientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == ClientSocket)
		ErrorHandling("socket() error");

	// server address 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// client socket에 server address 정보 등록(connect)
	FunctionResult = connect(ClientSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("connect() error");

	// UDP 통신
	while (1)
	{
		fputs("Insert message(q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		StringLength = send(ClientSocket, message, strlen(message), 0);
		if (SOCKET_ERROR == StringLength)
			ErrorHandling("send() error");

		StringLength = recv(ClientSocket, message, sizeof(message) - 1, 0);
		if (SOCKET_ERROR == StringLength)
			ErrorHandling("recv() error");

		message[StringLength] = 0;
		printf("Message from server : %s\n", message);
	}

	// client socket 해제
	closesocket(ClientSocket);

	// 윈도우 소켓 라이브러리 해제
	WSACleanup();

	return 0;
}