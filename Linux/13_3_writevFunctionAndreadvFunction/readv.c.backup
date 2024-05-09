#include <stdio.h>
#include <sys/uio.h>
#include <string.h>
#define BUF_SIZE 30

int main()
{
	struct iovec vec[2];
	char buf1[BUF_SIZE];
	char buf2[BUF_SIZE];
	int StringLength;

	memset(buf1, 0, sizeof(buf1));
	memset(buf2, 0, sizeof(buf2));

	vec[0].iov_base = buf1;
	vec[0].iov_len = 5;
	
	vec[1].iov_base = buf2;
	vec[1].iov_len = BUF_SIZE;

	StringLength = readv(0, vec, 2);
	printf("Read bytes : %d\n", StringLength);
	printf("First message : %s\n", buf1);
	printf("Second message : %s\n", buf2);

	return 0;
}
