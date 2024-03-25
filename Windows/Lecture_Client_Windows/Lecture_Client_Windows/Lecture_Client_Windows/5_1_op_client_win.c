#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define RESULT_SIZE 4
#define OPERAND_SIZE 4

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;						// 윈도우 소켓 라이브러리 정보
	SOCKET HandleClientSocket;				// client socket
	SOCKADDR_IN ServerAddress;				// server address 정보
	int OperationResult;					// 함수 동작 결과

	char OperationMessage[BUF_SIZE];		// 수신한 메세지
	int OperandCount;						// 피연산자 개수
	int result;								// 연산 결과

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

	// 피연산자, 연산자 입력
	fputs("Operand count: ", stdout);
	scanf("%d", &OperandCount);
	OperationMessage[0] = (char)OperandCount;

	for (int i = 0; i < OperandCount; i++)
	{
		printf("Operand %d : ", i + 1);
		scanf("%d", (int*)&OperationMessage[i * OPERAND_SIZE + 1]);
	}
	fgetc(stdin);

	fputs("Operator : ", stdout);
	scanf("%c", &OperationMessage[OperandCount * OPERAND_SIZE + 1]);

	// 입력된 정보를 server에 전송(write)
	OperationResult = send(HandleClientSocket, OperationMessage, OperandCount * OPERAND_SIZE + 2, 0);
	
	// server에서 온 메시지(데이터) 수신(read)
	recv(HandleClientSocket, &result, RESULT_SIZE, 0);

	printf("Operation result: %d\n", result);

	// client socket 해제
	closesocket(HandleClientSocket);

	// 윈도우 소켓 라이브러리 정보 해제
	WSACleanup();

	return 0;
}