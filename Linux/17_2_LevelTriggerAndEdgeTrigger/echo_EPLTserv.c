#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int ServerSocket, ClientSocket;
	struct sockaddr_in ServerAddress, ClientAddress;
	socklen_t AddressSize;

	char buf[BUF_SIZE];
	int StringLength;

	struct epoll_event* EpollEvents;
	struct epoll_event Event;
	int EpollFileDescriptor, EventCount;

	int FunctionResult;

	if(2 != argc)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	ServerSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(-1 == ServerSocket)
		ErrorHandling("socket() error");

	AddressSize = sizeof(ClientAddress);
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	if(-1 == FunctionResult)
		ErrorHandling("bind() error");

	FunctionResult = listen(ServerSocket, 5);
	if(-1 == FunctionResult)
		ErrorHandling("listen() error");

	EpollFileDescriptor = epoll_create(EPOLL_SIZE);
	EpollEvents = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

	Event.events = EPOLLIN;
	Event.data.fd = ServerSocket;
	epoll_ctl(EpollFileDescriptor, EPOLL_CTL_ADD, ServerSocket, &Event);

	while(1)
	{
		EventCount = epoll_wait(EpollFileDescriptor, EpollEvents, EPOLL_SIZE, -1);
		if(-1 == EventCount)
		{
			puts("epoll_wait() error");
			break;
		}

		puts("return epoll_wait");
		for(int i=0; i<EventCount; i++)
		{
			if(EpollEvents[i].data.fd == ServerSocket)
			{
				ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientSocket, &AddressSize);
				if(-1 == ClientSocket)
					ErrorHandling("accept() error");

				Event.events = EPOLLIN;
				Event.data.fd = ClientSocket;
				epoll_ctl(EpollFileDescriptor, EPOLL_CTL_ADD, ClientSocket, &Event);
				
				printf("connected client: %d\n", ClientSocket);
			}
			else
			{
				StringLength = read(EpollEvents[i].data.fd, buf, BUF_SIZE);
				if(0 == StringLength)
				{
					epoll_ctl(EpollFileDescriptor, EPOLL_CTL_DEL, EpollEvents[i].data.fd, NULL);
					close(EpollEvents[i].data.fd);
					printf("closed client : %d\n", EpollEvents[i].data.fd);
				}
				else
					write(EpollEvents[i].data.fd, buf, StringLength);
			}
		}
	}

	close(ServerSocket);
	close(EpollFileDescriptor);

	return 0;
}
