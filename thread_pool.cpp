#include "thread_pool.h"
#include "sig_wrapper.h"
#include <unistd.h> //sleeep()
//bsem
void bsem::init(){
	pthread_mutex_init(&mutex,NULL);
	pthread_cond_init(&cond,NULL);
	v=0;		
}
void bsem::wait(){
	pthread_mutex_lock(&mutex);
	while(v==0){
		pthread_cond_wait(&cond,&mutex);
	}
	v--;
	pthread_mutex_unlock(&mutex);
}
void bsem::signal(){
	pthread_mutex_lock(&mutex);
	v++;
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex);	
}
//jobqueue
void jobqueue::init(){
	has_jobs.init();
	pthread_mutex_init(&_rwmutex,NULL);
}
void jobqueue::push(std::function<void(void)> job){
	pthread_mutex_lock(&_rwmutex);
	_joblist.push_back(job);
	pthread_mutex_unlock(&_rwmutex);
}
std::function<void(void)> jobqueue::pull(){
	std::function<void(void)> r;
	pthread_mutex_lock(&_rwmutex);
	if(!_joblist.empty()){	
		r= _joblist.front();
		_joblist.pop_front();
	}else{
		r=nullptr;
	}
	pthread_mutex_unlock(&_rwmutex);
	return r;
}
void jobqueue::printinfo(){
	std::cout<<"job queue size: "<<_joblist.size()<<std::endl;
}
//thread_pool

//thread_wrapper
void thread::init(std::function<void(void)> _do){
    this->_do=_do;
    this->id=0;
    pause_flag=0;
}

void thread::start(){
    pthread_create(&id,NULL,thread_do_helper,this); 
    //为什么不在thread_do中注册？因为有可能主线程调用start() 返回后立即调用pause(),导致线程函数中注册代码未真正完成.
    sig_wrapper sig(SIGUSR1,std::bind(&thread::pause_do,this,std::placeholders::_1));
    sig.reg();
}   
void thread::pause(){ 
        if(id==0){
            fprintf(stderr,"thread is not started yet..\n");
        }else{
            std::cout<<"pause "<<id<<std::endl;
            pause_flag=1;
            pthread_kill(id,SIGUSR1);
        }
};  
void thread::resume(){
        pause_flag=0;
};  
void thread::thread_do(){
    _do();
}   
void* thread::thread_do_helper(void *arg){
    ((thread*) arg)->thread_do();
}
void thread::pause_do(int sig){
    while(pause_flag==1){
        sleep(1);
    }
}

//thpool
thpool::thpool(int _n):n(_n){
}
void thpool::add_work(std::function<void(void)>work){
    std::cout<<"add work"<<std::endl;
	job_q.push(work);
	job_q.has_jobs.signal();
}
void thpool::init(){
    pause_flag=false;
	threads.resize(n);
	job_q.init();
}
void thpool::start(){
	for(int i=0;i<n;i++){
        std::cout<<"start i:"<<i<<std::endl;
        std::function<void(void)> f=std::bind(&thpool::threads_do,this,i);
		threads[i].init(f);
        threads[i].start();
	}
}
void thpool::threads_do(int id){
	while(pause_flag==false){
        std::cout<<"threads "<<id<<std::endl;
        job_q.has_jobs.wait();
		auto work=job_q.pull();
		if(work!=nullptr){
			work();
		}
        std::cout<<"threads end "<<id<<std::endl;
	}
}
void thpool::pause(){ 
    std::cout<<"pause p_flag: "<<pause_flag<<std::endl;
    pause_flag=true;
    std::cout<<"pause p_flag: "<<pause_flag<<std::endl;
}
void thpool::resume(){
    pause_flag=false;
}

