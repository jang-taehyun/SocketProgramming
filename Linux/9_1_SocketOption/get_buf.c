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
	int SendBufferSize, ReceiveBufferSize, state;
	socklen_t OptionLength;

	// socket 생성
	Socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(!Socket)
		ErrorHandling("socket() error");

	// socket의 출력 buffer의 크기를 얻기
	OptionLength = sizeof(SendBufferSize);
	state = getsockopt(Socket, SOL_SOCKET, SO_SNDBUF, (void*)&SendBufferSize, &OptionLength);
	if(state)
		ErrorHandling("send buffer getsockopt() error");
	
	// socket의 입력 buffer의 크기를 얻기
	OptionLength = sizeof(ReceiveBufferSize);
	state = getsockopt(Socket, SOL_SOCKET, SO_RCVBUF, (void*)&ReceiveBufferSize, &OptionLength);
	if(state)
		ErrorHandling("receive buffer getsockopt() error");

	// 받아온 입출력 buffer의 크기 출력
	printf("Input buffer size : %d\n", SendBufferSize);
	printf("Output buffer size : %d\n", ReceiveBufferSize);

	// socket 해제
	close(Socket);

	return 0;
}
