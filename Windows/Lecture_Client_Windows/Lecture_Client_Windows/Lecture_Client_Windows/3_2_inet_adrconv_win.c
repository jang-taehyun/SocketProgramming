#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS

typedef unsigned long ULong;

void ErrorHandling(const char* message);

int main()
{
	WSADATA wsaData;
	int OperationResult;

	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() function error");

	{
		const char* Address = "127.212.124.78";
		ULong ConvertAddress = inet_addr(Address);
		if (INADDR_NONE == ConvertAddress)
			printf("Error occured!\n");
		else
			printf("Network ordered integer address : %#lx\n", ConvertAddress);
	}

	{
		struct sockaddr_in Address;
		char* AddressPointer;
		char StringAddress[16];

		Address.sin_addr.S_un.S_addr = htonl(0x1020304);
		AddressPointer = inet_ntoa(Address.sin_addr);
		strcpy(StringAddress, AddressPointer);
		printf("Dotted-Decimal notation1 : %s\n", AddressPointer);
		printf("Dotted-Decimal notation2 : %s\n", StringAddress);
	}

	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}