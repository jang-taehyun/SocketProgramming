#include <stdio.h>
#include <windows.h>
#include <process.h>
#define NUM_THREAD 50

unsigned WINAPI threadInc(void* arg);
unsigned WINAPI threadDes(void* arg);

long long num = 0;
CRITICAL_SECTION cs;

int main()
{
	HANDLE tHandles[NUM_THREAD];

	InitializeCriticalSection(&cs);

	for (int i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			tHandles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, tHandles, TRUE, INFINITE);

	DeleteCriticalSection(&cs);

	printf("result: %lld\n", num);

	return 0;
}

unsigned WINAPI threadInc(void* arg)
{
	EnterCriticalSection(&cs);
	for (int i = 0; i < 50000000; i++)
		num++;
	LeaveCriticalSection(&cs);

	return 0;
}

unsigned WINAPI threadDes(void* arg)
{
	EnterCriticalSection(&cs);
	for (int i = 0; i < 50000000; i++)
		num--;
	LeaveCriticalSection(&cs);

	return 0;
}