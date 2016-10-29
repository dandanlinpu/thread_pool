#include <iostream>
#include <list>
#include <functional>
#include "sem.h"
class jobqueue{
    using job_t=std::function<void(void)>; 
public:
	jobqueue();
	void push(job_t); //push get是线程安全的
	job_t pull();
	void printinfo();
private:
    sem  has_jobs;
	std::list<job_t> job_list;
};
