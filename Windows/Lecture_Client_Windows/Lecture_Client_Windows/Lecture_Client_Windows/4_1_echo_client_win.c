#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUF_SIZE 1024

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;						// 윈도우 소켓 라이브러리 정보
	SOCKET HandleClientSocket;				// client socket
	SOCKADDR_IN ServerAddress;				// server address 정보
	int StringLength;						// 수신한 메세지(데이터)의 길이

	char message[BUF_SIZE];					// 수신한 메세지
	int OperationResult;					// 함수 동작 결과

	// argument 검사
	if (3 != argc)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	// 윈도우 소켓 라이브러리 정보 초기화
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() error");

	// client socket 생성(socket)
	HandleClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("socket() error");

	// server address 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server에 연결 요청(connect)
	OperationResult = connect(HandleClientSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("connect() error");
	else
		puts("Connected............");

	while (1)
	{
		memset(message, 0, sizeof(message));
		fputs("Input message(Q to quit) : ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		// server에 메시지(데이터) 전송(send)
		send(HandleClientSocket, message, strlen(message), 0);
		
		// server에서 온 메세지(데이터) 수신(recv)
		StringLength = recv(HandleClientSocket, message, BUF_SIZE - 1, 0);

		message[StringLength] = NULL;
		printf("Message from server : %s\n", message);
	}

	// client socket 해제
	closesocket(HandleClientSocket);

	// 윈도우 소켓 라이브러리 정보 해제
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}