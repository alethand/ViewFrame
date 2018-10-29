#include "commonalgorithms.h"

Calculator::Super::Super()
    :QThread()
{
    result = 0;
}

/*!
 * \brief Calculator::Super::append  外界调用接口
 * \param value
 */
void Calculator::Super::append(double value){
    temp = value;
    this->start();
}

/*!
 * \brief Calculator::Super::start  禁止用户手动调用start函数
 */
void Calculator::Super::start(){
    QThread::start();
}



void Calculator::MaxValue::run()
{
    if(temp > result)
        result = temp;
}

void Calculator::MinValue::run()
{
    if(temp < result)
        result = temp;
}


void Calculator::MeanValue::run()
{

}

void Calculator::Variance::run()
{

}
