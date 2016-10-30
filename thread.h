#ifndef __THREAD__H
#define __THREAD__H
#include <iostream>
#include <pthread.h>
#include <functional>
#include "sig_wrapper.h"
class thread{
    using do_t = std::function<void(void)>;
public:
    thread(do_t _do);
    void start();
    void pause();
    void resume();
private:  
    //thread_id
    pthread_t id;
    pthread_t thread_id();

    //pause
    volatile bool pause_flag; 
    void pause_do(int sig);
    
    //thread routine
    do_t thread_do;  
    void call_helper();
    static void* call_thread_do(void *);
 
};
#endif
