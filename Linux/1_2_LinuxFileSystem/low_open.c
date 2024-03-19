#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void ErrorHandling(const char* message);

int main()
{
	int FileDescriptor;
	char StoreData[] = "Let's go!\n";
	int OperationResult;

	// 파일을 open/create하여 연다.
	FileDescriptor = open("./data.txt", O_CREAT | O_WRONLY | O_TRUNC);
	if(-1 == FileDescriptor)
		ErrorHandling("open() function error");
	printf("file descriptor : %d\n", FileDescriptor);

	// 연 파일에 data를 쓴다.
	OperationResult = write(FileDescriptor, StoreData, sizeof(StoreData));
	if(-1 == OperationResult)
		ErrorHandling("write() function error");

	// 파일을 닫는다.
	close(FileDescriptor);

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
