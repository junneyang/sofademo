#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int shared_num=10000;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *print_msg(void *msg){
	unsigned int usecs=10;
	for(int i=0; i<2000; i++){
		usleep(usecs);
		
		pthread_mutex_lock(&mutex);
		shared_num--;
		fprintf(stdout, "shared_num:%d\n", shared_num);
		pthread_mutex_unlock(&mutex);
	}
	char *ret="abcd";
	//return NULL;
	return ret;
}

int main()
{
	fprintf(stdout, "helloworld\n");
	pthread_t thread1,thread2,thread3,thread4,thread5;
	char *msg="thread001";
	
	int create_status=pthread_create(&thread1, NULL, print_msg, (void *)msg);
	fprintf(stdout, "create_status:%d\n", create_status);
	create_status=pthread_create(&thread2, NULL, print_msg, (void *)msg);
	fprintf(stdout, "create_status:%d\n", create_status);
	create_status=pthread_create(&thread3, NULL, print_msg, (void *)msg);
	fprintf(stdout, "create_status:%d\n", create_status);
	create_status=pthread_create(&thread4, NULL, print_msg, (void *)msg);
	fprintf(stdout, "create_status:%d\n", create_status);
	create_status=pthread_create(&thread5, NULL, print_msg, (void *)msg);
	fprintf(stdout, "create_status:%d\n", create_status);
	
	void *ret;
	int join_status=pthread_join(thread1, &ret);
	fprintf(stdout, "join_status:%d\n", join_status);
	fprintf(stdout, "ret:%s\n", (char *)ret);
	fprintf(stdout, "thread001 end\n");
	join_status=pthread_join(thread2, &ret);
	fprintf(stdout, "join_status:%d\n", join_status);
	fprintf(stdout, "ret:%s\n", (char *)ret);
	fprintf(stdout, "thread001 end\n");
	join_status=pthread_join(thread3, &ret);
	fprintf(stdout, "join_status:%d\n", join_status);
	fprintf(stdout, "ret:%s\n", (char *)ret);
	fprintf(stdout, "thread001 end\n");
	join_status=pthread_join(thread4, &ret);
	fprintf(stdout, "join_status:%d\n", join_status);
	fprintf(stdout, "ret:%s\n", (char *)ret);
	fprintf(stdout, "thread001 end\n");
	join_status=pthread_join(thread5, &ret);
	fprintf(stdout, "join_status:%d\n", join_status);
	fprintf(stdout, "ret:%s\n", (char *)ret);
	fprintf(stdout, "thread001 end\n");
	
	return 0;
}

