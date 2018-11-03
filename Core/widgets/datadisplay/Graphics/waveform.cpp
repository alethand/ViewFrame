#include "waveform.h"
#include <qmath.h>
#include <QDebug>

namespace Diagram {

WaveForm::WaveForm(QString name)
    :DiagramSuper(name),QThread()
{
    m_chart.addSeries(&series);
    m_chart.legend()->setVisible(false);

    setShortCutVal_MaxCount(pow(10,5));
    DiagramSuper::setScale(QChartView::HorizontalRubberBand);
}

/*!
 * \brief WaveForm::useDataSrc  传入正常数据
 * \param normalData
 */
void WaveForm::useDataSrc(QList<QPair<QVariant,ushort>> *normalData)
{
    dataSrc.size = Normal;
    dataSrc.data.normal = normalData;
    //insertValDraw();
}


/*!
 * \brief WaveForm::useDataSrc  使用超量数据
 * \param hugeData
 * \not 超量数据一般是由于实时发送过多数据所积累的。
 * 而且一把为ushort。其它类型--->你就认为做系统的疯了吧
 */
void WaveForm::useDataSrc(QList<QList<ushort> *> *hugeData)
{
    dataSrc.size = Huge;
    dataSrc.data.huge = hugeData;

    /* 等距离段 */
    int count = dataSrc.data.huge->count();
    DiagramSuper::setTickCount(AxisType::xAxis,count);

    /*  初始化插值记录--即每隔多少数据取一个点 */
    shortCutVal_IntervalRecords.resize(count);
    ulong sum = figureDataSum();
    ushort interval = sum/shortCutVal_MaxNum;
    set_ShortCutValRecords(shortCutVal_IntervalRecords.begin(),
                           shortCutVal_IntervalRecords.end(),interval);
    /* 初始化插值记录跃迁表 */
    while(interval) {
        interVal_JumpRecord.append(interval);
        interval = floor(interval/2) ;   //记录间隔变化 如开始间隔10个数；则为10 5 2 1
    }

    for(int i=0;i < dataSrc.data.huge->count();i++) {
        insertValDraw(i,interval);
    }

}



/*!
 * \brief WaveForm::setShortCutVal_MaxCount  获取数据的最大计数量
 * \details 因为要进行处理超量数据，上百万数据时，极短时间内无法绘出。
 *          因此设定阈值-取值最大计数，一次性最多显示num个绘制点
 * \param num   将要设置的-取值最大计数
 */
void WaveForm::setShortCutVal_MaxCount(ulong num){
    shortCutVal_MaxNum = num;
}




/*!
 * \brief WaveForm::insertValDraw 插值绘图法
 *              \note 根据需要戳你关系绘制的区域进行插值绘图
 * \param recordIndex
 * \param comingInterval  即将变成的间隔数
 */
void WaveForm::insertValDraw(int recordIndex, int comingInterval)
{
    QPointF pos;
    qint64 disatanceTime = xAxis.valMaxRange.second.toDateTime().secsTo( xAxis.valMaxRange.first.toDateTime());
    uint interval = disatanceTime/dataSrc.data.huge->count();
    ulong uIntervalCnt = dataSrc.data.huge->at(recordIndex)->count();
    for(int i = 0;i < uIntervalCnt;i++) {
        if((i %(recordIndex)) ==0 &&ifHasInserted(comingInterval,i)) {

            pos.setX( allocatDataValue(xAxis.valMaxRange.first.toDateTime().addMSecs(i*interval),interval,(double)i/uIntervalCnt).toDouble() );
            pos.setY(dataSrc.data.huge->at(recordIndex)->at(i) );
            series.append(pos);
        }
    }
}

void WaveForm::initDefault()
{

}

void WaveForm::atttachToSeries()
{
    series.attachAxis(xAxis.axis);
    series.attachAxis(yAxis.axis);
}


/*!
 * \brief WaveForm::wheelEvent  处理放大缩小
 * \param event
 * \attention  目前暂支持 vlaue值的横轴放大缩小，
 */
void WaveForm::wheelEvent(QWheelEvent *event)
{
    QPointF pos = event->posF();
    double scale  = getClickPointScale_At(Qt::Horizontal,pos);
    qDebug()<<scale;
    switch(xAxis.dataType) {
    case DataType::value: {
        double curVal = (xAxis.valCurRange.second.toDouble() - xAxis.valCurRange.first.toDouble())*scale;
        double minVal = 0,maxVal = 0;
        if(event->angleDelta().y() > 0) {
            minVal = curVal - (curVal - xAxis.valCurRange.first.toDouble())/2;
            maxVal =  curVal + (xAxis.valCurRange.second.toDouble() - curVal)/2;
        }
        else {
            minVal = curVal - (curVal - xAxis.valCurRange.first.toDouble())*2;
            maxVal =  curVal + (xAxis.valCurRange.second.toDouble() - curVal)*2;

        }
        if(minVal < xAxis.valMaxRange.first.toDouble())
            minVal = xAxis.valMaxRange.first.toDouble();
        if(maxVal > xAxis.valMaxRange.second.toDouble())
            maxVal = xAxis.valMaxRange.second.toDouble();
         xAxis.setRange(false,minVal,maxVal);

        break;
    }

    case DataType::DateTime:
        //时间再后写



        break;
    }
     event->accept();
}

void WaveForm::dragMoveEvent(QDragMoveEvent *event)
{
    //if(event->)
    m_chart.moveBy(10,0);
}

/*!
 * \brief WaveForm::figureDataSum 计算数据总量
 * \return
 */
ulong WaveForm::figureDataSum()
{
    ulong sum =0;
    if(dataSrc.size == Huge) {
        for(int i=0;i < dataSrc.data.huge->count();i++) {
            sum += dataSrc.data.huge->at(i)->count();
        }
    }
    else if(dataSrc.size == Normal) {
        sum = dataSrc.data.normal->count();
    }

    return sum;
}

/*!
 * \brief WaveForm::getClickPointScale_At  获取点击点所在方向的比例
 * \param dir    指定横纵坐标方向
 * \param clickP    点击点
 * \return          比例尺
 * \warning 现在返回的比例还有点问题
 */
double WaveForm::getClickPointScale_At(Qt::Orientation dir, QPointF clickP)
{
    QPointF chartPos = m_chart.pos();
    double wid = m_chart.size().width();
    double height = m_chart.size().height();
    if(dir == Qt::Horizontal) {
        return (clickP.x() - chartPos.x())/wid;
    }
    else if(dir == Qt::Vertical) {
        return 1 - (clickP.y() - chartPos.y())/height;
    }
}

/*!
 * \brief WaveForm::set_ShortCutValRecords  设置取值记录表
 * \param start     开始位置
 * \param end       结束位置
 * \param val       全部赋予val
 */
void WaveForm::set_ShortCutValRecords(WaveForm::Ite_U8List start, WaveForm::Ite_U8List end, int val)
{
    while(start < end) {
        *start = val;
        start++;
    }
}

/*!
 * \brief WaveForm::ifHasInserted 判断当前的序号是否已经进行插值处理
 * \param curInterval  当前间隔
 * \param curIndex     当前序号
 * \return             是否已插值
 */
bool WaveForm::ifHasInserted(int curInterval,int curIndex)
{
    for(int i=0;interVal_JumpRecord.at(i) != curInterval;i++) {
        if((curIndex % interVal_JumpRecord.at(i)) == 0) {
            return false;
        }
    }
    return true;
}


/*!
 * \brief WaveForm::allocatDataValue 分配数据的时间值
 *            目的:为了解决中频数据中，只有脉冲到达时间& n个AD数据
 *                 无法确定每一个AD数据的横坐标值
 * \param start     起始时间
 * \param timeInterval       时间间隔
 * \param percent   百分比
 * \return          计算出的时间值
 *
 * \attention \todo 应把时间更改为QVariant，实现更高的扩展性
 */
QVariant WaveForm::allocatDataValue(QVariant start, QVariant interval, double percent)
{

     if(xAxis.dataType == DataType::DateTime) {
        ulong secs = interval.toUInt()*percent;
        return start.toDateTime().time().addSecs(secs);
     }
     else  if(xAxis.dataType == DataType::value) {

         return start.toDouble() + interval.toDouble()*percent;
     }
}















}
