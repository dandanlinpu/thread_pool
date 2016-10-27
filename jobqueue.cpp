#include "jobqueue.h"
//jobqueue
jobqueue::jobqueue(){
	pthread_mutex_init(&_rwmutex,NULL);
}
void jobqueue::push(job_t job){
	has_jobs.signal( [&](){job_list.push_back(job);} ); //job job_list
}
job_t jobqueue::pull(){
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
int main(){
    auto job=[](int i){std::cout<<i<<std::endl;};
    jobqueue jobq;
    //jobq.push(std::bind(job,0));
    auto p = jobq.pull();
    p();
}
#endif