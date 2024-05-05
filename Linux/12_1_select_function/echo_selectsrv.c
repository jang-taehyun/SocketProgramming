#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int ServerSocket, ClientSocket;
	struct sockaddr_in ServerAddress, ClientAddress;
	socklen_t AddressSize;

	fd_set reads, cpy_reads;
	struct timeval timeout;

	int FileDescriptorMax, SelectResult;

	int StringLength;
	char buf[BUF_SIZE];

	int FunctionResult;

	// argument 검사
	if(2 != argc)
	{
		printf("Usage %s <port>\n", argv[0]);
		exit(1);
	}

	// server socket 생성(socket)
	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ServerSocket)
		ErrorHandling("socket() error");

	// server address 정보 입력
	AddressSize = sizeof(ServerAddress);
	memset(&ServerAddress, 0, AddressSize);
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server socket에 server address 정보 할당(bind)
	FunctionResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, AddressSize);
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	// server socket을 연결 요청 대기 상태로 전환(listen)
	FunctionResult = listen(ServerSocket, 5);
	if(-1 == FunctionResult)
		ErrorHandling("listen() error");

	// fd_set 변수 초기화 및 server socket을 fd_set 변수에 등록
	FD_ZERO(&reads);
	FD_SET(ServerSocket, &reads);

	FileDescriptorMax = ServerSocket;
	while(1)
	{
		// fd_set 변수 복사
		cpy_reads = reads;

		// timeout 설정
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		// 검사 범위 설정 및 select function 호출
		SelectResult = select(FileDescriptorMax + 1, &cpy_reads, 0, 0, &timeout);
		if(-1 == SelectResult)
		{
			puts("select() error");
			break;
		}

		// time-out된 경우
		if(0 == SelectResult)
		{
			puts("Time-out");
			continue;
		}
		
		// event가 발생한 경우
		// event가 발생한 descriptor 찾기
		for(int i=0; i<FileDescriptorMax+1; i++)
		{
			if(FD_ISSET(i, &cpy_reads))
			{
				// server socket에 데이터 수신 여부 event가 발생
				if(i == ServerSocket)
				{
					// 연결 요청 수락(accept)
					ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientSocket, &AddressSize);
					if(-1 == ClientSocket)
						ErrorHandling("accept() error");

					// fd_set 변수에 client socket 등록
					FD_SET(ClientSocket, &reads);

					if(ClientSocket > FileDescriptorMax)
						FileDescriptorMax = ClientSocket;
					printf("connected client : %d\n", ClientSocket);
				}
				// client socket에 데이터 수신 여부 event가 발생
				else
				{
					// client socket으로 부터 data 수신(read)
					StringLength = read(i, buf, BUF_SIZE);
					if(-1 == StringLength)
						ErrorHandling("read() error");

					// EOF인 경우
					if(0 == StringLength)
					{
						// fd_set에서 client socket을 제외
						FD_CLR(i, &reads);

						// client socket 해제(close)
						close(i);

						printf("closed client : %d\n", i);
					}
					// buffer에 저장된 내용을 client에 전송(write)
					else
					{
						StringLength = write(i, buf, StringLength);
						if(-1 == StringLength)
							ErrorHandling("write() error");
					}

				}
			}
		}
	}

	// server socket 해제(close)
	close(ServerSocket);

	return 0;
}
