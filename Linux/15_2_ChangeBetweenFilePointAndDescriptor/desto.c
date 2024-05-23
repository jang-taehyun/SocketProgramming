#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	FILE* fp;
	int fd = open("data.dat", O_WRONLY | O_CREAT | O_TRUNC);
	if(-1 == fd)
	{
		fputs("file open error", stdout);
		return -1;
	}

	fp = fdopen(fd, "w");
	if(NULL == fp)
	{
		fputs("file open error in fdopen()", stdout);
		return -1;
	}

	fputs("Network C programming\n", fp);
	fclose(fp);

	return 0;
}
