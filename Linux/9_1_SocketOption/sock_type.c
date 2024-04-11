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
	int TCPSocket, UDPSocket;
	int SocketType;
	socklen_t OptionLen;
	int State;

	OptionLen = sizeof(SocketType);

	// socket 생성(socket) 및 socket의 file descriptor 출력
	TCPSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(!TCPSocket)
		ErrorHandling("TCP socket() error");
	UDPSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(!UDPSocket)
		ErrorHandling("UDP socket() error");
	printf("SOCK_STREAM : %d\n", SOCK_STREAM);
	printf("SOCK_DGRAM : %d\n", SOCK_DGRAM);

	// TCP socket의 socket type 추출 및 출력
	State = getsockopt(TCPSocket, SOL_SOCKET, SO_TYPE, (void*)&SocketType, &OptionLen);
	if(State)
		ErrorHandling("TCP getsockopt() error");
	printf("Socket type one : %d\n", SocketType);

	// UDP socket의 socket type 추출 및 출력
	State = getsockopt(UDPSocket, SOL_SOCKET, SO_TYPE, (void*)&SocketType, &OptionLen);
	if(State)
		ErrorHandling("UDP getsockopt() error");
	printf("Socket type two : %d\n", SocketType);

	// TCP socket, UDP socket 해제
	close(TCPSocket);
	close(UDPSocket);

	return 0;
}
