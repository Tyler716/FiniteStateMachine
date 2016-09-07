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

//有限狀態機介面。
template<typename T>
class IFsmState
{
protected:
    //狀態值。
    T StateValue;

public:
    //進入條件判斷方法。
    ICallable<bool> *EnterCondition;

    //離開條件判斷方法。
    ICallable<T> *ExitCondition;

public:
    /* 加入狀態的執行方法。
     * [Parameters]
     * action: 執行方法的識別字串。
     * executeMethod: 執行方法。
     */
    virtual void AddExecuteMethod(string action, ICallable<void> *executeMethod) = 0;

    /* 設定判斷是否可以進入狀態的方法。
     * [Parameters]
     * enterCondition: 判斷方法。
     */
    virtual void SetEnterConditionMethod(ICallable<bool> *enterCondition) = 0;

    /* 設定離開本狀態，判斷轉移進入下一個狀態的方法。
     * [Parameters]
     * enterCondition: 判斷方法。
     */
    virtual void SetExitConditionMethod(ICallable<T> *exitCondition) = 0;

    /* 執行對應的方法。
     * [Parameters]
     * action: 執行方法的識別字串。
     * args:執行參數。
     */
    virtual void Execute(string action = "Enter", void *args = NULL) = 0;

    /* 移除執行方法。
     * [Parameters]
     * action: 執行方法的識別字串。
     */
    virtual void RemoveExecuteMethod(string action) = 0;

    T GetCurState() { return StateValue; };

    virtual ~IFsmState(){};
};
#endif //_IFSMSTATE_H_