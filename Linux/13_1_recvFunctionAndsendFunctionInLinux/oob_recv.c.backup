#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <netinet/in.h>
#include <fcntl.h>
#define BUF_SIZE 30

void ErrorHandling(const char* message);
void UrgentHandler(int signal);

int ReceiveSocket;
int AcceptSocket;

int main(int argc, char* argv[])
{
	struct sockaddr_in ReceiveAddress, SendAddress;
	socklen_t AddressSize;

	struct sigaction act;
	int state;

	char buf[BUF_SIZE];
	int StringLength;
	int FunctionResult;

	// argument 검사
	if(2 != argc)
	{
		printf("Usage %s <port>\n", argv[0]);
		exit(1);
	}

	// signal handling 정보 등록
	act.sa_handler = UrgentHandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	// accept socket 생성(socket)
	AcceptSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == AcceptSocket)
		ErrorHandling("socket() error");

	// receive address 정보 입력
	AddressSize = sizeof(ReceiveAddress);
	memset(&ReceiveAddress, 0 ,sizeof(ReceiveAddress));
	ReceiveAddress.sin_family = AF_INET;
	ReceiveAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ReceiveAddress.sin_port = htons(atoi(argv[1]));

	// accept socket에 address 정보 할당(bind)
	FunctionResult = bind(AcceptSocket, (struct sockaddr*)&ReceiveAddress, sizeof(ReceiveAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	// accept socket을 연결 요청 대기 상태로 전환(listen)
	FunctionResult = listen(AcceptSocket, 5);
	if(-1 == FunctionResult)
		ErrorHandling("listen() error");

	// sender로부터 연결 요청 수락(accept)
	ReceiveSocket = accept(AcceptSocket, (struct sockaddr*)&SendAddress, &AddressSize);
	if(-1 == FunctionResult)
		ErrorHandling("accept() error");

	// signal handling을 처리할 process 지정
	fcntl(ReceiveSocket, F_SETOWN, getpid());
	
	// signal handling 등록
	state = sigaction(SIGURG, &act, 0);
	if(-1 == state)
		ErrorHandling("sigaction() error");

	while(1)
	{
		// sender로부터 데이터 수신(recv)
		StringLength = recv(ReceiveSocket, buf, sizeof(buf), 0);
		if(-1 == StringLength)
		{
			puts("recv() error in loop");
			continue;
		}

		if(0 == StringLength)
		{
			puts("Receive EOF to sender");
			break;
		}

		buf[StringLength] = 0;
		puts(buf);
	}

	// accept socket, receive socket 해제(close)
	close(ReceiveSocket);
	close(AcceptSocket);

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void UrgentHandler(int signal)
{
	int StringLength;
	char buf[BUF_SIZE];

	// 긴급 메세지 수신(recv)
	StringLength = recv(ReceiveSocket, buf, sizeof(buf) - 1, MSG_OOB);
	if(-1 == StringLength)
		ErrorHandling("recv() error in UrgentHandler");

	buf[StringLength] = 0;
	printf("Urgent message : %s\n", buf);
}
