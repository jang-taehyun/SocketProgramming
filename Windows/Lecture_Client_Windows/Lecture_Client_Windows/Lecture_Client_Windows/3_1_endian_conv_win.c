#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <winsock2.h>

typedef unsigned short UShort;
typedef unsigned long ULong;

void ErrorHandling(const char* message);

int main()
{
	WSADATA wsaData;
	UShort HostPort, NetworkPort;
	ULong HostAddress, NetworkAddress;
	int OperationResult;

	HostPort = 0x1234;
	HostAddress = 0x12345678;

	OperationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (OperationResult)
		ErrorHandling("WSAStartup() function error");

	NetworkPort = htons(HostPort);
	NetworkAddress = htonl(HostAddress);

	printf("Host ordered port: %#x\n", HostPort);
	printf("Network ordered port: %#x\n", NetworkPort);
	printf("Host ordered address: %#x\n", HostAddress);
	printf("Network ordered address: %#x\n", NetworkAddress);

	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}