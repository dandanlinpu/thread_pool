#include<signal.h>
#include<pthread.h>
#include<functional>
#include<iostream>
using namespace std;

std::function<void(int)> ff=nullptr;
void cat(int para){
    ff(para);
};

//1.sig helper
class sig{
public:
    void reg_sig(int sig){
        struct sigaction act;
        sigemptyset(&act.sa_mask);
        act.sa_flags=0;
        act.sa_handler=call_handler;
        if((sigaction(SIGUSR1,&act,NULL)==-1)){
            fprintf(stderr,"can not handler\n"); 
        }
    }
    void set_handler(std::function<void(int)> f){
        handler=f;
    }
private:
    static std::function<void(int)> handler;
    static void call_handler(int pa){
        handler(pa);
    }
};
std::function<void(int)> sig::handler=nullptr;
class thread{
public:
    sig ss;
    thread(){
        ss.reg_sig(SIGUSR1);
        ss.set_handler(std::bind(&thread::catch_signal,this,std::placeholders::_1)); // 去除this指针的方法,连同对象一起打包，交给另一个处理类
    }
    void init(std::function<void(void)> _do){
    }
    void catch_signal(int sig){
        var =1;
        std::cout<<"catch var "<<var<<std::endl;
        std::cout<<"sig "<<sig<<std::endl;
        std::cout<<"catch pause\n"<<std::endl;
    }
    void pause(){
        var = 0;
        id=pthread_self();
        std::cout<<"pause var "<<var<<std::endl;
        std::cout<<"pause\n"<<std::endl;
        pthread_kill(id,SIGUSR1);  
    }
    pthread_t id;
    int var;
};
//2 保存static this指针,这意味着多个对象时会有问题
class Test{
public:
    Test(){
        me=this;
    }
    static void catch_signal(int sig){
        me->var =1;
        std::cout<<"catch change static var "<<me->var<<std::endl;
    }
    int var;
    static Test * me;
};
Test * Test::me=NULL;
//3.传递this指针

int main(){
    std::cout<<(SIGUSR1)<<endl;
    thread t;
    t.pause();
    
    Test t1;
    t1.catch_signal(1);
}

