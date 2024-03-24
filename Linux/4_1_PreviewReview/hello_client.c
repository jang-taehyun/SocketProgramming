#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

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
	char message[30];
	int StringLength;
	int OperationResult;

	// argument 검사
	if(3 != argc)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	// client socket 생성(socket)
	ClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
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

	// 수신된 데이터(메세지)를 받는다(read)
	StringLength = read(ClientSocket, message, sizeof(message)-1);
	if(-1 == StringLength)
		ErrorHandling("read() error");

	printf("Message from server : %s\n", message);

	// client socket 해제
	close(ClientSocket);

	return 0;
}
