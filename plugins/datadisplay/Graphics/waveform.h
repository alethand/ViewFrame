/*
 * 波形图 --兼职处理超量数据
 *      /todo 以后遇到其它的图形也需要处理超量数据时，可以提升到DiagramSuper父类中
 *      超量数据处理说明：超量数据默认是随时间助词发送过来的数据。
 *                     可能存在某一个时段内有很多数据，有的时段内没有任何数据
 *      \warning 使用须知 传送过来的大数据链表中，每个1级list中的子项应发送时间间隔相同（允许存在空list子项
 *                       每个list子项的内容为同一时间段内传送过来，该事件段-应为传送周期
 *                      且传送过来的数据应为new出来的空间
 *
 */
#ifndef WAVEFORM_H
#define WAVEFORM_H

#include "scatterdiagram.h"
#include <QSplineSeries>    //曲线图类
#include <QString>
#include <QAbstractAxis>
#include <QAbstractTableModel>
#include <QList>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QString>
#include <QListIterator>
#include <QPair>
#include <QVector>
#include <QThread>

#include <core/protocol/datastruct.h>
#include "superparent.h"

namespace Diagram {

/*!
 * \brief The WaveForm class 波形图
 * \destnation 为了处理大型数据 --如100w
 * \attention 仅用于事后数据绘制
 */
class WaveForm:virtual public DiagramSuper,virtual public QThread
{
    Q_OBJECT
    typedef QVector<ushort>::iterator  Ite_U8List;
    enum DataSize{
        Huge,
        Normal
    };
    struct DataSrc{   //数据源
        union {
                QList<QList<ushort> *> *huge;           //超量数据
                QList<QPair<QVariant,ushort> > *normal;//普通数据
        }data;
       DataSize size;//数据大小
       DataSrc(){
           data.huge = NULL; size = Normal;
       }
    };
    void run(){}
public:
     explicit WaveForm(QString name);

     //**********  传入数据源  ***********/
     void useDataSrc(QList<QPair<QVariant,ushort>> *normalData);
     void useDataSrc(QList<QList<ushort> *>  *hugeData);

     void setShortCutVal_MaxCount(ulong num);                        //设置取值阈值

    /* QPair<Ite_SelectedData,Ite_SelectedData>
        getSelectedData_Iterator();                        */          /*! 获取选中范围的数据 */
protected:
     void insertValDraw(int recordIndex,int comingInterval);         /*! 绘制曲线图 */
     void wheelEvent(QWheelEvent *event);                           /*! 放大缩小功能 */
     void dragMoveEvent(QDragMoveEvent *event);                     /*! 拖拽功能 */
     void mouseReleaseEvent(QMouseEvent *event)
     {event->accept();
     }

      virtual void initDefault();
      virtual void atttachToSeries();
private:
     DataSrc dataSrc;

     QSplineSeries series;
     QVector<ushort> shortCutVal_IntervalRecords;   ///取值间隔记录表
     QVector<ushort> interVal_JumpRecord;         ///间隔跃迁记录表
     ulong shortCutVal_MaxNum;                    ///取值最大个数

private:
     ulong figureDataSum();                                           //计算数据量
     double getClickPointScale_At
         (Qt::Orientation dir,QPointF clickP);                      //获取点击所占比例
     void set_ShortCutValRecords
       (Ite_U8List start,Ite_U8List end,int val);                   //设置取值记录表

     bool ifHasInserted(int curInterval,int curIndex);                           //判断当前间隔是否已插入

     QVariant allocatDataValue(QVariant start,QVariant interval,double percent);
};

}

#endif // WAVEFORM_H
