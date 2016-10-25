#include<iostream>
#include<list>
#include<vector>
#include<functional>
#include<pthread.h>
#include<signal.h>
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
	void push(function<void(void)>); //push get是线程安全的
	function<void(void)> pull();
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

class thread{
public:
    void init(std::function<void(void)> _do){
      //  me=this;
        this->_do=_do;
        pause_flag=0;
    }
    void start(){
        pthread_create(&id,NULL,do_helper,NULL); 
    }
    void pause(){ 
        pause_flag=1;
        pthread_kill(id,SIGUSR1);
    };
    void resume(){
        pause_flag=0;
    };
private:
    void thread_do(){
        //register puase signal hanlder
        struct sigaction act;
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;
        act.sa_handler=pause_do_helper;
        //exit_if(sigaction(SIGUSR1,&act,NULL));
        if (sigaction(SIGUSR1, &act, NULL) == -1) {
            fprintf(stderr, "thread_do(): cannot handle SIGUSR1");
        }    
        //external do function
        _do();  
    }
    static void* do_helper(void *arg){
        ((thread*) arg)->thread_do();
    }
    void pause_do(){
        while(pause_flag==1){ }
    }
    static void pause_do_helper(int signum){
       me->pause_do(); 
    }
    pthread_t id;
    function<void(void)>_do;
    volatile bool pause_flag; 
    const static thread *me=this;  //store a static pointer to 'this',should be some problem 
};
