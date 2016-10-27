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
thread_pool::thread_pool(int n):_n(n){
}
void thread_pool::init(){
	keep_working=true;
	_threads_id.resize(_n);	
	//pthread_mutex_init(_mutex);
	_job_q.init();
	
}
void thread_pool::start(){
	for(int i=0;i<_n;i++){
		pthread_create(&_threads_id[i],NULL,&thread_pool::_threads_do_helper,this);
		pthread_detach(_threads_id[i]);
	}
}
void thread_pool::stop(){
	keep_working=false;
}  
void thread_pool::add_work(std::function<void(void)> work){
	_job_q.push(work);
	_job_q.has_jobs.signal();
}

void thread_pool::_threads_do(){
 	while(keep_working){
		_job_q.has_jobs.wait();
		std::function<void(void)> job=_job_q.pull();
		if(job){
			job();	
		}
		sleep(1);
	}
}
void* thread_pool::_threads_do_helper(void *context){
	((thread_pool*)context)->_threads_do();
}
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
#if 1 
//thpool
thpool::thpool(int _n):n(_n){
}
void thpool::add_work(std::function<void(void)>work){
}
//jobs
void my_job(int i){
    while(true){
        std::cout<<"jobs : "<<i<<std::endl;
        sleep(1);
    }
}
int main(){
    #if 1 
	auto f1=std::bind(my_job,1);
	auto f2=std::bind(my_job,2);
	thread_pool th(3);
	th.init();
	th.add_work(f2);
	th.add_work(f1);
	th.add_work(f2);
	th.add_work(f1);
	th.add_work(f2);
	th.start();
    #endif
    #if 0
	auto f1=std::bind(my_job,1); //function<void(void)>
	auto f2=std::bind(my_job,2);
    
    std::vector<thread> vt;
    vt.resize(2);
    vt[0].init(f1);
    vt[1].init(f2);
    for(auto it=vt.begin();it!=vt.end();it++){
        it->start();
        it->pause();
        sleep(2);
        it->resume();
    }
    #endif
 	while(true){

	}
}
#endif
