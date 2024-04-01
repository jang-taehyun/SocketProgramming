#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int Socket;
	char message[BUF_SIZE];
	struct sockaddr_in ServerAddress;
	int StringLength;
	int FunctionResult;

	if(3 != argc)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	Socket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(-1 == Socket)
		ErrorHandling("socket() error");

	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// UDP socket에 address 정보 할당(connect) -> connected UDP
	FunctionResult = connect(Socket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("connect() error");

	// UDP 통신
	while(1)
	{
		memset(message, 0, sizeof(message));

		fputs("Insert message(q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if(!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
			break;

		StringLength = write(Socket, message, strlen(message));
		if(-1 == StringLength)
			ErrorHandling("write() error");

		StringLength = read(Socket, message, sizeof(message)-1);
		if(-1 == StringLength)
			ErrorHandling("read() error");

		message[StringLength] = 0;
		printf("Message from server : %s\n", message);
	}

	close(Socket);

	return 0;
}
