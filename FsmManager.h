#ifndef _FSMMANAGER_H_
#define _FSMMANAGER_H_

#include "FiniteStateMachine.h"

using namespace std;

class FsmMapBase
{
public:
    virtual ~FsmMapBase() {}

    template<typename T> map<int, FiniteStateMachine<T>*> get();
};

template<typename T>
class FsmMap : public FsmMapBase
{
public:
    FsmMap(){};
    virtual ~FsmMap()
    {
        for(map<int, FiniteStateMachine<T>* >::iterator it = _fsmMap.begin(); it != _fsmMap.end(); ++it)
        {
            delete it->second;
        }
        _fsmMap.clear();
    }
    map<int, FiniteStateMachine<T>*> get()
    {
        return _fsmMap;
    }
private:
    map<int, FiniteStateMachine<T>*> _fsmMap;
};

template<typename T> map<int, FiniteStateMachine<T>*> FsmMapBase::get()
{ return dynamic_cast<FsmMap<T>*>(this)->get(); }

class FsmManager
{
public:
    static FsmManager *Instance();

public:
    template<typename T> FiniteStateMachine<T>* CreateStateMachine(int id);

protected:
    FsmManager();
    virtual ~FsmManager();

private:
    FsmMapBase _fsmMap;
};

template<typename T>
FiniteStateMachine<T>* FsmManager::CreateStateMachine(int id)
{
    FiniteStateMachine<T> *fsm = NULL;

    map<int, FiniteStateMachine<T>* > fsmMap = _fsmMap.get<T>();

    if(fsmMap.find(id) != fsmMap.end())
    {
        fsm = fsmMap[id];
    }
    else
    {        
        pair<map<int, FiniteStateMachine<T>* >::iterator, bool> Insert_Pair = fsmMap.insert(make_pair(id, new FiniteStateMachine<T>()));

        if(Insert_Pair.second)
        {
            fsm = Insert_Pair.first->second;
        }
    }

    return fsm;
}

#endif