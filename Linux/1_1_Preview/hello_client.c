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
	int ClientSocket;			// client 소켓
	struct sockaddr_in ServerAddress;	// server의 주소
	char message[MESSAGE_MAX_LENGTH];	// server에서 받은 메세지
	int StringLength;			// 받은 메세지의 길이
	int OperationResult;			// 연산 결과

	// parameter의 개수가 부족하다면 프로그램 종료
	if(3 != argc)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	// client socket 생성
	ClientSocket = socket(PF_INET, SOCK_STREAM, 0);
	if(-1 == ClientSocket)
	{
		ErrorHandling("socket() function error");
	}

	// server address 설정
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// client socket을 이용해 연결 요청
	OperationResult = connect(ClientSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == OperationResult)
	{
		ErrorHandling("connect() function error");
	}

	// 서버에서 받아온 메시지를 저장
	StringLength = read(ClientSocket, message, sizeof(message)-1);
	if(-1 == StringLength)
	{
		ErrorHandling("read() function error");
	}

	// 서버에서 받아온 메시지 출력
	printf("Message from Server : %s\n", message);

	// socket 제거
	close(ClientSocket);

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
