#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	struct hostent* host;
	SOCKADDR_IN addr;

	int FunctionResult;

	// argument �˻�
	if (2 != argc)
	{
		printf("Usage : %s <IP>\n", argv[0]);
		exit(1);
	}

	// ������ ���� ���̺귯�� �ʱ�ȭ
	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	// IP�� in_addr Ÿ������ ����
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	// IP�� �̿��� �ش� �ּ��� ���� �޾ƿ���
	host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
	if (NULL == host)
		ErrorHandling("gethost...error");

	// �޾ƿ� ���� ���
	// ���� ������ �̸�(Official Domain Name)
	printf("Official Domain Name : %s\n", host->h_name);
	// ���� ������ �̸� �ܿ� ������ �� �ִ� ������ �̸�(aliases)
	for (int i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s\n", i + 1, host->h_aliases[i]);
	// IP �ּ� ü��
	printf("Address type : %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
	// ������ �̸��� ���� �ִ� �ٸ� IP��
	for (int i = 0; host->h_addr_list[i]; i++)
		printf("IP addr %d : %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	// ������ ���� ���̺귯�� ����
	WSACleanup();

	return 0;
}