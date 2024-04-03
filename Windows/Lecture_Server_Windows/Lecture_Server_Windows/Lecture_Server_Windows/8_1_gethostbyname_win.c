#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
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

	int FunctionResult;

	// argument 검사
	if (2 != argc)
	{
		printf("Usage : %s <addr>\n", argv[0]);
		exit(1);
	}

	// 윈도우 소켓 라이브러리 초기화
	FunctionResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (FunctionResult)
		ErrorHandling("WSAStartup() error");

	// domain name을 이용해 IP 정보 얻어오기
	host = gethostbyname(argv[1]);
	if (NULL == host)
		ErrorHandling("gethost...error");

	// 얻어온 정보 출력
	// 공식 도메인 이름(Official domain name)
	printf("Official name : %s\n", host->h_name);
	// 공식 도메인 이름 외에 접속 가능한 다른 도메인 이름들(aliases)
	for (int i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d : %s\n", i + 1, host->h_aliases[i]);
	// IP 주소체계
	printf("Address type : %s\n", host->h_addrtype == AF_INET ? "AF_INET" : "AF_INET6");
	// 도메인 이름에 묶인 다른 IP들
	for (int i = 0; host->h_addr_list[i]; i++)
		printf("IP addr %d : %s\n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	// 윈도우 소켓 라이브러리 해제
	WSACleanup();

	return 0;
}