#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define MESSAGE_BUFFER 30
#define _WINSOCK_DEPRECATED_NO_WARNINGS

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;				// 윈도우 소켓 라이브러리 정보
	SOCKET HandleClientSocket;		// client socket handle
	SOCKADDR_IN ServerAddress;		// server address
	int OperationResult;			// 함수 동작 결과

	char message[MESSAGE_BUFFER];	// 받은 메세지 저장
	int StringLength;				// 메세지의 길이

	// argument 검사
	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// 윈도우 소켓 라이브러리 초기화
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() function error");

	// client socket 생성(socket)
	HandleClientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("socket() function error");

	// server 주소 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server에 연결 요청
	OperationResult = connect(HandleClientSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("connect() function error");

	// 메시지 수신
	StringLength = recv(HandleClientSocket, message, sizeof(message) - 1, 0);
	if (-1 == StringLength)
		ErrorHandling("read() function error");

	// 메시지 출력
	printf("Message from server : %s\n", message);

	// handle 해제
	closesocket(HandleClientSocket);

	// 윈도우 소켓 라이브러리 해제
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}