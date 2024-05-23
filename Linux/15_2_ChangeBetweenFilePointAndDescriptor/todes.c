#include <stdio.h>
#include <fcntl.h>

int main()
{
	FILE* fp;
	int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
	if(-1 == fd)
	{
		fputs("file open error\n", stdout);
		return -1;
	}

	printf("First file descriptor : %d\n", fd);

	fp = fdopen(fd, "w");
	if(NULL == fp)
	{
		fputs("file open error in fdopen()\n", stdout);
		return -1;
	}

	fputs("TCP/IP Socket programming\n", fp);
	printf("Second file descriptor : %d\n", fileno(fp));

	fclose(fp);

	return 0;
}
