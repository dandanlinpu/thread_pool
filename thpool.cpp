#include "thpool.h"
thread_pool::thread_pool(int _n){
    fetchjob_flag=true;
    for(int i=0;i<_n;i++){
        ths.push_back(thread(std::bind(&thread_pool::ths_do,this,i+1)));
    }
}
void thread_pool::start(){
    for(int i=0;i<ths.size();i++){
        ths[i].start();
    }
}
void thread_pool::stop_fetchjob(){
    fetchjob_flag=false;
}
void thread_pool::pause(){
    for(int i=0;i<ths.size();i++){
        ths[i].pause();
    }
}
void thread_pool::resume(){
    for(int i=0;i<ths.size();i++){
        ths[i].resume();
    }
}
void thread_pool::add_work(job_t job){
    job_q.push(job);
}
void thread_pool::wait(){
}
void thread_pool::destroy(){

}
void thread_pool::ths_do(int i){
    std::cout<<"thread I: "<<i<<std::endl;
    while(fetchjob_flag){
        std::cout<<"thread I: "<<i<<" fetch job"<<std::endl;
        job_t job=job_q.pull();
        if(job!=nullptr) job(); 
    }
}
#if 1
int main(){
    auto job=[](int i){
        int count=0;
        while(count<4){
        std::cout<<i<<std::endl;
        count++;
        sleep(1);
        }
    };
    auto f1=std::bind(job,1);
    auto f2=std::bind(job,2);
    auto f3=std::bind(job,3);
    auto f4=std::bind(job,4);
    auto f5=std::bind(job,5);
    auto f6=std::bind(job,6);
    
    thread_pool th(2);
    th.add_work(f1);
    th.add_work(f2);
    th.add_work(f3);
    th.add_work(f4);
    th.start();
    sleep(2);
    th.pause();
    sleep(5);
    th.resume();
    sleep(10);
    th.add_work(f5);
    th.add_work(f6);
    //th.stop_fetchjob();
    while(1){}
    
}
#endif
