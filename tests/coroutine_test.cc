#include <iostream>
#include <memory>
#include "src/zws.h"
using namespace zws;
using namespace std;
Coroutine::ptr cor;
Coroutine::ptr cor2;
void fun_2() {
    cout << "2----this is coroutine 2" << endl;;
    Coroutine::Yield();
    cout << "4----yield back, it is cor 2"<< endl;
}

int main() {
    // cor_1 就是当前main函数
    // cor_2 执行fun_2函数
    
    // char* sp = reinterpret_cast<char*>(malloc(stack_size));
    // cor = std::make_shared<zrpc::Coroutine>(stack_size, sp, fun_2);
    int stack_size = 128 * 1024;
    char* sp2 = reinterpret_cast<char*>(malloc(stack_size));
    cor2 = std::make_shared<zws::Coroutine>(stack_size, sp2, fun_2);
    cout << "1----this is main cor"<< endl; 

    Coroutine::Resume(cor2);  // 切换到cor_2

    cout << "3----resume back, this is main cor"<< endl;
    Coroutine::Resume(cor2);   // 再次切换到cor_2
    cout << "5----resume back, this is main cor"<< endl;
    return 0;



    // std::shared_ptr<int> p1(new int(1));
    // std::shared_ptr<int> p2(new int(2));
    // p1 = p2;
    // cout << (*p1) << endl;

}