#ifndef _CONSTANT_H_
#define _CONSTANT_H_

enum GameStateType
{
    State_GameInit,
    State_GameWait,
    State_NGReady,
    State_NGDeal,
    State_NGDraw,
    State_NGWin,
    State_GetPoint,
    State_GameBalance,
    State_RoundEnd,
    State_DGSelect,
    State_DGChoose,
    State_DGWin,
    State_GameOver,
    End
};

#endif