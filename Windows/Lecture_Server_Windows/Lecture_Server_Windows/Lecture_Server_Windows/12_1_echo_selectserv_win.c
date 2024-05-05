#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
	SOCKET ServerSocket, ClientSocket;
	SOCKADDR_IN ServerAddress, ClientAddress;
	int AddressSize;

	fd_set reads, cpyreads;
	TIMEVAL timeout;
	int SelectResult;

	int StringLength;
	char buf[BUF_SIZE];

	int FunctionResult;

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

	// server socket 생성(socket)
	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == ServerSocket)
		ErrorHandling("socket() error");

	// server address 정보 입력
	AddressSize = sizeof(ServerAddress);
	memset(&ServerAddress, 0, AddressSize);
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server socket에 server address 정보 할당(bind)
	FunctionResult = bind(ServerSocket, (SOCKADDR*)&ServerAddress, AddressSize);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("bind() error");

	// server socket을 연결 요청 대기 상태로 전환(listen)
	FunctionResult = listen(ServerSocket, 5);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("listen() error");

	// fd_set 변수 초기화 및 server socket을 fd_set 변수에 등록
	FD_ZERO(&reads);
	FD_SET(ServerSocket, &reads);

	while (1)
	{
		// fd_set 변수 backup
		cpyreads = reads;

		// timeout 설정
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		// 검사 범위 설정 및 select function 호출
		SelectResult = select(0, &cpyreads, 0, 0, &timeout);
		if (SOCKET_ERROR == SelectResult)
		{
			puts("select() error");
			break;
		}

		// time-out
		if (!SelectResult)
		{
			puts("Time-out");
			continue;
		}

		// event가 발생한 경우
		// event가 발생한 handle 찾기
		for (int i = 0; i < reads.fd_count; i++)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyreads))
			{
				// server socket에 데이터 수신 여부 event가 발생
				if (reads.fd_array[i] == ServerSocket)
				{
					// client로부터 연결 요청 수락(accept)
					ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddress, &AddressSize);
					if (INVALID_SOCKET == ClientSocket)
						ErrorHandling("accept() error");

					// client socket handle을 fd_set 변수에 등록
					FD_SET(ClientSocket, &reads);

					printf("connected client: %d\n", ClientSocket);
				}
				// client socket에 데이터 수신 여부 event가 발생
				else
				{
					// client로부터 데이터 수신(recv)
					StringLength = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);

					// EOF
					if (!StringLength)
					{
						// fd_set 변수에서 client socket 제외
						FD_CLR(reads.fd_array[i], &reads);

						// client socket 해제
						closesocket(reads.fd_array[i]);

						printf("closed client: %d\n", reads.fd_array[i]);
					}
					// client에 data 전송(send)
					else
					{
						send(reads.fd_array[i], buf, StringLength, 0);
					}
				}
			}
		}
	}

	// server socket 해제
	closesocket(ServerSocket);

	// 윈도우 소켓 라이브러리 해제
	WSACleanup();

	return 0;
}