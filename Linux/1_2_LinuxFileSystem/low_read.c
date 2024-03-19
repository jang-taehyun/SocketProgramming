#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFFER_SIZE 100

void ErrorHandling(const char* message);

int main(int argc, char* argv[])
{
	int FileDescriptor;		// file descriptor
	char BringInData[BUFFER_SIZE];	// 파일에서 가져온 데이터
	int OperationResult;		// 함수 동작 결과
	
	// 파일을 연다.
	FileDescriptor = open("data.txt", O_RDONLY);
	if(-1 == FileDescriptor)
		ErrorHandling("open() function error");
	printf("file descriptor : %d\n", FileDescriptor);

	// 열린 파일의 데이터를 읽어온다.
	OperationResult = read(FileDescriptor, BringInData, sizeof(BringInData));
	if(-1 == OperationResult)
		ErrorHandling("read() function error");

	// 가져온 데이터를 출력하고 파일을 닫는다.
	printf("file data : %s\n", BringInData);
	close(FileDescriptor);

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
