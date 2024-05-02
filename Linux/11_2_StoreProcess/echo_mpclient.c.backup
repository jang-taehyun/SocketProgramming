#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void ErrorHandling(const char* message);
void ReadRoutine(int Socket, char* buffer);
void WriteRoutine(int Socket, char* buffer);

int main(int argc, char* argv[])
{
	int ClientSocket;
	pid_t pid;
	struct sockaddr_in ServerAddress;

	int FunctionResult;
	char buf[BUF_SIZE];

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

	// server address 설정
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server에 연결 요청(connect)
	FunctionResult = connect(ClientSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("connect() error");

	// child process 생성
	pid = fork();
	if(-1 == pid)
		ErrorHandling("fork() error");

	// child process 실행
	if(0 == pid)
		WriteRoutine(ClientSocket, buf);
	// parent process 실행
	else
		ReadRoutine(ClientSocket, buf);

	// client socket 해제
	close(ClientSocket);

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void ReadRoutine(int Socket, char* buffer)
{
	int StringLength;
	while(1)
	{
		// socket으로부터 data를 수신(read)
		StringLength = read(Socket, buffer, BUF_SIZE);
		if(-1 == StringLength)
			ErrorHandling("read() error");
		if(0 == StringLength)
			return;

		buffer[StringLength] = 0;
		printf("Message from server : %s\n", buffer);
	}
}

void WriteRoutine(int Socket, char* buffer)
{
	int FunctionResult;
	while(1)
	{
		fputs("Input message(Q to quit) : ", stdout);
		fgets(buffer, BUF_SIZE, stdin);
		if(!strcmp(buffer, "q\n") || !strcmp(buffer, "Q\n"))
		{
			// server와의 연결 종료(shutdown)
			shutdown(Socket, SHUT_WR);
			return;
		}

		// socket을 통해 data를 전송(write)
		FunctionResult = write(Socket, buffer, strlen(buffer));
		if(-1 == FunctionResult)
			ErrorHandling("write() error");
	}
}
