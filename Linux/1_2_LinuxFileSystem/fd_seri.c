#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main()
{
	int FileDescriptor[3];
	FileDescriptor[0] = socket(PF_INET, SOCK_STREAM, 0);
	FileDescriptor[1] = open("./test.dat", O_CREAT | O_WRONLY | O_TRUNC);
	FileDescriptor[2] = socket(PF_INET, SOCK_DGRAM, 0);

	for(int i=0; i<3; i++)
	{
		printf("file descriptor %d: %d\n", i+1, FileDescriptor[i]);
		close(FileDescriptor[i]);
	}

	return 0;
}
