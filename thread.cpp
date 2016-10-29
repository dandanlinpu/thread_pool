#include "thread.h"
#include <unistd.h>
#include <vector>
thread::thread(std::function<void(void)> _out_do){
    this->thread_do=thread_do;
    this->id=0;
    pause_flag=0;
}
void thread::start(){
    pthread_create(&id,NULL,call_thread_do,NULL); 
    //为什么不在threadout_do中注册？因为有可能主线程调用start() 返回后立即调用pause(),导致线程函数中注册代码未真正完成.
    sig_wrapper sig(SIGUSR1,std::bind(&thread::pauseout_do,this,std::placeholders::_1));
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

void thread::pause_do(int sig){
    while(pause_flag==1){
        sleep(1);
    }
}
void * thread::call_thread_do(void *){
    thread_do();
}
#ifdef test
int main(){
    auto job=[](int i){
        std::cout<<"job "<<i<<endl;
        sleep(1);
        };
    std::vector<thread> th;
    th.resize(4);
    for(int i=0;i<th.size();i++){
        th[i]=thread(std::bind(job,i));
            th.start();
    }


}
#endif