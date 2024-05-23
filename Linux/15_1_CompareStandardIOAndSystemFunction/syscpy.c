#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#define BUF_SIZE 30

int main()
{
	int fd1, fd2;
	int length;
	char buf[BUF_SIZE];

	fd1 = open("news.txt", O_RDONLY);
	fd2 = open("cpy.txt", O_WRONLY | O_CREAT | O_TRUNC);

	while(1)
	{
		length = read(fd1, buf, sizeof(buf));
		if(length <= 0)
			break;
		write(fd2, buf, length);
	}

	close(fd1);
	close(fd2);

	return 0;
}
