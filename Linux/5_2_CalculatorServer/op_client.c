#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define RECEIVE_COUNT 5

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int ClientSocket;
	struct sockaddr_in ServerAddress;
	int OperationResult;

	int OperandCount;
	int Operand;
	int ReadCount=0;
	int Result = 0;
	char Operator;

	// argument 검사
	if(3 != argc)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// client socket 생성(socket)
	ClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ClientSocket)
		ErrorHandling("socket() error");

	// server address 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server에 연결 요청(connect)
	OperationResult = connect(ClientSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == OperationResult)
		ErrorHandling("connect() error");
	else
		puts("Connected............");

	// Server에 데이터 전송(write)
	printf("Operand Count(max Operand count is %d) : ", RECEIVE_COUNT);
	scanf("%d", &OperandCount);

	OperationResult = write(ClientSocket, &OperandCount, sizeof(int));
	if(-1 == OperationResult)
		ErrorHandling("write() 1 error");

	for(int i=0; i<OperandCount; i++)
	{
		printf("Operand %d : ", i+1);
		scanf("%d", &Operand);
		OperationResult = write(ClientSocket, &Operand, sizeof(int));
		if(-1 == OperationResult)
		{
			ErrorHandling("write() 2 error");
		}

	}

	fputs("Operator : ", stdout);
	scanf(" %c", &Operator);
	OperationResult = write(ClientSocket, &Operator, sizeof(char));
	if(-1 == OperationResult)
		ErrorHandling("write() 3 error");

	// server에서 온 데이터 수신(read)
	while(ReadCount < sizeof(int))
	{
		OperationResult = read(ClientSocket, &Result, sizeof(int));
		if(-1 == OperationResult)
			ErrorHandling("read() error");

		ReadCount += OperationResult;
	}

	printf("Operation result : %d\n", Result);

	// client socket 해제
	close(ClientSocket);

	return 0;
}
