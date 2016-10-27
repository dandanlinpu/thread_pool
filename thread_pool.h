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

class thread_pool{
public:
	thread_pool(int n);
	volatile bool keep_working;
	void init();
	void start();
	void stop();
	void add_work(std::function<void(void)> work);
private:
	int _n;
	void _threads_do();
	static void *_threads_do_helper(void *context);
	std::vector<pthread_t> _threads_id;
	jobqueue _job_q;
	//int _num_threads_alive;
	//pthread_mutex_t _mutex;
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
private:    
    std::vector<thread> threads;
	void threads_do();
    int n;    
    jobqueue job_q;
};
