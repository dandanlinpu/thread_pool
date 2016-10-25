#include<iostream>
#include<functional>
#include<signal.h>
#include<pthread.h>
class sig_wrapper{
public:
    sig_wrapper(int _s,std::function<void(int)> _f){
        sig=_s;
        handler=_f;
    }
    void set_sig(int s){
        sig=s;
    }
    void set_handler(std::function<void(int)> h){
        handler=h; 
    }
    void reg(){
        struct sigaction act;
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;
        act.sa_handler=call_handler;
        if(sigaction(sig,&act,NULL)==-1){
            fprintf(stderr,"reg sig failed !\n");
        }
    }
private:
    static std::function<void(int)> handler;
    int sig;
    static void call_handler(int sig){
        handler(sig);    
    }
};
std::function<void(int)> sig_wrapper::handler=nullptr;

#define TEST
#ifdef TEST
class test{
public:
    test(){
        var=1;
        sig_wrapper sig(SIGUSR1,std::bind(&test::catch_sigal,this,std::placeholders::_1));
        //sig.set_sig(SIGUSR1);
        //sig.set_handler(std::bind(&test::catch_sigal,this,std::placeholders::_1));
        sig.reg();
    }
    void send_sigal(pthread_t id){
        pthread_kill(id,SIGUSR1);
    }
    void catch_sigal(int sig){
        var=3;
        std::cout<<"catch_sigal change var "<<var<<std::endl;
    }    
    int var;
};
int main(){
   test t;
   t.send_sigal(pthread_self());
}
#endif
