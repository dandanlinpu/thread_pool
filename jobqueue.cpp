#include "jobqueue.h"
//jobqueue
jobqueue::jobqueue(){
}
void jobqueue::push(job_t job){
    auto push_f=[&](){
        job_list.push_back(job);
    };
    has_jobs.signal(push_f); //job job_list
}

jobqueue::job_t jobqueue::pull(){
	job_t r;
    has_jobs.wait([&](){ //r job_list
        r=job_list.front();
        job_list.pop_front();
    });
    return r;
}
void jobqueue::printinfo(){
	std::cout<<"job queue size: "<<job_list.size()<<std::endl;
}

#define test
#ifdef test
void test1(){
    auto job=[](int i){std::cout<<i<<std::endl;};
    jobqueue jobq;
    //jobq.push(std::bind(job,0));
    auto p = jobq.pull();
    p();
}
void test2(){
    auto job=[](int i){std::cout<<i<<std::endl;};  //相当于先定义函数，然后bind打包
    jobqueue jobq;
    jobq.push(std::bind(job,0));
    jobq.push(std::bind(job,1));
    auto p = jobq.pull();
    p();
    p = jobq.pull();
    p();
    jobq.push(std::bind(job,2));
    p = jobq.pull();
    p();
    
    p = jobq.pull();
    p();
}
int main(){
    test2();
}
#endif
