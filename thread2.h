#include<iostream>
#include<list>
#include<vector>
#include<functional>
#include<pthread.h>
#include<signal.h>
#include "sig_wrapper.h"
#include <unistd.h> //sleeep()
//thread
class thread{
    using do_t =std::function<void(void)>;
public:
    void init(do_t  _do);
    void start();
    void pause();
    void resume();
private:
    pthread_t id;
    volatile bool pause_flag; 

    std::function<void(void)>_do;
    void thread_do(); //调用_do
    static void* thread_do_helper(void *arg);
    void pause_do(int sig);
};
