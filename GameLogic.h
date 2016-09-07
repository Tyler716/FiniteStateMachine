#ifndef _GAMELOGIC_H_
#define _GAMELOGIC_H_

#include <iostream>

using namespace std;

class GameLogic
{
public:
    GameLogic();
    
    bool EnterCondition(void *);

    GameStateType ExitCondition(void *);

    void Execute(void *);

    void ExecuteTest(void *);

    void TransitionSuccess(void *){ cout<<"TransitionSuccess"<<endl;};
    void TransitionError(void *){ cout<<"TransitionError"<<endl;};

};

#endif //_GAMELOGIC_H_