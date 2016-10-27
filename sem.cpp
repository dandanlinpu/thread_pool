#include "sem.h"
//sem
sem::sem(int _v):v(_v){
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);		
}
void sem::wait(std::function<void(void)> after_wait){ 	//移动语义可以加快速度
	pthread_mutex_lock(&mutex);
	while(v==0){
		pthread_cond_wait(&cond,&mutex);
	}
	if(!after_wait) {
		after_wait();
		v--;
	}
	pthread_mutex_unlock(&mutex);
}
void sem::signal(std::function<void(void)> before_signal){	//移动语义可以加快速度
	pthread_mutex_lock(&mutex);
	if(!before_signal) {
		before_signal();
		v++;
	}
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);	
}

#define test1
#ifdef test
	int main(){


	}
#endif