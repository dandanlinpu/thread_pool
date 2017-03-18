#include "thread2.h"
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
        std::cout<<"pause.."<<std::endl;
    }
}

void my_job(int i){
    std::cout<<"jobs : "<<i<<std::endl;
    sleep(2);
}
int main(){
	auto f1=std::bind(my_job,1); //function<void(void)>
	auto f2=std::bind(my_job,2);
    
    std::vector<thread> vt;
    vt.resize(2);
    vt[0].init(f1);
    vt[1].init(f2);
    for(auto it=vt.begin();it!=vt.end();it++){
        it->start();
        sleep(2);
        it->pause();
        //it->resume();
    }

 	while(true){

	}



}
