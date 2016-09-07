// FSMTest.cpp : 定義主控台應用程式的進入點。
//

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "stdafx.h"
#include "FsmState.h"
#include "Constant.h"
#include "FiniteStateMachine.h"
#include "FsmManager.h"
#include "GameLogic.h"

GameLogic::GameLogic()
{
    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    FiniteStateMachine<GameStateType> *fsm = new FiniteStateMachine<GameStateType>();

    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_GameInit), "Enter", new ClassMemberCallable<GameLogic, void>(this, &GameLogic::Execute), NULL, new ClassMemberCallable<GameLogic, GameStateType>(this, &GameLogic::ExitCondition));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_GameWait), "Enter", NULL, new ClassMemberCallable<GameLogic, bool>(this, &GameLogic::EnterCondition));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_NGReady));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_NGDeal));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_NGDraw));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_NGWin));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_GetPoint));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_GameBalance));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_RoundEnd));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_DGSelect));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_DGChoose));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_DGWin));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_GameOver));
    fsm->RegisterState((IFsmState<GameStateType> *)new FsmState<GameStateType>(State_RoundEnd));

    fsm->SetupTransitionRelationship(State_GameInit, State_GameWait);
    fsm->SetupTransitionRelationship(State_GameWait, State_NGReady);
    fsm->SetupTransitionRelationship(State_NGReady, State_NGDeal);
    fsm->SetupTransitionRelationship(State_NGDeal, State_NGDraw);
    fsm->SetupTransitionRelationship(State_NGDraw, State_NGWin);
    fsm->SetupTransitionRelationship(State_NGDraw, State_GameOver);
    fsm->SetupTransitionRelationship(State_NGWin, State_GetPoint);
    fsm->SetupTransitionRelationship(State_NGWin, State_DGSelect);
    fsm->SetupTransitionRelationship(State_DGSelect, State_DGChoose);
    fsm->SetupTransitionRelationship(State_DGChoose, State_DGWin);
    fsm->SetupTransitionRelationship(State_DGChoose, State_GameOver);
    fsm->SetupTransitionRelationship(State_DGWin, State_GetPoint);
    fsm->SetupTransitionRelationship(State_DGWin, State_DGSelect);
    fsm->SetupTransitionRelationship(State_GetPoint, State_GameOver);
    fsm->SetupTransitionRelationship(State_GameOver, State_GameBalance);
    fsm->SetupTransitionRelationship(State_GameBalance, State_RoundEnd);
    fsm->SetupTransitionRelationship(State_RoundEnd, State_GameWait);

    fsm->SetupExecuteMethod(State_GameInit, "ExecuteTest", new ClassMemberCallable<GameLogic, void>(this, &GameLogic::ExecuteTest));
    fsm->Start(State_GameInit);
    int a = 5;
    fsm->Execute("ExecuteTest", &a);

    fsm->TransitionSuccess = new ClassMemberCallable<GameLogic, void>(this, &GameLogic::TransitionSuccess);
    fsm->TransitionError = new ClassMemberCallable<GameLogic, void>(this, &GameLogic::TransitionError);

    //fsm->Transit();

/*
    for(int i = 0; i < 100; i++)
    {
        fsm->Transit(State_GameWait);
        fsm->Transit(State_NGReady);
        fsm->Transit(State_NGDeal);
        fsm->Transit(State_NGDraw);
        fsm->Transit(State_GameOver);
        fsm->Transit(State_GameBalance);
        fsm->Transit(State_RoundEnd);

        fsm->Transit(State_GameWait);
        fsm->Transit(State_NGReady);
        fsm->Transit(State_NGDeal);
        fsm->Transit(State_NGDraw);
        fsm->Transit(State_NGWin);
        fsm->Transit(State_GetPoint);
        fsm->Transit(State_GameOver);
        fsm->Transit(State_GameBalance);
        fsm->Transit(State_RoundEnd);

        fsm->Transit(State_GameWait);
        fsm->Transit(State_NGReady);
        fsm->Transit(State_NGDeal);
        fsm->Transit(State_NGDraw);
        fsm->Transit(State_NGWin);
        fsm->Transit(State_DGSelect);
        fsm->Transit(State_DGChoose);
        fsm->Transit(State_GameOver);
        fsm->Transit(State_GameBalance);
        fsm->Transit(State_RoundEnd);

        fsm->Transit(State_GameWait);
        fsm->Transit(State_NGReady);
        fsm->Transit(State_NGDeal);
        fsm->Transit(State_NGDraw);
        fsm->Transit(State_NGWin);
        fsm->Transit(State_DGSelect);
        fsm->Transit(State_DGChoose);
        fsm->Transit(State_DGWin);
        fsm->Transit(State_GetPoint);
        fsm->Transit(State_GameOver);
        fsm->Transit(State_GameBalance);
        fsm->Transit(State_RoundEnd);

        fsm->Transit(State_GameWait);
        fsm->Transit(State_NGReady);
        fsm->Transit(State_NGDeal);
        fsm->Transit(State_NGDraw);
        fsm->Transit(State_NGWin);
        fsm->Transit(State_DGSelect);
        fsm->Transit(State_DGChoose);
        fsm->Transit(State_DGWin);
        fsm->Transit(State_DGSelect);
        fsm->Transit(State_DGChoose);
        fsm->Transit(State_GameOver);
        fsm->Transit(State_GameBalance);
        fsm->Transit(State_RoundEnd);
    }
*/
    delete fsm;
}

void GameLogic::ExecuteTest(void *arg)
{
    cout<<"ExecuteTest "<<(*(int *)(arg))<<endl; 
};

bool GameLogic::EnterCondition(void *)
{
    return true;
}

GameStateType GameLogic::ExitCondition(void *)
{
    return State_GameWait;
}

void GameLogic::Execute(void *)
{
    int i = 0;
}


int _tmain(int argc, _TCHAR* argv[])
{
    /*
    FsmManager *fsmManager = FsmManager::Instance();

    fsmManager->CreateStateMachine<GameStateType>(0);
    fsmManager->CreateStateMachine<GameStateType>(1);
    */
    //FiniteStateMachine<GameStateType> *fsm = fsmManager->CreateStateMachine<GameStateType>(0);

    _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

    GameLogic *game = new GameLogic();

    delete game;
    //IFsmState<int> *init = (IFsmState<int> *)new InitState<int>((int)Init);

    int i = 0;
    cin>>i;

	return 0;
}
