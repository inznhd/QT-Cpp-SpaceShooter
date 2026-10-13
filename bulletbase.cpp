#include "bulletbase.h"

//构造函数，不允许空父类对象
BulletBase::BulletBase(QObject *parent) : QObject(parent)
{

}

//对象池直接获取状态接口
int BulletBase::getState()
{
    return state;
}

//子弹滞空函数
void BulletBase::setDeath()
{
    state=0;
    setPos(-100,-100);
}
