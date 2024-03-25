#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
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
	int ClientSocket;
	char OperationMessage[BUF_SIZE];
	int Result, OperandCount, OperationResult;
	struct sockaddr_in ServerAddress;

	// argument 검사
	if(argc != 3)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	// client socket 생성
	ClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ClientSocket)
		ErrorHandling("socket() error");

	// server address 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server에 연결 요청(connect)
	OperationResult = connect(ClientSocket,(struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == OperationResult)
		ErrorHandling("connect() error");
	else
		puts("Connected........");

	memset(OperationMessage, 0, sizeof(OperationMessage));

	// operand의 개수 입력
	fputs("Operand count : ", stdout);
	scanf("%d", &OperandCount);
	OperationMessage[0] = (char)OperandCount;

	// operand 입력
	for(int i=0; i<OperandCount; i++)
	{
		printf("Operand %d : ", i+1);
		scanf("%d", (int*)&OperationMessage[i*OPERAND_SIZE+1]);
	}
	fgetc(stdin);

	// operator 입력
	fputs("Operator : ", stdout);
	scanf("%c", &OperationMessage[OperandCount * OPERAND_SIZE + 1]);

	// server에 메시지(데이터) 전송(write)
	OperationResult = write(ClientSocket, OperationMessage, OperandCount*OPERAND_SIZE+2);
	if(-1 == OperationResult)
		ErrorHandling("write() error");

	// server에서 메시지(데이터) 수신(read)
	OperationResult = read(ClientSocket, &Result, RESULT_SIZE);
	if(-1 == OperationResult)
		ErrorHandling("read() error");

	printf("Operation result : %d\n", Result);

	// client socket 해제
	close(ClientSocket);

	return 0;
}
