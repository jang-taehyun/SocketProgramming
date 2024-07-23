#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100

typedef long long LL;

LL sum=0;
int inc=0;
int des=0;

void* thread_inc(void* arg)
{
    for(LL i=0; i<50000000; i++)
        *(&sum) += 1;

    inc++;
    return NULL;
}

void* thread_des(void* arg)
{
    for(LL i=0; i<50000000; i++)
        *(&sum) -= 1;

    des++;
    return NULL;
}

int main(int argc, char* argv[])
{
    pthread_t thread_id[NUM_THREAD];

    printf("sizeof long long: %ld\n", sizeof(LL));
    for(int i=0; i<NUM_THREAD; i++)
    {
        if(i%2)
        {
            if(pthread_create(&thread_id[i], NULL, thread_inc, NULL))
            {
                puts("pthread_create() error");
                return -1;
            }
        }
        else
        {
            if(pthread_create(&thread_id[i], NULL, thread_des, NULL))
            {
                puts("pthread_create() error");
                return -1;
            }
        }
    }

    for(int i=0; i<NUM_THREAD; i++)
    {
        if(pthread_join(thread_id[i], NULL))
        {
            puts("pthread_join() error");
            return -1;
        }
    }

    printf("result: %lld\n", sum);
    printf("increase call count : %d\n", inc);
    printf("decrease call count : %d\n", des);

    return 0;
}
