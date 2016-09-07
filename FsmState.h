#ifndef _FSMSTATE_H_
#define _FSMSTATE_H_

/*
 * Author: Tyler
 * Email: houseyeah@gmail.com
 * Date: 2014/02/14
 * Modify History:
 */

#include "IFsmState.h"
#include <iostream>
#include <map>

/* 有限狀態機狀態物件基本型別。
 * T: 狀態值型別。
 */
template<typename T>
class FsmState : public IFsmState<T>
{
private:
    map<string, ICallable<void> *> _executeMethodMap;

public:
    FsmState(T state)
    {
        StateValue = state;

        EnterCondition = NULL;
        ExitCondition = NULL;
    }

    virtual ~FsmState()
    {
        if(EnterCondition != NULL)
        {
            delete EnterCondition;
        }

        if(ExitCondition != NULL)
        {
            delete ExitCondition;
        }
        ClearMethod();
    }

	virtual void AddExecuteMethod(string action, ICallable<void> *executeMethod)
    {
        //pair<std::string, EXECUTEMETHOD_FUNC> item(action, executeMethod);
        pair<map<string, ICallable<void> *>::iterator, bool> Insert_Pair = _executeMethodMap.insert(make_pair(action, executeMethod));

        if(!Insert_Pair.second)
        {
            //TODO: 加入執行方法失敗
        }
    }

    virtual void Execute(string action = "Enter", void *args = NULL)
    {
        map<string, ICallable<void> *>::iterator iter = _executeMethodMap.find(action);

        if(iter != _executeMethodMap.end())
        {
            if(_executeMethodMap[action] != NULL)
            {
                cout<<"State "<<StateValue<<" execute "<<action<<endl;
                (*(_executeMethodMap[action]))(args);
            }
        }
    }

    virtual void RemoveExecuteMethod(string action)
    {
        map<string, ICallable<void> *>::iterator iter = _executeMethodMap.find(action);

        if(iter != _executeMethodMap.end())
        {
            delete iter->second;
            _executeMethodMap.erase(action);
        }
    }

	virtual void SetEnterConditionMethod(ICallable<bool> *enterCondition)
    {
        EnterCondition = enterCondition;
    }

    virtual void SetExitConditionMethod(ICallable<T> *exitCondition)
    {
        ExitCondition = exitCondition;
    }

private:
    void ClearMethod()
    {
        for(map<string, ICallable<void> *>::iterator it = _executeMethodMap.begin(); it != _executeMethodMap.end(); ++it)
        {
            delete it->second;
        }
        _executeMethodMap.clear();
    }
};
#endif //_FSMSTATE_H_