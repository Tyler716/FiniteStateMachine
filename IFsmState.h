#ifndef _IFSMSTATE_H_
#define _IFSMSTATE_H_

/*
 * Author: Tyler
 * Email: houseyeah@gmail.com
 * Date: 2014/02/14
 * Modify History:
 */

#include "stdafx.h"
#include "Constant.h"
#include "ICallable.h"
#include <string>

using namespace std;

//�������A�������C
template<typename T>
class IFsmState
{
protected:
    //���A�ȡC
    T StateValue;

public:
    //�i�J����P�_��k�C
    ICallable<bool> *EnterCondition;

    //���}����P�_��k�C
    ICallable<T> *ExitCondition;

public:
    /* �[�J���A�������k�C
     * [Parameters]
     * action: �����k���ѧO�r��C
     * executeMethod: �����k�C
     */
    virtual void AddExecuteMethod(string action, ICallable<void> *executeMethod) = 0;

    /* �]�w�P�_�O�_�i�H�i�J���A����k�C
     * [Parameters]
     * enterCondition: �P�_��k�C
     */
    virtual void SetEnterConditionMethod(ICallable<bool> *enterCondition) = 0;

    /* �]�w���}�����A�A�P�_�ಾ�i�J�U�@�Ӫ��A����k�C
     * [Parameters]
     * enterCondition: �P�_��k�C
     */
    virtual void SetExitConditionMethod(ICallable<T> *exitCondition) = 0;

    /* �����������k�C
     * [Parameters]
     * action: �����k���ѧO�r��C
     * args:����ѼơC
     */
    virtual void Execute(string action = "Enter", void *args = NULL) = 0;

    /* ���������k�C
     * [Parameters]
     * action: �����k���ѧO�r��C
     */
    virtual void RemoveExecuteMethod(string action) = 0;

    T GetCurState() { return StateValue; };

    virtual ~IFsmState(){};
};
#endif //_IFSMSTATE_H_