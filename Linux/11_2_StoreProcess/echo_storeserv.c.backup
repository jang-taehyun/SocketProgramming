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
	socklen_t AddressSize;
	
	struct sigaction act;
	int status;

	pid_t pid1, pid2;

	int fds[2];

	char buf[BUF_SIZE];
	int StringLength;
	int FunctionResult;

	// argument 검사
	if(2 != argc)
	{
		printf("Usage <port> : %s\n", argv[0]);
		exit(1);
	}

	// signal handling 등록
	act.sa_handler = ReadChildProcess;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	status = sigaction(SIGCHLD, &act, 0);
	if(-1 == status)
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

	// server socket을연결요청 대기 상태로 전환(listen)
	FunctionResult = listen(ServerSocket, 5);
	if(-1 == FunctionResult)
		ErrorHandling("listen() error");

	// pipe, child process 생성
	FunctionResult = pipe(fds);
	if(-1 == FunctionResult)
		ErrorHandling("pipe() error");
	pid1 = fork();
	if(-1 == pid1)
		ErrorHandling("fork() error");

	// parent process execute
	if(0 == pid1)
	{
		puts("parent process execute start");
		FILE* fp = fopen("echomsg.txt", "wt");
		if(0 == fp)
			ErrorHandling("fopen() error");

		char msgbuf[BUF_SIZE];
		int comp, len;

		for(int i=0; i<10; i++)
		{
			len = read(fds[0], msgbuf, BUF_SIZE);
			if(-1 == FunctionResult)
				ErrorHandling("read() error");

			comp = fwrite((void*)msgbuf, 1, len, fp);
			if(comp != len)
				ErrorHandling("fwrite() error");
		}

		fclose(fp);
		puts("parent process execute end");
		return 0;
	}

	// child process 1 execute
	AddressSize = sizeof(ClientAddress);
	while(1)
	{
		// client로부터 연결 요청 수락(accept)
		ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientSocket, &AddressSize);
		if(-1 == ClientSocket)
		{
			puts("accept() error");
			continue;
		}
		else
			puts("New Client connected......");

		// child process 2 생성
		pid2 = fork();
		if(-1 == pid2)
			ErrorHandling("fork() in child error");

		// child process 1 execute
		if(0 == pid2)
		{
			puts("Close server socket in child process 1");
			close(ServerSocket);
			while(1)
			{
				StringLength = read(ClientSocket, buf, BUF_SIZE);
				if(!StringLength)
					break;
				if(-1 == StringLength)
					ErrorHandling("read() error");

				FunctionResult = write(ClientSocket, buf, StringLength);
				if(-1 == FunctionResult)
					ErrorHandling("write() 1 error");
				FunctionResult = write(fds[1], buf, StringLength);
				if(-1 == FunctionResult)
					ErrorHandling("write() 2 error");
			}

			close(ClientSocket);
			puts("Client disconnected......");
			return 0;
		}
		// child process 2 execute
		else
			close(ClientSocket);
	}

	// server socket 해제(close)
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
