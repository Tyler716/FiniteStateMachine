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

/* �������A���C
 * T: ���A�ȫ��O�C
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
    //���A�ಾ���\Ĳ�o�ƥ�C
    ICallable<void> *TransitionSuccess;

    //���A�ಾ����Ĳ�o�ƥ�C
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

    //���o�ثe���A�C
    IFsmState<T>* GetCurrentState()
    {
        if(_currentStateObject)
        {
            return _currentStateObject;
        }

        return NULL;
    }

    //���o�e�@�Ӫ��A�C
    IFsmState<T>* GetPreviousState()
    {
        if(_previousStateObject)
        {
            return _previousStateObject;
        }

        return NULL;
    }

    /* ���U���A�C
     * [Parameters]
     * stateObject: ���A������C
     * action: �����k���ѧO�r��C
     * executeMethod: �����k�C
     * enterCondition: �]�w�P�_�O�_�i�H�i�J���A����k�C
     * exitCondition: �]�w���}�����A�A�P�_�ಾ�i�J�U�@�Ӫ��A����k�C
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

    /* �إߪ��A�����p�C
     * [Parameters]
     * stateValueEntire: �ثe���A�C
     * stateValueEnd: �i�ಾ���A�C
     */
    void SetupTransitionRelationship(T stateValueEntire, T stateValueEnd)
    {
        _transitionList.push_back(make_pair(stateValueEntire, stateValueEnd));
    }

    /* �إ߰����k�C
     * [Parameters]
     * stateValue: ���榹��k�����A�C
     * action: �����k���ѧO�r��C
     * executeMethod: �����k�C
     */
    void SetupExecuteMethod(T stateValue, string action = "Enter" , ICallable<void> *executeMethod = NULL)
    {
        if(_stateMap.find(stateValue) != _stateMap.end() && executeMethod != NULL)
        {
            _stateMap[stateValue]->AddExecuteMethod(action, executeMethod);
        }
    }

    /* �]�w��l���A�C
     * [Parameters]
     * initStateValue: ��l���A�C
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

    /* ���A�ಾ�C
     * [Parameters]
     * initStateValue: �n�ಾ�����A�C
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

    /* �����������k�C
     * [Parameters]
     * action: �����k���ѧO�r��C
     * args: �����k�n�a�J���ѼơC
     */
    void Execute(string action, void *args = NULL)
    {
        if (_currentStateObject != NULL)
        {
            _currentStateObject->Execute(action, args);
        }
    }

protected:
    /* �]�w�ثe�����A�ȡC
     * [Parameters]
     * stateValue: ���A�ȡC
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