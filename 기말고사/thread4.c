//mutex.c
//thread4.c를 동기화 수정하여 실행

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREAD 100

//전역변수
void *thread_inc(void *arg);
void *thread_des(void *arg);

long long num = 0;
pthread_mutex_t mutex; // 뮤텍스 변수 선언

int main(int argc, char *argv[])
{
	pthread_t thread_id[NUM_THREAD];
	int i;

	pthread_mutex_init(&mutex, NULL); // 뮤텍스 등록

	for (i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			pthread_create(&(thread_id[i]), NULL, thread_inc, NULL);
		else
			pthread_create(&(thread_id[i]), NULL, thread_des, NULL);
	}

	for (i = 0; i < NUM_THREAD; i++)
		pthread_join(thread_id[i], NULL);

	printf("result: %lld \n", num);
	pthread_mutex_destroy(&mutex); //소멸
	return 0;
}

void *thread_inc(void *arg)
{
	int i;
	//for 밖에서 lock하면 한번밖에 안돈다
	pthread_mutex_lock(&mutex);
	for (i = 0; i < 50000000; i++)
	{
		num += 1;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}

void *thread_des(void *arg)
{
	int i;
	pthread_mutex_lock(&mutex); //for문에 넣어줌 -> 많이 실행
	for (i = 0; i < 50000000; i++)
	{
		num -= 1;
	}
	pthread_mutex_unlock(&mutex);
	return NULL;
}

/*
swyoon@com:~/tcpip$ gcc mutex.c -D_REENTRANT -o mutex -lpthread
swyoon@com:~/tcpip$ ./mutex
result: 0 

*/