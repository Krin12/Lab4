#include <pthread.h>
void *create_thread (void *arg){
	printf ("Thread: Hello, World!\n");
	return arg;
}
int main(){
	pthread_t tid;
	int status;
	/* 쓰레드 생성 */
	status = pthread_create (&tid, NULL, create_thread, NULL);
	if (status != 0)
		perror ("Create thread");
	pthread_exit (NULL);
}
