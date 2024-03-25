#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define BUF_SIZE 1024
#define RESULT_SIZE 4
#define OPERAND_SIZE 4

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;						// ������ ���� ���̺귯�� ����
	SOCKET HandleClientSocket;				// client socket
	SOCKADDR_IN ServerAddress;				// server address ����
	int OperationResult;					// �Լ� ���� ���

	char OperationMessage[BUF_SIZE];		// ������ �޼���
	int OperandCount;						// �ǿ����� ����
	int result;								// ���� ���

	// argument �˻�
	if (3 != argc)
	{
		printf("Usage : %s <IP> <Port>\n", argv[0]);
		exit(1);
	}

	// ������ ���� ���̺귯�� ���� �ʱ�ȭ
	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() error");

	// client socket ����(socket)
	HandleClientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == HandleClientSocket)
		ErrorHandling("socket() error");

	// server address ���� �Է�
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = inet_addr(argv[1]);
	ServerAddress.sin_port = htons(atoi(argv[2]));

	// server�� ���� ��û(connect)
	OperationResult = connect(HandleClientSocket, (SOCKADDR*)&ServerAddress, sizeof(ServerAddress));
	if (SOCKET_ERROR == OperationResult)
		ErrorHandling("connect() error");
	else
		puts("Connected............");

	// �ǿ�����, ������ �Է�
	fputs("Operand count: ", stdout);
	scanf("%d", &OperandCount);
	OperationMessage[0] = (char)OperandCount;

	for (int i = 0; i < OperandCount; i++)
	{
		printf("Operand %d : ", i + 1);
		scanf("%d", (int*)&OperationMessage[i * OPERAND_SIZE + 1]);
	}
	fgetc(stdin);

	fputs("Operator : ", stdout);
	scanf("%c", &OperationMessage[OperandCount * OPERAND_SIZE + 1]);

	// �Էµ� ������ server�� ����(write)
	OperationResult = send(HandleClientSocket, OperationMessage, OperandCount * OPERAND_SIZE + 2, 0);
	
	// server���� �� �޽���(������) ����(read)
	recv(HandleClientSocket, &result, RESULT_SIZE, 0);

	printf("Operation result: %d\n", result);

	// client socket ����
	closesocket(HandleClientSocket);

	// ������ ���� ���̺귯�� ���� ����
	WSACleanup();

	return 0;
}