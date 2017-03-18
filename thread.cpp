#include "thread.h"
#include <unistd.h>
#include <vector>
thread::thread(do_t _do){
    this->id=0;
    this->pause_flag=0;
    this->thread_do=_do;
}
void thread::start(){
    pthread_create(&id,NULL,call_thread_do,this); 
}   
void thread::pause(){ 
        if(id==0){
            fprintf(stderr,"thread is not started yet..\n");
        }else{
            std::cout<<"send pause signal to "<<id<<std::endl;
            pause_flag=1;
            pthread_kill(id,SIGUSR1);
        }
};  
void thread::resume(){
        pause_flag=0;
};  

void thread::pause_do(int sig){
    std::cout<<"pause_do "<<sig<<std::endl;
    while(pause_flag==1){
        sleep(1);
        std::cout<<"pause..."<<std::endl;
    }
}
void thread::call_helper(){
    sig_wrapper sig(SIGUSR1,std::bind(&thread::pause_do,this,std::placeholders::_1));
    sig.reg();
    
    thread_do();
}
void * thread::call_thread_do(void *arg){
    ((thread *)arg)->call_helper();
}
pthread_t thread::thread_id(){
    return id;
}
#ifdef test
int main(){
    auto job=[](int i){
        while(true){
            std::cout<<"job "<<i<<std::endl;
            sleep(1);
        }
        };
 auto   job1=std::bind(job,1);
 auto   job2=std::bind(job,2);
 auto   job3=std::bind(job,3);
 auto   job4=std::bind(job,4);
 auto   job5=std::bind(job,5);
       /* thread m(job1);
        m.start();
        m.pause();
        sleep(2);
        m.resume();*/
   std::vector<thread> th;
   th.push_back(thread(std::bind(job,1)));
   th.push_back(thread(std::bind(job,2)));
   th.push_back(thread(std::bind(job,3)));
    for(auto it=th.begin();it!=th.end();it++){
        it->start();
        sleep(2);
        it->pause();
    }
    sleep(3);
    th[0].resume();
    sleep(3);
    th[1].resume();
    sleep(5);
    th[2].resume();
    while(1){
    }

}
#endif
