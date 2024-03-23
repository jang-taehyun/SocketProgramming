#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#define ADDRESS_LENGTH 16

int main()
{
	struct sockaddr_in Address1, Address2;
	char* StringPointer1, *StringPointer2;
	char ConvertAddress[ADDRESS_LENGTH];

	Address1.sin_addr.s_addr = htonl(0x1020304);
	Address2.sin_addr.s_addr = htonl(0x1010101);

	StringPointer1 = inet_ntoa(Address1.sin_addr);
	strcpy(ConvertAddress, StringPointer1);
	printf("Dotted Decimal notation1_1 : %s\n", StringPointer1);
	printf("Dooted Decimal notation1_2 : %s\n", ConvertAddress);

	StringPointer2 = inet_ntoa(Address2.sin_addr);
	printf("Dotted Decimal notation2 : %s\n", StringPointer2);
	printf("Dotted Decimal notation1_1 : %s\n", StringPointer1);
	printf("Dotted Decimal notation1_2 : %s\n", ConvertAddress);

	return 0;
}
