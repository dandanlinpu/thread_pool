#include<iostream>
#include<list>
#include<vector>
#include<functional>
#include<pthread.h>
using namespace std;
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
	void push(function<void(void)>);
	function<void(void)> get();
	void printinfo();
private:
	pthread_mutex_t _rwmutex;
	list<std::function<void(void)> > _joblist;
};

class thread_pool{
public:
	thread_pool(int n);
	volatile bool keep_working;
	void init();
	void start();
	void stop();
	void add_work(function<void(void)> work);
private:
	int _n;
	void _threads_do();
	static void *_threads_do_helper(void *context);
	vector<pthread_t> _threads_id;
	jobqueue _job_q;
	//int _num_threads_alive;
	//pthread_mutex_t _mutex;
};
	
