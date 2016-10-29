#include<iostream>
#include<functional>
using namespace std;

void fun(function<void(void)> f){
    f();
}

int main(){
    int a=0;
    int b=0;
    auto  f=[&]()mutable{
        a=1;
        cout<<1<<endl;
    };
    auto f2=f;
    fun(f2);
    cout<<"hello world"<<endl;
}