#ifndef COMMONALGORITHMS_H
#define COMMONALGORITHMS_H
#include <QThread>

/*!
 *      命名空间-计算器类
 *      用来解决大量数据的计算问题
 */
namespace Calculator
{

    class Super:public QThread
    {
        Q_OBJECT
     public:
        explicit Super();
        void append(double value);
        void reSet(){result = 0;}

        virtual void run()=0;

    public:
        volatile double result;         //计算器计算结果
        volatile double temp;           //临时变量

    protected:
        void start();

    };

    class MaxValue:public Super
    {
        Q_OBJECT
     public:
        explicit MaxValue():Super(){}
        virtual void  run();
    };


    class MinValue:public Super
    {
      Q_OBJECT
      public:
          MinValue():Super(){result = INT_MAX ;}
          virtual void run();
    };


    class MeanValue:public Super
    {
      Q_OBJECT
    public:
        MeanValue(QList<double> *p = NULL):Super(){ pdataSrc = p;}
        MeanValue(QList<int> *p):Super(){ pdataSrc = p;}

        virtual void run();

    private:
        void *pdataSrc;      //数据源指针
    };


    class Variance:public Super
    {
      Q_OBJECT
    public:
        Variance(QList<double> *p = NULL):Super(){ pdataSrc = p;}
        Variance(QList<int> *p):Super(){ pdataSrc = p;}

        virtual void run();

    private:
        void *pdataSrc;      //数据源指针
    };


}


#endif // COMMONALGORITHMS_H
