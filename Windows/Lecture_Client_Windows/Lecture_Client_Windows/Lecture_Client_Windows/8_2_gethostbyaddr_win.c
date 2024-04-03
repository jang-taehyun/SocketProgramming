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

	// argument 검사
	if (2 != argc)
	{
		printf("Usage : %s <IP>\n", argv[0]);
		exit(1);
	}

	// 윈도우 소켓 라이브러리 초기화
	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	// IP를 in_addr 타입으로 변경
	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	// IP를 이용해 해당 주소의 정보 받아오기
	host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
	if (NULL == host)
		ErrorHandling("gethost...error");

	// 받아온 정보 출력
	// 공식 도메인 이름(Official Domain Name)
	printf("Official Domain Name : %s\n", host->h_name);
	// 공식 도메인 이름 외에 접속할 수 있는 도메인 이름(aliases)
	for (int i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s\n", i + 1, host->h_aliases[i]);
	// IP 주소 체계
	printf("Address type : %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
	// 도메인 이름에 묶여 있는 다른 IP들
	for (int i = 0; host->h_addr_list[i]; i++)
		printf("IP addr %d : %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	// 윈도우 소켓 라이브러리 해제
	WSACleanup();

	return 0;
}