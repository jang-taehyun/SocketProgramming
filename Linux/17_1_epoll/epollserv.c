#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#define BUF_SIZE 100
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

	struct epoll_event* ep_events;
	struct epoll_event event;
	int EpollDescriptor, EventCount;

	int FunctionResult;

	int StringLength;
	char buf[BUF_SIZE];

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

	EpollDescriptor = epoll_create(EPOLL_SIZE);
	ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

	event.events = EPOLLIN;
	event.data.fd = ServerSocket;
	epoll_ctl(EpollDescriptor, EPOLL_CTL_ADD, ServerSocket, &event);

	while(1)
	{
		EventCount = epoll_wait(EpollDescriptor, ep_events, EPOLL_SIZE, -1);
		if(-1 == EventCount)
		{
			puts("epoll_wait() error");
			break;
		}

		for(int i=0; i<EventCount; i++)
		{
			if(ep_events[i].data.fd == ServerSocket)
			{
				ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientSocket, &AddressSize);
				if(-1 == ClientSocket)
					ErrorHandling("accept() error");

				event.events = EPOLLIN;
				event.data.fd = ClientSocket;
				
				epoll_ctl(EpollDescriptor, EPOLL_CTL_ADD, ClientSocket, &event);
				printf("Connect client : %d\n", ClientSocket);
			}
			else
			{
				StringLength = read(ep_events[i].data.fd, buf, BUF_SIZE);
				
				if(!StringLength)
				{
					epoll_ctl(EpollDescriptor, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
					close(ep_events[i].data.fd);
					printf("closed client : %d\n", ep_events[i].data.fd);
				}
				else
				{
					write(ep_events[i].data.fd, buf, StringLength);
				}
			}
		}
	}

	close(ServerSocket);
	close(EpollDescriptor);
	free(ep_events);

	return 0;
}
