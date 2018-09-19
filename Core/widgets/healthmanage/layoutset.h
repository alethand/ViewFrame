﻿/*date：2018/8/20
 *author:yww
 *file brief:根据给定参数，动态的根据协议结构体生成相应的界面
 *要求：界面大小会被动态的调整，内部布局要跟着变，数据量较多，刷新速度保证够快
 *
 *界面刷新触发条件：报文数据量更改，显示界面大小更改
 *date:工作状态与分机-组件
 * default：数据默认标签使用label,工作状态使用lineEdit，分机状态使用状态灯
 *modeName:使用设计模式--模板方法
 */

#ifndef LAYOUTSET_H
#define LAYOUTSET_H

#include <QStringList>
#include <QList>
#include <QVector>
#include <QPair>
#include <QMap>
#include <QWidget>
#include <QListWidget>
#include <QSharedPointer>

class QStackedWidget;
class QLabel;
class QLineEdit;

#include "healthstate_display.h"
#include "fileOpt.h"
#include "head.h"
#include "Base/pluginmanager/observer.h"
#include "Base/selfwidget/pageinfo.h"
#include "Base/util/fileutils.h"

#define MAX_WORKSTATE_NUMS 300                          //最大工作状态数量
#define MAX_SubMACHINE_NUMS 20                          //最大分机数量
#define MAX_COMPONENT_NUMS 20                           //最大分机组件个数

using namespace FileUtils;

using namespace HealthState;
using HealthState::HealthData;

class HealthState_Display;

class StateLamp:public QWidget
{
  Q_OBJECT
  public:
    StateLamp(QWidget* parent=0):QWidget(parent){value  = -1;this->setWindowFlags(Qt::FramelessWindowHint);}

    void setValue(int v)                                 //设置状态灯的值
    {
        this->value = v;
        this->update();
    }
private:
     short value;

protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

public:
    static void initColorMap();                         //初始化颜色映射
    static QMap<int,QColor> colorMap;
    static QColor othersColor;
};

class LayoutAndDisplay:public QWidget,public PageInfo,public Base::Observer
{
    Q_OBJECT
public:
    explicit LayoutAndDisplay(QWidget* mparent);
    virtual ~LayoutAndDisplay();
    virtual bool startLayout(HealthData &data,bool ifUse)=0;                //开始布局
    virtual void flushScreen(HealthData &data);                             //根据数据刷新界面显示

    void onMessage(MessageType::MessType type);

public slots:
    void reLayout_BySize();                                                 //根据大小重新开始布局

protected:
    virtual void upDateSwitchPageInfo(int index);

    /*!
     * \brief generateWSLayout   生成工作状态布局
     */
    void  generateWSLayout(unsigned int start =1);

    /*!
     * \brief generateSubMCpLayout  生成分机-组件布局
     */
     void generateSubMCpLayout(unsigned int index =0);
     void ensureLayoutData(const HealthData &data);                         //确定布局数据

     virtual int findMatchWKIndex(int index)=0;                             //数据刷新时工作状态数据定位函数
     virtual QPair<int,int> findMatchSubMIndexPair(int subMIndex,int compIndex)=0;      //分机组件数据定位函数

protected:
    QWidget &mparent;
    bool  isHasLayout;                       //是否已经布局

    QWidget *mWorkStateWidget;
    SwitchPage *mSwPgWidget;

    QStackedWidget *msubMachWidget;
    QListWidget *msubMachList;

    const double mItemsHeight;
    const double  wkItemWidth,subMachItemWidth;
    const  double  minRowInterval,minColumnInterval;

    QVector<QPair<QLabel*,QLineEdit*> > mWkStItems;                                    //工作状态的所有子控件
    QVector<QPair<QString,QList<QPair<QLabel*,StateLamp*> >* >>  mSubMchCmpntItems;    //分机组件的所有子控件
    QVector<QWidget*> mWKObjPair;                                                     //工作状态中--标签与行编辑器组成的控件对
    QVector<QPair<QWidget*,QList<QWidget*>* > > mSubMPair;                             //分机组件--组件所有信息的包裹控件与组件对控件列表
    unsigned short wkRowCount,wkColcount;
    unsigned short subMRowCount,subMColCount;

    QSharedPointer<HealthData> mUpdatePara;

    QTabWidget * tabWidget;

private slots:
     void choooseComponent(QModelIndex index);

private:
     void retranslateUi();

public:
     static void initColorMap();
     static QMap<int,QColor> colorMap;
     static QColor othersColor;
};
#endif // LAYOUTSET_H
