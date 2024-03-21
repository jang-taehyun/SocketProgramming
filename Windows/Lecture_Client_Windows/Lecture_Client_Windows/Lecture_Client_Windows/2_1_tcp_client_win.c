#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define MESSAGE_MAX_LENGTH 30

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;							// 윈도우 소켓 라이브러리 정보
	SOCKET HandleClientSocket;					// client socket
	SOCKADDR_IN ServerAddress;					// server 주소 정보
	int OperationResult;						// 함수 동작 결과

	char ReceiveMessage[MESSAGE_MAX_LENGTH];	// 서버에서 받은 메세지(데이터)
	int StringLength = 0;						// 서버에서 받은 메세지(데이터)의 길이
	int Index = 0, ReadLength;					// 메세지(데이터)의 index, 읽은 데이터의 길이

	// argument 검사
	if (3 != argc)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	// 윈도우 소켓 라이브러리 정보 초기화
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() function error");

	// client socket 생성(socket)
	HandleClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("socket() function error");

	// server 주소 정보 초기화
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server에 연결 요청(connect)
	OperationResult = connect(HandleClientSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("connect() function error");

	// 데이터 수신
	while (ReadLength = recv(HandleClientSocket, &ReceiveMessage[Index++], 1, 0))		// 1byte씩 수신
	{
		if (-1 == ReadLength)
			ErrorHandling("recv() function error");

		StringLength += ReadLength;
	}

	printf("message from server : %s\n", ReceiveMessage);
	printf("Function recv() call count : %d\n", StringLength);

	// socket 해제
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