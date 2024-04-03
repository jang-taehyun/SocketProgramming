#include <stdio.h>
#include <stdlib.h>
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

	// argument 검사
	if(2 != argc)
	{
		printf("Usage : %s <addr>\n", argv[0]);
		exit(1);
	}

	// domain name으로 ip 주소 얻어오기
	host = gethostbyname(argv[1]);
	if(!host)
		ErrorHandling("gethost...error");

	// hostent 정보 출력
	// 공식 도메인 이름(Official domain name)
	printf("Official name: %s\n", host->h_name);
	// 공식 도메인 이름 이외에 메인 페이지로 접속할 수 있는 다른 domain name들
	for(int i=0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s\n", i+1, host->h_aliases[i]);
	// IP 주소 체계
	printf("Address type : %s\n", (host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6"));
	// domain name에 묶인 IP 주소들
	for(int i=0; host->h_addr_list[i]; i++)
		printf("IP addr %d : %s\n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	return 0;
}
