#ifndef __THREAD__H
#define __THREAD__H
#include <iostream>
#include <pthread.h>
#include <functional>
#include "sig_wrapper.h"
class thread{
    using do_t = std::function<void(void)>;
public:
    thread(do_t thread_do);
    void start();
    void pause();
    void resume();
private:
    //thread_id
    pthread_t id;
    pthread_t id();

    //pause
    volatile bool pause_flag; 
    void pause_do(int sig);
    
    //thread routine
    static do_t thread_do;  
    static void * call_thread_do(void *){
        thread_do();
    }
 
};
#endif