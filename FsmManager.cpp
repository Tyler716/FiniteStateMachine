#include "FsmManager.h"

FsmManager::FsmManager()
{
    cout<<"constructor"<<endl;
}

FsmManager::~FsmManager()
{
    cout<<"desstructor"<<endl;

    int i;
    cin>>i;
}

FsmManager* FsmManager::Instance()
{
    static FsmManager instance;

    return &instance;
}
