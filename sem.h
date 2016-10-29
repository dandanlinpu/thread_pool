#ifndef __SEM__H
#define __SEM__H

#include <iostream>
#include <pthread.h>
#include <functional>

class sem{
public:
	sem(int _v=0);	//v初始默认为0
	void wait(std::function<void(void)> after_wait);		 //wait()->after_wait  ,v--
	void signal(std::function<void(void)> befor_signal);   	 //befor_signal->signal()  ,v++
private:
	int v; 	//资源数目
	pthread_mutex_t mutex;
	pthread_cond_t cond;	
};
#endif
