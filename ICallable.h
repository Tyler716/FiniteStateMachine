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

/* function pointer�I�s�u��C
 * T: �I�s���禡���^�ǭȡC
 */
template <typename U>
class ICallable
{
public:
    virtual ~ICallable() { cout<<"ICallable destruct"<<endl; }
    virtual U operator() (void* args) = 0;
};

//�@��禡�ϥΡC
class CallableFreeFunction : public ICallable<int>
{
public:
    CallableFreeFunction(int (*func)(void*)) : func_(func) {}

    virtual int operator() (void* args) { return (*func_)(args); }

private:

    int (*func_)(void*);
};

/* class member function�ϥΡC
 * tClass: �I�s���禡��������Ы��O
 * T: �I�s���禡���^�ǭȡC
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