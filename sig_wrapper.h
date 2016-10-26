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
    static std::function<void(int)> handler;
    static void call_handler(int sig);
};