#ifndef _FINITESTATEMACHINE_H_
#define _FINITESTATEMACHINE_H_

/*
 * Author: Tyler
 * Email: houseyeah@gmail.com
 * Date: 2014/02/14
 * Modify History:
 */

#include "IFsmState.h"
#include <map>
#include <list>

/* 有限狀態機。
 * T: 狀態值型別。
 */
template<typename T>
class FiniteStateMachine
{
private:
    map<T, IFsmState<T>* > _stateMap;
    list<pair<T, T> > _transitionList;

    void *_conditionReference;

    IFsmState<T> *_currentStateObject;
    IFsmState<T> *_previousStateObject;

public:
    //狀態轉移成功觸發事件。
    ICallable<void> *TransitionSuccess;

    //狀態轉移失敗觸發事件。
    ICallable<void> *TransitionError;

    FiniteStateMachine()
    {
        _conditionReference = NULL;
        _currentStateObject = NULL;
        _previousStateObject = NULL;

        TransitionSuccess = NULL;
        TransitionError = NULL;
    }

    ~FiniteStateMachine()
    {
        if(TransitionSuccess != NULL)
        {
            delete TransitionSuccess;
        }

        if(TransitionError != NULL)
        {
            delete TransitionError;
        }

        for(map<T, IFsmState<T>* >::iterator it = _stateMap.begin(); it != _stateMap.end(); ++it)
        {
            delete it->second;
        }
        _stateMap.clear();
    }

    //取得目前狀態。
    IFsmState<T>* GetCurrentState()
    {
        if(_currentStateObject)
        {
            return _currentStateObject;
        }

        return NULL;
    }

    //取得前一個狀態。
    IFsmState<T>* GetPreviousState()
    {
        if(_previousStateObject)
        {
            return _previousStateObject;
        }

        return NULL;
    }

    /* 註冊狀態。
     * [Parameters]
     * stateObject: 狀態的物件。
     * action: 執行方法的識別字串。
     * executeMethod: 執行方法。
     * enterCondition: 設定判斷是否可以進入狀態的方法。
     * exitCondition: 設定離開本狀態，判斷轉移進入下一個狀態的方法。
     */
    bool RegisterState(IFsmState<T> *stateObject, string action = "Enter" , ICallable<void> *executeMethod = NULL, ICallable<bool> *enterCondition = NULL, ICallable<T> *exitCondition = NULL)
    {
        if(_stateMap.find(stateObject->GetCurState()) != _stateMap.end())
        {
            delete stateObject;
        }
        else
        {
            pair<map<T, IFsmState<T>*>::iterator, bool> Insert_Pair = _stateMap.insert(make_pair(stateObject->GetCurState(), stateObject));

            if(Insert_Pair.second)
            {
                _stateMap[stateObject->GetCurState()]->AddExecuteMethod(action, executeMethod);
                _stateMap[stateObject->GetCurState()]->SetEnterConditionMethod(enterCondition);
                _stateMap[stateObject->GetCurState()]->SetExitConditionMethod(exitCondition);

                return true;
            }
            else
            {
                delete stateObject;
            }
        }
        
        return false;
    }

    /* 建立狀態機關聯。
     * [Parameters]
     * stateValueEntire: 目前狀態。
     * stateValueEnd: 可轉移狀態。
     */
    void SetupTransitionRelationship(T stateValueEntire, T stateValueEnd)
    {
        _transitionList.push_back(make_pair(stateValueEntire, stateValueEnd));
    }

    /* 建立執行方法。
     * [Parameters]
     * stateValue: 執行此方法的狀態。
     * action: 執行方法的識別字串。
     * executeMethod: 執行方法。
     */
    void SetupExecuteMethod(T stateValue, string action = "Enter" , ICallable<void> *executeMethod = NULL)
    {
        if(_stateMap.find(stateValue) != _stateMap.end() && executeMethod != NULL)
        {
            _stateMap[stateValue]->AddExecuteMethod(action, executeMethod);
        }
    }

    /* 設定初始狀態。
     * [Parameters]
     * initStateValue: 初始狀態。
     */
    void Start(T initStateValue)
    {
        if (_stateMap.find(initStateValue) != _stateMap.end())
        {
            SetCurrentState(initStateValue);
        }
    }

    void Transit()
    {
        if(_currentStateObject->ExitCondition != NULL)
        {
            T nextState = (*(_currentStateObject->ExitCondition))(NULL);

            if(_stateMap.find(nextState) != _stateMap.end())
            {
                Transit(nextState);
            }
        }
    }

    /* 狀態轉移。
     * [Parameters]
     * initStateValue: 要轉移的狀態。
     */
    void Transit(T stateValue)
    {
        if(_stateMap.find(stateValue) != _stateMap.end())
        {
            pair<T, T> transitPair = make_pair(_currentStateObject->GetCurState(), stateValue);
            
            bool isLegal = false;

            for(list<pair<T, T> >::iterator it = _transitionList.begin(); it != _transitionList.end(); ++it)
            {
                if(*it == transitPair)
                {
                    isLegal = true;
                    break;
                }
            }

            if(!isLegal)
            {
                if(TransitionError != NULL)
                {
                    cout<<_currentStateObject->GetCurState()<<" transitionError to "<<stateValue<<endl;
                    (*TransitionError)(NULL);
                }
            }
            else
            {
                bool canExit = false;
                bool canEnter = false;

                _previousStateObject = _currentStateObject;

                if(_previousStateObject->ExitCondition != NULL)
                {
                    if((*(_previousStateObject->ExitCondition))(NULL))
                    {
                        canExit = true;
                    }
                }
                else
                {
                    canExit = true;
                }

                if(_stateMap[stateValue]->EnterCondition != NULL)
                {
                    if((*(_stateMap[stateValue]->EnterCondition))(NULL))
                    {
                        canEnter = true;
                    }
                }
                else
                {
                    canEnter = true;
                }
                    
                if(canExit && canEnter)
                {
                    if(TransitionSuccess != NULL)
                    {
                        cout<<_previousStateObject->GetCurState()<<" transitionSuccess to "<<stateValue<<endl;
                        (*TransitionSuccess)(NULL);
                    }

                    SetCurrentState(stateValue);                    
                }
                else
                {
                    if(TransitionError != NULL)
                    {
                        cout<<_currentStateObject->GetCurState()<<" transitionSuccess to "<<stateValue<<endl;
                        (*TransitionError)(NULL);
                    }
                }
            }
        }
        else
        {
            if(TransitionError != NULL)
            {
                cout<<"No State "<<stateValue<<endl;
                cout<<_currentStateObject->GetCurState()<<" transitionError to "<<stateValue<<endl;
                (*TransitionError)(NULL);
            }
        }
    }

    /* 執行對應的方法。
     * [Parameters]
     * action: 執行方法的識別字串。
     * args: 執行方法要帶入的參數。
     */
    void Execute(string action, void *args = NULL)
    {
        if (_currentStateObject != NULL)
        {
            _currentStateObject->Execute(action, args);
        }
    }

protected:
    /* 設定目前的狀態值。
     * [Parameters]
     * stateValue: 狀態值。
     */
    void SetCurrentState(T stateValue)
    {
        if(_stateMap.find(stateValue) != _stateMap.end())
        {
            Execute("Exit");
            _currentStateObject = _stateMap[stateValue];
            Execute("Enter");
        }
        else
        {
            if(TransitionError != NULL)
            {
                cout<<_currentStateObject->GetCurState()<<" transitionSuccess to "<<stateValue<<endl;
                (*TransitionError)(NULL);
            }
        }
    }
};

#endif //_FINITESTATEMACHINE_H_