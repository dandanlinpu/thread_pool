#include<iostream>
#include<list>
#include<vector>
#include<functional>
#include<pthread.h>
#include<signal.h>
class bsem{
public:
	void init();
	void wait();
	void signal();
private:
	int v;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	
};

class jobqueue{
public:
	bsem has_jobs;
	void init();
	void push(std::function<void(void)>); //push get是线程安全的
	std::function<void(void)> pull();
	void printinfo();
private:
	pthread_mutex_t _rwmutex;
	std::list<std::function<void(void)> > _joblist;
};

//thread
class thread{
public:
    void init(std::function<void(void)> _do);
    void start();
    void pause();
    void resume();
private:
    pthread_t id;
    volatile bool pause_flag; 

    std::function<void(void)>_do;
    void thread_do(); //调用_do
    static void* thread_do_helper(void *arg);
    void pause_do(int sig);
};
//thpool
class thpool{
public:
    thpool(int _n);
    void init();
    void add_work(std::function<void(void)> work);
    void start();
    void pause(); //线程不再去读取任务队列,正在运行的任务不受影响,如果有线程处于wait,那么是不受此pause_flag
    void resume();
    void destroy(){};
    void threads_pause(){}; 
    void threads_resume(){};
private:    
    std::vector<thread> threads;
	void threads_do(int);
    int n;    
    jobqueue job_q;
    volatile bool pause_flag;
};
