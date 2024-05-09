#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define BUF_SIZE 30

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET AcceptSocket, ReceiveSocket;
	SOCKADDR_IN AcceptAddress, ReceiveAddress;
	int AddressSize;

	fd_set read, except, readCopy, exceptCopy;
	struct timeval timeout;

	char buf[BUF_SIZE];
	int StringLength, FunctionResult;

	// argument 검사
	if (2 != argc)
	{
		printf("Usage %s <port>\n", argv[0]);
		exit(1);
	}

	// 윈도우 소켓 라이브러리 초기화
	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	// accept socket 생성(socket)
	AcceptSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == AcceptSocket)
		ErrorHandling("socket() error");

	// accept server address 정보 입력
	AddressSize = sizeof(ReceiveAddress);
	memset(&AcceptAddress, 0, sizeof(AcceptAddress));
	AcceptAddress.sin_family = AF_INET;
	AcceptAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	AcceptAddress.sin_port = htons(atoi(argv[1]));

	// accept server address 정보를 accept socket에 할당(bind)
	FunctionResult = bind(AcceptSocket, (SOCKADDR*)&AcceptAddress, sizeof(AcceptAddress));
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("bind() error");

	// accept socket을 연결 요청 대기 상태로 전환(listen)
	FunctionResult = listen(AcceptSocket, 5);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("listen() error");

	// client로부터 연결 요청 수락(accept)
	ReceiveSocket = accept(AcceptSocket, (SOCKADDR*)&ReceiveAddress, &AddressSize);
	if (INVALID_SOCKET == ReceiveSocket)
		ErrorHandling("accept() error");

	// fd_set 변수 초기화 및 receive socket을 fd_set 변수에 등록
	FD_ZERO(&read);
	FD_ZERO(&except);
	FD_SET(ReceiveSocket, &read);
	FD_SET(ReceiveSocket, &except);

	while (1)
	{
		FunctionResult = 0;
		memset(buf, 0, sizeof(buf));

		// fd_set 변수 backup
		readCopy = read;
		exceptCopy = except;

		// time-out 설정
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		// select function 호출
		FunctionResult = select(0, &readCopy, 0, &exceptCopy, &timeout);
		if (SOCKET_ERROR == FunctionResult)
		{
			puts("select() error");
			break;
		}

		// time-out 발생
		if (!FunctionResult)
		{
			puts("time-out");
			continue;
		}

		// event 발생
		if (FunctionResult > 0)
		{
			// urgent message 수신
			if (FD_ISSET(ReceiveSocket, &exceptCopy))
			{
				StringLength = recv(ReceiveSocket, buf, BUF_SIZE - 1, MSG_OOB);
				buf[BUF_SIZE - 1] = 0;
				printf("Urgent message : %s\n", buf);
			}

			// 데이터 수신
			if (FD_ISSET(ReceiveSocket, &readCopy))
			{
				StringLength = recv(ReceiveSocket, buf, BUF_SIZE - 1, 0);

				// EOF 수신
				if (!StringLength)
				{
					puts("Receivie EOF from client");
					closesocket(ReceiveSocket);
					break;
				}
				// 데이터 수신
				else
				{
					buf[StringLength] = 0;
					puts(buf);
				}
			}
		}
	}

	// accept socket 해제(close socket)
	closesocket(AcceptSocket);

	// 윈도우 소켓 라이브러리 해제
	WSACleanup();

	return 0;
}