#include<iostream>
using namespace std;
class A{
   typedef int myInt;
public:
    
    myInt fun(){
        std::cout<<"fun"<<std::endl;
    }
    myInt fun2();

};
A::myInt A::fun2(){
     std::cout<<"fun"<<std::endl;
}
int main(){
    cout<<"hello world"<<endl;
    A a;
    a.fun();

}