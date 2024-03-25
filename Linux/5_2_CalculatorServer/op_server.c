#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BACK_LOG_QUEUE_SIZE 5
#define CONNECT_COUNT 3
#define RECEIVE_COUNT 5

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int ServerSocket, ClientSocket;
	struct sockaddr_in ServerAddress, ClientAddress;
	socklen_t ClientAddressSize;
	int OperationResult;

	int ReceiveDataSize = sizeof(int);
	int ReceiveCount=0;

	int Operand[RECEIVE_COUNT];
	int OperandCount;
	char Operator;
	int SendNumber = 0;

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

	// client로부터 연결 요청 수락(accept)
	ClientAddressSize = sizeof(ClientAddress);
	for(int i=0; i<CONNECT_COUNT; i++)
	{
		ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
		if(-1 == ClientSocket)
			ErrorHandling("accept() error");
		else
			puts("Connected........");

		memset(Operand, 0, sizeof(Operand));
		ReceiveCount = 0;
		
		// client에서 온 메시지(데이터) 수신(read)
		ReceiveCount = read(ClientSocket, &OperandCount, ReceiveDataSize);
		if(-1 == ReceiveCount)
			ErrorHandling("read() error");
		else
			printf("Receive at client %d, operand count is %d\n", i+1, OperandCount);

		for(int j=0; j<OperandCount; j++)
		{
			ReceiveCount = read(ClientSocket, &Operand[j], sizeof(int));
			if(-1 == ReceiveCount)
				ErrorHandling("read() at input operand error");
			else
				printf("Receive at client %d, operand %d is %d\n", i+1, j+1, Operand[j]);
		}

		ReceiveCount = read(ClientSocket, &Operator, sizeof(char));
		if(-1 == ReceiveCount)
			ErrorHandling("read() at input operator error");
		else
			printf("Receive at client %d, operator is %c\n", i+1, Operator);

		SendNumber = Operand[0];
		for(int j=1; j<OperandCount; j++)
		{
			if(Operator == '+')
				SendNumber += Operand[j];
			else if(Operator == '-')
				SendNumber -= Operand[j];
			else if(Operator == '*')
				SendNumber *= Operand[j];
			else if(Operator == '/')
				SendNumber /= Operand[j];
		}

		printf("Send at client %d, result is %d\n", i+1, SendNumber);

		// client에 메시지(데이터) 전송(write)
		write(ClientSocket, &SendNumber, sizeof(int));

		// client socket 해제
		close(ClientSocket);
	}

	// server socket 해제
	close(ServerSocket);

	return 0;
}
