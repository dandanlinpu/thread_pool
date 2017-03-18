#ifndef __SIG_WRAPPER__H
#define __SIG_WRAPPER__H
#include<iostream>
#include<functional>
#include<signal.h>
#include<pthread.h>
#include<map>
class sig_wrapper{
public:
    sig_wrapper(int id,int _s,std::function<void(int)> _f);
    void set_sig(int s);
    void set_handler(std::function<void(int)> h);
    void reg();
private:
    struct sigaction act;
    int sig;
    static std::function<void(int)> handler;
    static void call_handler(int sig);
private:
    using handler_t = std::function<void(int)>;
    static std::map<int,int> handler_map;
};
std::map<int,int> sig_wrapper::handler_map;
#endif
