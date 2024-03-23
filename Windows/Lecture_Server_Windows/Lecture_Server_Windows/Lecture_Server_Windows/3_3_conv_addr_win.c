#pragma comment(lib, "ws2_32.lib")

#undef UNICODE
#undef _UNICODE

#include <stdio.h>
#include <winsock2.h>
#define STRING_BUFFER_SIZE 50

int main()
{
	const char* StringAddress = "1.2.3.4:9190";

	char StringAddressBuffer[STRING_BUFFER_SIZE];
	SOCKADDR_IN ServerAddress;
	int Size;

	WSADATA wsaData;
	int OperationResult;

	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
	{
		printf("WSAStartup() function error\n");
		exit(1);
	}

	Size = sizeof(ServerAddress);
	WSAStringToAddress(StringAddress, AF_INET, NULL, (SOCKADDR*)&ServerAddress, &Size);
	printf("First convert result : %#lx\n", ServerAddress.sin_addr.s_addr);

	Size = sizeof(StringAddressBuffer);
	WSAAddressToString((SOCKADDR*)&ServerAddress, sizeof(ServerAddress), NULL, StringAddressBuffer, &Size);
	printf("Second convert result : %s\n", StringAddressBuffer);

	WSACleanup();

	return 0;
}