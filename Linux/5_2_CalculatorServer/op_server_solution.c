#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define OPERAND_SIZE 4
#define BACK_LOG_QUEUE_SIZE 5
#define CONNECT_COUNT 3

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int Calculate(int OperandCount, int Operand[], char Operator)
{
	int result = Operand[0];

	switch(Operator)
	{
		case '+':
			{
				for(int i=1; i<OperandCount; i++)
					result += Operand[i];
			}
			break;
		case '-':
			{
				for(int i=1; i<OperandCount; i++)
					result -= Operand[i];
			}
			break;
		case '*':
			{
				for(int i=1; i<OperandCount; i++)
					result *= Operand[i];
			}
			break;
	}

	return result;
}

int main(int argc, char* argv[])
{
	int ServerSocket, ClientSocket;
	char OperationInfo[BUF_SIZE];
	int Result, OperandCount;
	int ReceiveCount, ReceiveLength;
	struct sockaddr_in ServerAddress, ClientAddress;
	socklen_t ClientAddressSize;
	int OperationResult;

	// argument 검사
	if(2 != argc)
	{
		printf("Usage : %s <Port>\n", argv[0]);
		exit(1);
	}

	// server socket 생성(socket)
	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ServerSocket)
		ErrorHandling("socket() error");

	// server address 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server socket에 IP, port 할당(bind)
	OperationResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == OperationResult)
		ErrorHandling("bind() error");

	// server socket을 연결 대기 상태로 전환(listen)
	OperationResult = listen(ServerSocket, BACK_LOG_QUEUE_SIZE);
	if(-1 == OperationResult)
		ErrorHandling("listen() error");

	ClientAddressSize = sizeof(ClientAddress);

	for(int i=0; i<CONNECT_COUNT; i++)
	{
		OperandCount = 0;

		// client의 연결 요청 수락(accept)
		ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
		if(-1 == ClientSocket)
			ErrorHandling("accept() error");

		// client에서 온 메시지(데이터) 수신(read)
		OperationResult = read(ClientSocket, &OperandCount, 1);
		if(-1 == OperationResult)
			ErrorHandling("read() error");

		ReceiveLength = 0;
		while((OperandCount * OPERAND_SIZE+1) > ReceiveLength)
		{
			ReceiveCount = read(ClientSocket, &OperationInfo[ReceiveLength], BUF_SIZE-1);
			if(-1 == ReceiveCount)
				ErrorHandling("read() error");

			ReceiveLength += ReceiveCount;
		}

		// 연산
		Result = Calculate(OperandCount, (int*)OperationInfo, OperationInfo[ReceiveLength-1]);

		// 연산 결과를 client에 전송(write)
		OperationResult = write(ClientSocket, (char*)&Result, sizeof(Result));
		if(-1 == OperationResult)
			ErrorHandling("write() error");

		// client socket 해제
		close(ClientSocket);
	}

	// server socket 해제
	close(ServerSocket);

	return 0;
}
