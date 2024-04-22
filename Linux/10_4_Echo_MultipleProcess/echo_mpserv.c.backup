#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

void ErrorHandling(const char* message);
void ReadChildProcess(int signal);

int main(int argc, char* argv[])
{
	int ServerSocket, ClientSocket;
	struct sockaddr_in ServerAddress, ClientAddress;
	pid_t pid;
	struct sigaction act;
	socklen_t ClientAddressSize;
	int state;

	int StringLength, FunctionResult;
	char buf[BUF_SIZE];

	// argument 검사
	if(2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// signal handling 등록
	act.sa_handler = ReadChildProcess;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	state = sigaction(SIGCHLD, &act, 0);
	if(-1 == state)
		ErrorHandling("sigaction() error");

	// server socket 생성(socket)
	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ServerSocket)
		ErrorHandling("socket() error");

	// server address 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server address를 server socket에 할당(bind)
	FunctionResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	// server socket을 연결요청 대기 상태로 전환(listen)
	FunctionResult = listen(ServerSocket, 5);
	if(-1 == FunctionResult)
		ErrorHandling("listen() error");

	ClientAddressSize = sizeof(ClientAddress);
	while(1)
	{
		// server socket에서 client 연결 요청 수락(accept)
		ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
		if(-1 == ClientSocket)
			continue;

		puts("New Client connected...");

		// child process 생성
		pid = fork();
		if(-1 == pid)
		{
			close(ClientSocket);
			continue;
		}

		// child process 실행
		if(0 == pid)
		{
			// server socket 해제
			close(ServerSocket);

			while(1)
			{
				// client socket에서 받아온 data를 buffer에 저장(read)
				StringLength = read(ClientSocket, buf, BUF_SIZE);
				if(-1 == StringLength)
					ErrorHandling("read() error");
				if(0 == StringLength)
					break;

				// buffer에 저장된 data를 client socket을 통해 전송(write)
				FunctionResult = write(ClientSocket, buf, StringLength);
				if(-1 == FunctionResult)
					ErrorHandling("write() error");
			}

			// client socket 해제
			close(ClientSocket);

			puts("Client disconnected...");
			return 0;
		}
		// parent process 실행
		else
		{
			// client socket 해제
			close(ClientSocket);
		}
	}

	// server socket 해제
	close(ServerSocket);

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void ReadChildProcess(int signal)
{
	pid_t pid;
	int status;

	pid = waitpid(-1, &status, WNOHANG);
	if(-1 == pid)
		ErrorHandling("waitpid() error");
	printf("Remove Process ID : %d\n", pid);
}
