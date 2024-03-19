#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;								// 윈도우 소켓 라이브러리 정보
	SOCKET HandleServerSocket, HandleClientSocket;	// server socket handle, client socket handle
	SOCKADDR_IN ServerAddress, ClientAddress;		// server 주소, client 주소
	int ClientAddressSize;							// client 주소의 크기
	int OperationResult;							// 함수 동작 결과

	const char* message = "Hello World!";

	// argument 검사
	if (2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// 윈도우 소켓 라이브러리 초기화
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (!OperationResult)
		ErrorHandling("WSAStartup() function error");

	// server socket 생성(socket)
	HandleServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == HandleServerSocket)
		ErrorHandling("socket() function error");

	// server 주소 정보 입력
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// IP 주소, port 번호 할당(bind)
	OperationResult = bind(HandleServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("bind() function error");

	// 연결 요청을 받을 수 있는 상태로 전환(listen)
	OperationResult = listen(HandleServerSocket, 5);
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("listen() function error");

	// client의 연결 요청을 받아들인다.(accept)
	ClientAddressSize = sizeof(ClientAddress);
	HandleClientSocket = accept(HandleServerSocket, (SOCKADDR*)&ClientAddress, &ClientAddressSize);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("accept() function error");

	// 메시지(data)를 전송
	send(HandleClientSocket, message, sizeof(message), 0);

	// handle 해제
	closesocket(HandleClientSocket);
	closesocket(HandleServerSocket);

	// 윈도우 소켓 라이브러리 해제
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}