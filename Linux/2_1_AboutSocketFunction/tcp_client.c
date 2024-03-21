#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define MESSAGE_MAX_LENGTH 30

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int ClientSocket;				// client socket
	struct sockaddr_in ServerAddress;		// server 주소 정보
	char ReceiveMessage[MESSAGE_MAX_LENGTH];	// server에서 받은 메세지(데이터)
	int StringLength=0;				// 문자열의 길이
	int Index=0, ReadLength;			// array의 index, 전송받은 메시지(데이터)의 길이
	int OperationResult;				// 함수 동작 결과

	// argument 검사
	if(3 != argc)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	// socket 생성(socket)
	ClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ClientSocket)
		ErrorHandling("socket() function error");

	// server 주소 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server에게 연결 요청(connect)
	OperationResult = connect(ClientSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == OperationResult)
		ErrorHandling("connect() function error");

	// 메세지(데이터) 수신
	while(ReadLength = read(ClientSocket, &ReceiveMessage[Index++], 1))	// 1byte씩 수신
	{
		if(-1 == ReadLength)
			ErrorHandling("read() function error");

		StringLength += ReadLength;
	}

	printf("Message from server : %s\n", ReceiveMessage);
	printf("Function read() call count : %d\n", StringLength);

	// socket 해제
	close(ClientSocket);

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
