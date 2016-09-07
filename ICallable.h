#ifndef _CALLABLE_H_
#define _CALLABLE_H_

/*
* Author: Tyler
* Email: houseyeah@gmail.com
* Date: 2014/02/14
* Modify History:
*/

#include <iostream>

using namespace std;

/* function pointer呼叫工具。
 * T: 呼叫的函式的回傳值。
 */
template <typename U>
class ICallable
{
public:
    virtual ~ICallable() { cout<<"ICallable destruct"<<endl; }
    virtual U operator() (void* args) = 0;
};

//一般函式使用。
class CallableFreeFunction : public ICallable<int>
{
public:
    CallableFreeFunction(int (*func)(void*)) : func_(func) {}

    virtual int operator() (void* args) { return (*func_)(args); }

private:

    int (*func_)(void*);
};

/* class member function使用。
 * tClass: 呼叫的函式的物件指標型別
 * T: 呼叫的函式的回傳值。
 */
template <typename tClass, typename U>
class ClassMemberCallable : public ICallable<U>
{
public:
    ClassMemberCallable(tClass* instance, U (tClass::*memberfunction)(void*)) : instance_(instance), memberfunc_(memberfunction) {}

    virtual U operator() (void* args) { return (instance_->*memberfunc_)(args); }

private:
    tClass* instance_;
    U (tClass::*memberfunc_)(void*);
};

#endif //_CALLABLE_H_