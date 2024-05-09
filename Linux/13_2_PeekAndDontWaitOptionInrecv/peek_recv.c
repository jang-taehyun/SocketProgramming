#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define BUF_SIZE 30

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int AcceptSocket, ReceiveSocket;
	struct sockaddr_in AcceptAddress, ReceiveAddress;
	socklen_t AddressSize;

	char buf[BUF_SIZE];
	int StringLength;
	
	int FunctionResult;
	int state;

	// argument 검사
	if(2 != argc)
	{
		printf("Usage %s <port>\n", argv[0]);
		exit(1);
	}

	// accept socket 생성(socket)
	AcceptSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == AcceptSocket)
		ErrorHandling("socket() error");

	// accept server address 정보 입력
	AddressSize = sizeof(ReceiveAddress);
	memset(&AcceptAddress, 0, sizeof(AcceptAddress));
	AcceptAddress.sin_family = AF_INET;
	AcceptAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	AcceptAddress.sin_port = htons(atoi(argv[1]));

	// accept socket에 accept server address 정보 할당(bind)
	FunctionResult = bind(AcceptSocket, (struct sockaddr*)&AcceptAddress, sizeof(AcceptAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	// accept socket을 연결 요청 대기 상태로 전환(listen)
	FunctionResult = listen(AcceptSocket, 5);
	if(-1 == FunctionResult)
		ErrorHandling("listen() error");

	// client로부터 연결 요청 수락(accept)
	ReceiveSocket = accept(AcceptSocket, (struct sockaddr*)&ReceiveAddress, &AddressSize);
	if(-1 == ReceiveSocket)
		ErrorHandling("accept() error");

	while(1)
	{
		// client로부터 데이터 수신(recv)
		StringLength = recv(ReceiveSocket, buf, sizeof(buf) - 1, MSG_PEEK | MSG_DONTWAIT);
		if(-1 == StringLength)
		{
			puts("recv() error");
			continue;
		}

		if(0 == StringLength)
		{
			puts("Receive EOF from client");
			break;
		}

		if(StringLength > 0)
		{
			puts("recv() success!! option is peek and don't wait");
			
			buf[StringLength] = 0;
			printf("Buffering %d bytes : %s\n", StringLength, buf);

			break;
		}
	}

	StringLength = recv(ReceiveSocket, buf, sizeof(buf) - 1, 0);
	buf[StringLength] = 0;
	printf("Read again : %s\n", buf);

	// accept socket, receive socket 해제(close)
 	close(ReceiveSocket);
	close(AcceptSocket);
	
	return 0;
}
