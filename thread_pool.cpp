#include "thread_pool.h"
#include <unistd.h>
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
void jobqueue::push(function<void(void)> job){
	pthread_mutex_lock(&_rwmutex);
	_joblist.push_back(job);
	pthread_mutex_unlock(&_rwmutex);
}
function<void(void)> jobqueue::get(){
	function<void(void)> r;
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
	cout<<"job queue size: "<<_joblist.size()<<endl;
}
//thread_pool
thread_pool::thread_pool(int n):_n(n){
}
void thread_pool::init(){
	keep_working=true;
	_threads_id.resize(_n);	
	//pthread_mutex_init(_mutex);
	_job_q.init();
	_done_alljobs.init();
	
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
void thread_pool::add_work(function<void(void)> work){
	_job_q.push(work);
	_job_q.has_jobs.signal();
}

void thread_pool::_threads_do(){
	cout<<"new threads"<<endl;
 	while(keep_working){
		cout<<"keep_working.."<<endl;
		_job_q.has_jobs.wait();
		function<void(void)> job=_job_q.get();
		if(job){
			job();	
		}
		cout<<"after working.."<<endl<<endl;
		sleep(1);
	}
}
void* thread_pool::_threads_do_helper(void *context){
	((thread_pool*)context)->_threads_do();
}
//jobs
void my_job(int i){
	cout<<"jobs : "<<i<<endl;
}
int main(){
	auto f1=bind(my_job,1);
	auto f2=bind(my_job,2);
	thread_pool th(3);
	th.init();
	th.add_work(f2);
	th.add_work(f1);
	th.add_work(f2);
	th.add_work(f1);
	th.add_work(f2);
	th.start();
	
 	while(true){
	}
}
