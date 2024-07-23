#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50
#define DETECT_ERROR(num, msg) if(-1 == (num)) ErrorHandling((msg))

void ErrorHandling(const char* message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}

void SetNonBlockingMode(int fd)
{
	int flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag | O_NONBLOCK);
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
	DETECT_ERROR(ServerSocket, "socket() error");

	AddressSize = sizeof(ClientAddress);
	memset(&ServerAddress, 0, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	ServerAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddress.sin_port = htons(atoi(argv[1]));

	FunctionResult = bind(ServerSocket, (struct sockaddr*)&ServerAddress, sizeof(ServerAddress));
	DETECT_ERROR(FunctionResult, "bind() error");

	FunctionResult = listen(ServerSocket, 5);
	DETECT_ERROR(FunctionResult, "listen() error");

	EpollFileDescriptor = epoll_create(EPOLL_SIZE);
	EpollEvents = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

	SetNonBlockingMode(ServerSocket);

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
				ClientSocket = accept(ServerSocket, (struct sockaddr*)&ClientAddress, &AddressSize);
				DETECT_ERROR(ClientSocket, "accept() error");

				SetNonBlockingMode(ClientSocket);

				Event.events = EPOLLIN | EPOLLET;
				Event.data.fd = ClientSocket;

				epoll_ctl(EpollFileDescriptor, EPOLL_CTL_ADD, ClientSocket, &Event);
				printf("connected client : %d\n", ClientSocket);
			}
			else
			{
				while(1)
				{
					StringLength = read(EpollEvents[i].data.fd, buf, BUF_SIZE);
					if(0 == StringLength)
					{
						epoll_ctl(EpollFileDescriptor, EPOLL_CTL_DEL, EpollEvents[i].data.fd, NULL);
						close(EpollEvents[i].data.fd);
						printf("closed client : %d\n", EpollEvents[i].data.fd);
						break;
					}
					else if(StringLength < 0)
					{
						if(EAGAIN == errno)
							break;
					}
					else
					{
						write(EpollEvents[i].data.fd, buf, StringLength);
					}
				}
			}
		}
	}

	close(ServerSocket);
	close(EpollFileDescriptor);

	return 0;
}
