#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;								// ������ ���� ���̺귯�� ����
	SOCKET HandleServerSocket, HandleClientSocket;	// server socket handle, client socket handle
	SOCKADDR_IN ServerAddress, ClientAddress;		// server �ּ�, client �ּ�
	int ClientAddressSize;							// client �ּ��� ũ��
	int OperationResult;							// �Լ� ���� ���

	const char* message = "Hello World!";

	// argument �˻�
	if (2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	// ������ ���� ���̺귯�� �ʱ�ȭ
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (!OperationResult)
		ErrorHandling("WSAStartup() function error");

	// server socket ����(socket)
	HandleServerSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (INVALID_SOCKET == HandleServerSocket)
		ErrorHandling("socket() function error");

	// server �ּ� ���� �Է�
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// IP �ּ�, port ��ȣ �Ҵ�(bind)
	OperationResult = bind(HandleServerSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("bind() function error");

	// ���� ��û�� ���� �� �ִ� ���·� ��ȯ(listen)
	OperationResult = listen(HandleServerSocket, 5);
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("listen() function error");

	// client�� ���� ��û�� �޾Ƶ��δ�.(accept)
	ClientAddressSize = sizeof(ClientAddress);
	HandleClientSocket = accept(HandleServerSocket, (SOCKADDR*)&ClientAddress, &ClientAddressSize);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("accept() function error");

	// �޽���(data)�� ����
	send(HandleClientSocket, message, sizeof(message), 0);

	// handle ����
	closesocket(HandleClientSocket);
	closesocket(HandleServerSocket);

	// ������ ���� ���̺귯�� ����
	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}