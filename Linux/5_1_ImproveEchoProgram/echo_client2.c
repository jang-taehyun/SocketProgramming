#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

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
	
	int StringLength, ReceiveLength, ReceiveCount;
	char message[BUF_SIZE];
	int OperationResult;

	// argument 검사
	if(argc != 3)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
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

	while(1)
	{
		memset(message, 0 ,sizeof(message));
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if(!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
			break;

		// server에 메세지(데이터) 전송(write)
		StringLength = write(ClientSocket, message, strlen(message));

		ReceiveLength = 0;
		while(ReceiveLength < StringLength)
		{
			// server에서 메시지(데이터) 수신(read)
			ReceiveCount = read(ClientSocket, &message[ReceiveLength], BUF_SIZE-1);
			if(-1 == ReceiveCount)
				ErrorHandling("read() error");

			ReceiveLength += ReceiveCount;
		}

		message[ReceiveLength] = 0;
		printf("Message from server: %s\n", message);
	}

	// client socket 해제
	close(ClientSocket);

	return 0;
}
