#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUF_SIZE 1024
#define OPERAND_SIZE 4
#define BACK_LOG_QUEUE_SIZE 5
#define CONNECT_COUNT 3

void ErrorHandling(const char* message);
int Calculate(int OperandCount, int Operand[], char Operator);

int main(int argc, char* argv[])
{
	WSADATA wsaData;											// 윈도우 소켓 라이브러리 정보
	SOCKET HandleServerSocket, HandleClientSocket;				// server socket, client socket
	SOCKADDR_IN ServerAddress, ClientAddress;					// server address, client address
	int ClientAddressSize;										// client address의 크기
	int OperationResult;										// 함수 동작 결과

	int ReceiveCount;											// 한 번에 수신한 데이터의 크기
	int ReceiveLength;											// 현재까지 받은 데이터의 크기

	char OperationInfo[BUF_SIZE];								// 수신한 메시지
	int result;													// 연산 결과
	int OperandCount;											// 피연산자의 개수


	// argument 검사
	if (2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// 윈도우 소켓 라이브러리 정보 초기화
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() error");

	// server socket 생성(socket)
	HandleServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleServerSocket)
		ErrorHandling("socket() error");

	// server address 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server socket에 IP, port 할당
	OperationResult = bind(HandleServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("bind() error");

	// server socket을 연결 대기 상태로 전환(listen)
	OperationResult = listen(HandleServerSocket, BACK_LOG_QUEUE_SIZE);
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("listen() error");

	ClientAddressSize = sizeof(ClientAddress);
	memset(OperationInfo, 0, sizeof(OperationInfo));
	for (int i = 0; i < CONNECT_COUNT; i++)
	{
		OperandCount = 0;

		// client의 연결 요청 수락(accept)
		HandleClientSocket = accept(HandleServerSocket, (SOCKADDR*)&ClientAddress, &ClientAddressSize);
		if (-1 == HandleClientSocket)
			ErrorHandling("accept() error");
		else
			printf("Connected Client %d\n", i + 1);

		// client가 보낸 데이터를 수신(read)
		recv(HandleClientSocket, &OperandCount, 1, 0);

		ReceiveLength = 0;
		while ((OperandCount * OPERAND_SIZE + 1) > ReceiveLength)
		{
			ReceiveCount = recv(HandleClientSocket, &OperationInfo[ReceiveLength], BUF_SIZE - 1, 0);
			ReceiveLength += ReceiveCount;
		}

		// 연산
		result = Calculate(OperandCount, (int*)OperationInfo, OperationInfo[ReceiveLength - 1]);

		// client에 데이터를 전송(write)
		send(HandleClientSocket, (char*)&result, sizeof(result), 0);

		// client socket 해제
		closesocket(HandleClientSocket);
	}

	// server socket 해제
	closesocket(HandleServerSocket);

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

int Calculate(int OperandCount, int Operand[], char Operator)
{
	int result = Operand[0];

	switch (Operator)
	{
	case '+':
	{
		for (int i = 1; i < OperandCount; i++)
			result += Operand[i];
	}
		break;
	case '-':
	{
		for (int i = 1; i < OperandCount; i++)
			result -= Operand[i];
	}
		break;
	case '*':
	{
		for (int i = 1; i < OperandCount; i++)
			result *= Operand[i];
	}
		break;
	}

	return result;
}