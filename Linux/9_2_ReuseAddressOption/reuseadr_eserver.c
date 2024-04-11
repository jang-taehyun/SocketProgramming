#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE 1
#define FALSE 0

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int ServerSocket, ClientSocket;
	char message[30];
	int Option, StringLength, state;
	socklen_t OptionLength, ClientAddressSize;
	struct sockaddr_in ServerAddress, ClientAddress;

	if(2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ServerSocket)
		ErrorHandling("socket() error");

	// server socket의 reuse address option을 true로 설정
	OptionLength = sizeof(Option);
	Option = TRUE;
	state = setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, (void*)&Option, OptionLength);
	if(state)
		ErrorHandling("setsockopt() error");

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	state = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(state)
		ErrorHandling("bind() error");

	state = listen(ServerSocket, 5);
	if(-1 == state)
		ErrorHandling("listen() error");

	ClientAddressSize = sizeof(ClientAddress);
	ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &ClientAddressSize);
	if(-1 == ClientSocket)
		ErrorHandling("accept() error");

	while(1)
	{
		StringLength = read(ClientSocket, message, sizeof(message));
		if(-1 == StringLength)
			ErrorHandling("read() error");
		if(!StringLength)
			break;

		state = write(ClientSocket, message, StringLength);
		if(-1 == state)
			ErrorHandling("write() client socket error");
		state = write(1, message, StringLength);
		if(-1 == state)
			ErrorHandling("write() 1 error");
	}

	close(ClientSocket);
	close(ServerSocket);

	return 0;
}
