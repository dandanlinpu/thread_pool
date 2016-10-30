#include "thread_pool.h"
#include <unistd.h>
//jobs
void my_job(int i){
   while(true){
        std::cout<<"jobs : "<<i<<std::endl;
        sleep(2);
    }
}

int main(){
    #if 0 
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
    auto f0=std::bind(my_job,0);
    auto f1=std::bind(my_job,1);
    auto f2=std::bind(my_job,2);
    auto f3=std::bind(my_job,3);
    auto f4=std::bind(my_job,4);
    auto f5=std::bind(my_job,5);
    auto f6=std::bind(my_job,6);
    thpool th(2);
    th.init();
    th.add_work(f0);
    th.add_work(f1);
    th.add_work(f2);
    th.start();
    th.pause();
    th.add_work(f3);
    th.add_work(f4);
    #endif
    #if 1
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
    #endif
 	while(true){

	}
}

