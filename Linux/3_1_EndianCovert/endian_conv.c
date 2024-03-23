#include <stdio.h>
#include <arpa/inet.h>

typedef unsigned short UShort;
typedef unsigned long ULong;

int main(int argc, char* argv[])
{
	UShort HostPort = 0x1234;
	UShort NetworkPort;
	ULong HostAddress = 0x12346789;
	ULong NetworkAddress;

	NetworkPort = htons(HostPort);
	NetworkAddress = htonl(HostAddress);

	printf("Host ordered port: %#x\n", HostPort);
	printf("Network ordered port: %#x\n", NetworkPort);
	printf("Host ordered address: %#lx\n", HostAddress);
	printf("Network ordered address: %#lx\n", NetworkAddress);

	return 0;
}
