#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define BACK_LOG_QUEUE_SIZE 5
#define CONNECT_COUNT 5

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

	char message[BUF_SIZE];
	int StringLength;
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

	// server socket을 연결 요청 대기 상태로 전환(listen)
	OperationResult = listen(ServerSocket, BACK_LOG_QUEUE_SIZE);
	if(-1 == OperationResult)
		ErrorHandling("listen() error");

	// client로부터 연결 요청 수락(accept)
	ClientAddressSize = sizeof(ClientAddress);
	for(int i=1; i<=CONNECT_COUNT; i++)
	{
		ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
		if(-1 == ClientSocket)
			ErrorHandling("accept() error");
		else
			printf("Connected client %d\n", i);

		// client로부터 수신된 메시지(데이터) 수신(read)
		while(StringLength = read(ClientSocket, message, BUF_SIZE))
		{
			// 수신한 메시지(데이터)를 client에게 전송(write)
			write(ClientSocket, message, StringLength);
		}

		// client socket 해제
		close(ClientSocket);
	}

	// server socket 해제
	close(ServerSocket);

	return 0;
}
