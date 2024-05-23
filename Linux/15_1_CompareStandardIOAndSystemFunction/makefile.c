#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main()
{
	int fd;

	fd = open("news.txt", O_WRONLY | O_CREAT | O_TRUNC);

	for(unsigned long long i = 0; i<10000000; i++)
		write(fd, "123456789\n", strlen("123456789\n"));

	close(fd);

	return 0;
}
