#ifndef __SIG_WRAPPER__H
#define __SIG_WRAPPER__H
#include<iostream>
#include<functional>
#include<signal.h>
#include<pthread.h>
class sig_wrapper{
public:
    sig_wrapper(int _s,std::function<void(int)> _f);
    void set_sig(int s);
    void set_handler(std::function<void(int)> h);
    void reg();
private:
    struct sigaction act;
    int sig;
    static std::function<void(int)> handler; //如果多次使用使得此wrapper用于多个类对象（如数组sig_wrapper sig[3]），那么每次此handler会被覆盖
    static void call_handler(int sig);
};
#endif