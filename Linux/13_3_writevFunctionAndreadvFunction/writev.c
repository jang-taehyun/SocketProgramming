#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void TestFunction(const int descriptor, const struct iovec* _v, const int len)
{
	int StringLength = writev(descriptor, _v, len);
	if(-1 == StringLength)
		ErrorHandling("writev() error");
	puts("");
	printf("Write bytes : %d\n", StringLength);
}

int main()
{
	struct iovec vec[2];
	char buf1[] = "ABCDEFG";
	char buf2[] = "1234567";

	vec[0].iov_base = buf1;
	vec[0].iov_len = 3;

	vec[1].iov_base = buf2;
	vec[1].iov_len = 4;

	TestFunction(1, vec, 2);
	TestFunction(1, &vec[0], 1);
	TestFunction(1, &vec[1], 1);

	return 0;
}
