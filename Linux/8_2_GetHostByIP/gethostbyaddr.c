#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	struct hostent* host;
	struct sockaddr_in addr;

	// argument 검사
	if(2 != argc)
	{
		printf("Usage : %s <IP>\n", argv[0]);
		exit(1);
	}

	// IP 주소를 in_addr 타입으로 변경
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	// IP를 이용해 domain name 받아오기
	host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
	if(NULL == host)
		ErrorHandling("gethost...error");

	// 받아온 정보 출력
	// 공식 도메인 이름(Official domain name)
	printf("Official domain name : %s", host->h_name);
	// 공식 도메인 이름 이외에 접속할 수 있는 다른 domain name들
	for(int i=0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s\n", i+1, host->h_aliases[i]);
	// IP 주소 체계
	printf("Address type : %s\n", host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");
	// domain name과 묶인 다른 IP들
	for(int i=0; host->h_addr_list[i]; i++)
		printf("IP addr %d : %s\n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	return 0;
}
