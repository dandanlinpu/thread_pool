#ifndef __THREAD__POOL_H
#define __THREAD__POOL_H
#include "jobqueue.h"
#include "thread.h"
#include <unistd.h>
#include <iostream>
#include <vector>
class thread_pool{
    using job_t = std::function<void(void)>;
public:
    thread_pool(int _n=3);
    void start();
    void stop_fetchjob();
    void pause();
    void resume();
    void add_work(job_t);
    void wait(); //todo
    void destroy(); //todo
private:
    jobqueue job_q;
    std::vector<thread> ths;
    void ths_do(int);
    
    volatile bool fetchjob_flag;
};
#endif
