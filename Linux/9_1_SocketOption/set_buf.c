#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main()
{
	int Socket;
	int SendBufferSize = 1024*10, ReceiveBufferSize = 1024*10;
	int state;
	socklen_t OptionLength;

	// socket 생성
	Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(!Socket)
		ErrorHandling("socket() error");

	// socket의 입출력 buffer의 크기 변경
	state = setsockopt(Socket, SOL_SOCKET, SO_SNDBUF, (void*)&SendBufferSize, sizeof(SendBufferSize));
	if(state)
		ErrorHandling("input buffer setsockopt() error");
	state = setsockopt(Socket, SOL_SOCKET, SO_RCVBUF, (void*)&ReceiveBufferSize, sizeof(ReceiveBufferSize));
	if(state)
		ErrorHandling("output buffer setsockopt() error");

	// socket의 입출력 buffer의 크기 추출 및 출력
	OptionLength = sizeof(SendBufferSize);
	state = getsockopt(Socket, SOL_SOCKET, SO_SNDBUF, (void*)&SendBufferSize, &OptionLength);
	if(state)
		ErrorHandling("input buffer getsockopt() error");
	OptionLength = sizeof(ReceiveBufferSize);
	state = getsockopt(Socket, SOL_SOCKET, SO_RCVBUF, (void*)&ReceiveBufferSize, &OptionLength);
	if(state)
		ErrorHandling("output buffer getsockopt() error");

	printf("Input Buffer size : %d\n", SendBufferSize);
	printf("Output Buffer size : %d\n", ReceiveBufferSize);

	// socket 해제
	close(Socket);

	return 0;
}
