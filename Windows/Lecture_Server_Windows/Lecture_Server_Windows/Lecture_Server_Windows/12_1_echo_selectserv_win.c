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

	// argument �˻�
	if (2 != argc)
	{
		printf("Usage %s <port>\n", argv[0]);
		exit(1);
	}

	// ������ ���� ���̺귯�� �ʱ�ȭ
	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	// server socket ����(socket)
	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == ServerSocket)
		ErrorHandling("socket() error");

	// server address ���� �Է�
	AddressSize = sizeof(ServerAddress);
	memset(&ServerAddress, 0, AddressSize);
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	// server socket�� server address ���� �Ҵ�(bind)
	FunctionResult = bind(ServerSocket, (SOCKADDR*)&ServerAddress, AddressSize);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("bind() error");

	// server socket�� ���� ��û ��� ���·� ��ȯ(listen)
	FunctionResult = listen(ServerSocket, 5);
	if (SOCKET_ERROR == FunctionResult)
		ErrorHandling("listen() error");

	// fd_set ���� �ʱ�ȭ �� server socket�� fd_set ������ ���
	FD_ZERO(&reads);
	FD_SET(ServerSocket, &reads);

	while (1)
	{
		// fd_set ���� backup
		cpyreads = reads;

		// timeout ����
		timeout.tv_sec = 5;
		timeout.tv_usec = 5000;

		// �˻� ���� ���� �� select function ȣ��
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

		// event�� �߻��� ���
		// event�� �߻��� handle ã��
		for (int i = 0; i < reads.fd_count; i++)
		{
			if (FD_ISSET(reads.fd_array[i], &cpyreads))
			{
				// server socket�� ������ ���� ���� event�� �߻�
				if (reads.fd_array[i] == ServerSocket)
				{
					// client�κ��� ���� ��û ����(accept)
					ClientSocket = accept(ServerSocket, (SOCKADDR*)&ClientAddress, &AddressSize);
					if (INVALID_SOCKET == ClientSocket)
						ErrorHandling("accept() error");

					// client socket handle�� fd_set ������ ���
					FD_SET(ClientSocket, &reads);

					printf("connected client: %d\n", ClientSocket);
				}
				// client socket�� ������ ���� ���� event�� �߻�
				else
				{
					// client�κ��� ������ ����(recv)
					StringLength = recv(reads.fd_array[i], buf, BUF_SIZE - 1, 0);

					// EOF
					if (!StringLength)
					{
						// fd_set �������� client socket ����
						FD_CLR(reads.fd_array[i], &reads);

						// client socket ����
						closesocket(reads.fd_array[i]);

						printf("closed client: %d\n", reads.fd_array[i]);
					}
					// client�� data ����(send)
					else
					{
						send(reads.fd_array[i], buf, StringLength, 0);
					}
				}
			}
		}
	}

	// server socket ����
	closesocket(ServerSocket);

	// ������ ���� ���̺귯�� ����
	WSACleanup();

	return 0;
}