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
	int ClientSocket;
	char message[BUF_SIZE];
	int StringLength;
	struct sockaddr_in ServerAddress, FromAddress;
	socklen_t FromAddressSize;
	int FunctionResult;

	// argument 검사
	if(argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	// client socket 생성(socket)
	ClientSocket = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(-1 == ClientSocket)
		ErrorHandling("socket() error");

	// server address 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// UDP 통신
	FromAddressSize = sizeof(FromAddress);
	memset(&FromAddress, 0, FromAddressSize);
	while(1)
	{
		fputs("Insert message(q to quit) : ", stdout);
		fgets(message, sizeof(message), stdin);
		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		StringLength = sendto(ClientSocket, message, strlen(message), 0, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
		if(-1 == StringLength)
			ErrorHandling("sendto() error");

		StringLength = recvfrom(ClientSocket, message, BUF_SIZE, 0, (struct sockaddr*)&FromAddress, &FromAddressSize);
		if(-1 == StringLength)
			ErrorHandling("recvfrom() error");

		message[StringLength-1] = 0;
		printf("Message from server : %s\n", message);
	}

	// client socket 해제
	close(ClientSocket);

	return 0;
}
