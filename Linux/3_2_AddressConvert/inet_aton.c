#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
	struct sockaddr_in AddressUseInet;

	for(int i=1; i<argc; i++)
	{
		if(!inet_aton(argv[i], &AddressUseInet.sin_addr))
			printf("%s : Conversation Error!\n", argv[i]);
		else
			printf("Network ordered integer address : %#x\n", AddressUseInet.sin_addr.s_addr);
	}

	return 0;
}
