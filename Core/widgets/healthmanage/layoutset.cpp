#include "layoutset.h"

#include <QTabBar>
#include <QDebug>
#include <qmath.h>
#include <QPainter>
#include <QPainterPath>
#include <QStackedWidget>
#include <QLabel>
#include <QScrollArea>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QSpinBox>
#include <QLineEdit>
#include <QPaintEvent>

#include <QHeaderView>

#include "Base/util/rsingleton.h"
//test
#include <iostream>
using namespace std;
//test

QString  GetFontStyleSheetByInt(int value)
{
    QColor color;
    QMap<int,QColor>::const_iterator  ite = LayoutAndDisplay::colorMap.find(value);
    if(ite == LayoutAndDisplay::colorMap.end())
        color = LayoutAndDisplay::othersColor;
    else
        color= ite.value();

    int r,g,b;
    r=color.red();g=color.green();b=color.blue();
    QString style;
    style = QString("color:(%1,%2,%3);").arg(r).arg(g).arg(b);
    return style;
}

QMap<int,QColor> StateLamp::colorMap = QMap<int,QColor>();
QMap<int,QColor> LayoutAndDisplay::colorMap = QMap<int,QColor>();
QColor StateLamp:: othersColor = Qt::white;
QColor LayoutAndDisplay::othersColor = Qt::green;

void StateLamp::initColorMap()
{
    colorMap.clear();
    colorMap.insert(0,Qt::green);
    colorMap.insert(1,Qt::red);
    colorMap.insert(2,Qt::gray);
    othersColor = Qt::white;
}

void LayoutAndDisplay::initColorMap()
{
    colorMap.clear();
    colorMap.insert(0,Qt::black);
    colorMap.insert(1,Qt::yellow);
    colorMap.insert(2,Qt::red);
    othersColor = Qt::green;
}

LayoutAndDisplay::LayoutAndDisplay(QWidget *parent)
    :QWidget(parent),PageInfo(),mparent(*parent),
      wkItemWidth(150),subMachItemWidth(wkItemWidth -15),
      minRowInterval(10),minColumnInterval(10),mItemsHeight(50),isHasLayout(false)

{
    RSingleton<Base::Subject>::instance()->attach(this);

    initView();
    wkRowCount =0,wkColcount =0;
    subMRowCount = 0 ,subMColCount =0;

    retranslateUi();
}

LayoutAndDisplay::~LayoutAndDisplay()
{
    isHasLayout = false;

    for(int i=0;i < mWkStItems.count();i++)
    {
        mWkStItems.operator [](i).first->setParent(NULL);
        mWkStItems.operator [](i).second->setParent(NULL);
        delete mWkStItems.operator [](i).first;
        delete mWkStItems.operator [](i).second;
        mWkStItems.operator [](i).first = NULL;
        mWkStItems.operator [](i).second = NULL;
    }
    mWkStItems.clear();
    mSubMchCmpntItems.clear();
    mWKObjPair.clear();
    mSubMPair.clear();
}

void LayoutAndDisplay::flushScreen(HealthData &data)
{
    int wkPerPageItemsNum = wkRowCount*wkColcount;
    int index = 0;
    int trueIndex=0;
    for(int i=0;i < wkRowCount;i++)
    {
        for(int j =0;j<wkColcount;j++)
        {
            index = (mSwPgWidget->getCurPage() -1)* wkPerPageItemsNum+ i*wkColcount+j;
            if(i*wkColcount+j >= mWKObjPair.size())//布局允许数量比实际数量多
                break;
            if( index+1 <= mWKObjPair.size())
            {
                trueIndex =findMatchWKIndex(index);
                mWkStItems.operator [](i*wkColcount+j).second->setText(data.wsInfo.at(trueIndex).content);
                mWkStItems.operator [](i*wkColcount+j).second->setStyleSheet(GetFontStyleSheetByInt(data.wsInfo.at(trueIndex).contentColor));
            }
            else
                 mWkStItems.operator [](i*wkColcount+j).first->setText("");
        }
    }

    QPair<int,int> pair ;
    for(int i =0;i < mSubMchCmpntItems.count();i++)
    {
        for(int j=0;j < mSubMchCmpntItems.at(i).second->count();j++)
        {
            pair = findMatchSubMIndexPair(i,j);
            mSubMchCmpntItems.at(pair.first).second->at(pair.second).second->setValue(data.smInfo.at(pair.first).compInfo.at(pair.second).state);
        }
    }
}

void LayoutAndDisplay::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void LayoutAndDisplay::reLayout_BySize()
{
    //分机-组件的变化
    generateSubMCpLayout(msubMachWidget->currentIndex());

    //工作状态的变化
    int wkRow = mWorkStateWidget->height()/(mItemsHeight+minRowInterval);
    int wkCol = mWorkStateWidget->width()/(wkItemWidth+minColumnInterval);
    if(wkRow == wkRowCount && wkColcount == wkCol)
        return;
    int wkPerPageItemsNum = wkRowCount*wkColcount;
    for(int i = 0;i < wkPerPageItemsNum;i++)
    {
        if(i >= mWKObjPair.size())
            break;
        mWKObjPair.operator [](i)->setParent(NULL);
    }
    if( wkPerPageItemsNum > wkRow*wkCol )           //布局由大变小时，原先显示的多余控件应该隐藏
    {
        for(int i = wkRow*wkCol;i < wkPerPageItemsNum;i++)
        {
            if( i < mWKObjPair.size())
                mWKObjPair.operator [](i)->hide();
            else
                break;
        }
    }


    generateWSLayout(mSwPgWidget->getCurPage());


}

void LayoutAndDisplay::generateWSLayout(unsigned int start)
{
//    cout<<"thetest"<<endl;
    wkRowCount = mWorkStateWidget->height()/(mItemsHeight+minRowInterval);
    wkColcount = mWorkStateWidget->width()/(wkItemWidth+minColumnInterval);
    //1
    standItemModel->setColumnCount(wkColcount);
    standItemModel->setRowCount(wkRowCount);

    tableView->horizontalHeader()->setDefaultSectionSize(1.1*(wkItemWidth+minColumnInterval));
    tableView->verticalHeader()->setDefaultSectionSize(0.9*(mItemsHeight+minRowInterval));

    tableView->horizontalHeader()->hide();
    tableView->verticalHeader()->hide();

    tableView->setAlternatingRowColors(true);//设置变色
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置不可编辑


    //1
    int wkPerPageItemsNum = wkRowCount*wkColcount;
    QGridLayout *layout = dynamic_cast<QGridLayout*>(mWorkStateWidget->layout());
    int index = 0;
    for(int i=0;i < wkRowCount;i++)
    {
        for(int j =0;j<wkColcount;j++)
        {
            index = (start -1)* wkPerPageItemsNum+ i*wkColcount+j;
            if(i*wkColcount+j < mWKObjPair.size())
            {
                //update
                QList<QLabel*>tplabelFindOut = mWKObjPair.at(i*wkColcount+j)->findChildren<QLabel*>();
                QList<QLineEdit*>tplineEditFindOut = mWKObjPair.at(i*wkColcount+j)->findChildren<QLineEdit*>();
                QStandardItem *standItem1 = new QStandardItem(tplabelFindOut.at(0)->text()+"   "+ tplineEditFindOut.at(0)->text());
//                QStandardItem *standItem1 = new QStandardItem(dataList[i*wkColcount+j]+"   "+ tplineEditFindOut.at(0)->text());
                standItemModel->setItem(i,j,standItem1);
                //update
//                layout->addWidget(mWKObjPair.at(i*wkColcount+j),i,j,1,1);
                mWKObjPair.at(i*wkColcount+j)->setVisible(false);
            }
            else
                break;
//            if( index+1 <= mWKObjPair.size())
//            {
//                 mWkStItems.operator [](i*wkColcount+j).first->setText(mUpdatePara.data()->wsInfo.operator [](index).name);
//            }
//            else
//                 mWkStItems.operator [](i*wkColcount+j).first->setText("");
        }
    }
    int totalPage  = mWKObjPair.size()/wkPerPageItemsNum + ((mWKObjPair.size()%wkPerPageItemsNum == 0)?0:1);
    if(1 == totalPage)
         mSwPgWidget->hide();
    else
         mSwPgWidget->show();
    mSwPgWidget->setTotalPageNums(totalPage);
    //update2
    tableView ->setModel(standItemModel);
    layout->addWidget(tableView);


}

/*!
 * \brief LayoutAndDisplay::generateSubMCpLayout  根据列表顺序号进行生成指定页面的布局
 * \param index 分机中选中的列表序列号
 */
void LayoutAndDisplay::generateSubMCpLayout(unsigned int index)
{
    if(index >= mSubMPair.size() )
        return;
    QGridLayout *gridLayout = dynamic_cast<QGridLayout*>( mSubMPair.at(index).first->layout() );
    subMColCount = msubMachWidget->width()/(subMachItemWidth+minColumnInterval);

    int count = mSubMPair.at(index).second->count();
    int rowIndex = 0;

    for(int i=0;i < count;)
    {
        rowIndex = i/subMColCount;
        for(int j=0;j < subMColCount;j++)
        {
            mSubMPair.operator [](index).second->operator[](i)->setParent(NULL);
            gridLayout->addWidget( mSubMPair.operator [](index).second->operator[](i),
                    rowIndex,j,1,1);

            i++;
            if(i >= count)
                break;
        }
    }
    rowIndex = count/subMColCount + ( (count%subMColCount == 0)?0:1 );
    int height = (mItemsHeight+minRowInterval)*rowIndex + minRowInterval;
    mSubMPair.operator [](index).first->setMinimumHeight(height);
    mSubMPair.operator [](index).first->setMinimumWidth(msubMachWidget->width());
    mSubMPair.operator [](index).first->setMaximumWidth(msubMachWidget->width());
}

void LayoutAndDisplay:: ensureLayoutData(const HealthData &data)
{
    QLabel *tplabel = NULL;
    QLineEdit *tplineEdit = NULL;
    StateLamp *tpStateLamp = NULL;
    QWidget *outerWid = NULL;
    QHBoxLayout *layout = NULL;
    //    theData = data;
    QString *tpString = NULL;

    for(unsigned long i=0;i < data.workStateNums;i++)
    {   //生成显示对象
        tplabel = new QLabel(QString(data.wsInfo.at(i).name));
        tplineEdit = new QLineEdit();
        tplineEdit->setEnabled(false);

        mWkStItems.append( QPair<QLabel*,QLineEdit*>(tplabel,tplineEdit));

        //生成包裹对象
        outerWid = new QWidget();
        layout = new QHBoxLayout();
        layout->addWidget(tplabel,1);
        layout->addWidget(tplineEdit,1);
        layout->setMargin(0);
        outerWid->setLayout(layout);
        outerWid->setMaximumSize(wkItemWidth,mItemsHeight);
        mWKObjPair.append(outerWid);

    }

    ///**********确定分机组件数据与边框布局***************///
    QList<QPair<QLabel*,StateLamp*> >*  componentInfo = NULL;
    QList<QWidget*>* wrapPair = NULL;
    QScrollArea   *area = NULL;
    QWidget *aSubMachineWidget = NULL;
    for(unsigned int i=0;i < data.subMachineNums;i++)
    {
        area = new QScrollArea();
        area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        aSubMachineWidget = new QWidget();
        aSubMachineWidget->setLayout(new QGridLayout());

        componentInfo = new QList<QPair<QLabel*,StateLamp*> >();
        wrapPair = new QList<QWidget*>();

        for(unsigned int j =0;j < data.smInfo.at(i).componetNum;j++)
        {   //生成显示对象
            tplabel = new QLabel(data.smInfo.at(i).compInfo.at(j).name);
            tpStateLamp  = new StateLamp();
            componentInfo->append(QPair<QLabel*,StateLamp*>(tplabel,tpStateLamp));

            //生成包裹对象
            outerWid = new QWidget();
            layout = new QHBoxLayout();
            layout->addWidget(tplabel,1);
            layout->addWidget(tpStateLamp,1);
            layout->setMargin(0);
            outerWid->setLayout(layout);
            outerWid->setFixedSize(subMachItemWidth,mItemsHeight);
            wrapPair->append(outerWid);
        }
        mSubMchCmpntItems.append(QPair<QString,QList<QPair<QLabel*,StateLamp*> >* >(data.smInfo.at(i).name,componentInfo));
        mSubMPair.append(QPair<QWidget*,QList<QWidget*>* >(aSubMachineWidget,wrapPair));

        area->setWidget(aSubMachineWidget);
        msubMachWidget->addWidget(area);////@warning debug 时这里有错误 OpenType support missing for script 66 连续3行
        msubMachList->addItem(data.smInfo.at(i).name);
    }
}

void LayoutAndDisplay::upDateSwitchPageInfo(int index)
{
//    cout<<"try"<<index<<";"<<wkRowCount<<";"<<wkColcount<<endl;
    int thisTime;
    int wkPerPageItemsNum = wkRowCount*wkColcount;

//    int theindex = 0;
//    cout<<"-----------------"<<endl;

    for(int i=0;i < wkRowCount;i++)
    {
        for(int j =0;j<wkColcount;j++)
        {
//            theindex = (start -1)* wkPerPageItemsNum+ i*wkColcount+j;
            if((i*wkColcount+j+(index-1)*wkPerPageItemsNum) < mWKObjPair.size())
            {
//                cout<<i<<";"<<j<<endl;
                //update
                QList<QLabel*>tplabelFindOut = mWKObjPair.at(i*wkColcount+j+(index-1)*wkPerPageItemsNum)->findChildren<QLabel*>();
                QList<QLineEdit*>tplineEditFindOut = mWKObjPair.at(i*wkColcount+j+(index-1)*wkPerPageItemsNum)->findChildren<QLineEdit*>();
                QStandardItem *standItem1 = new QStandardItem(tplabelFindOut.at(0)->text()+"   "+ tplineEditFindOut.at(0)->text());
//                QStandardItem *standItem1 = new QStandardItem(dataList[i*wkColcount+j]+"   "+ tplineEditFindOut.at(0)->text());
                standItemModel->setItem(i,j,standItem1);
                //update
            }
            else{
//                cout<<i<<";"<<j<<endl;
                QStandardItem *standItem1 = new QStandardItem("   ");
                standItemModel->setItem(i,j,standItem1);
            }
        }
    }


//    for(int i = 0;i < wkPerPageItemsNum;i++)
//    {
//        thisTime = (index -1)*wkPerPageItemsNum + i;
//        if((thisTime+1) <= mWKObjPair.size() && (thisTime+1) <= mUpdatePara.data()->wsInfo.count())
//        {
//            mWkStItems.operator [](i).first->setText(mUpdatePara.data()->wsInfo.operator [](thisTime).name);
//        }
//        else
//        {
//            if(i < mWkStItems.count())
//                mWkStItems.operator [](i).first->setText("");
//        }
//    }
}

void LayoutAndDisplay::choooseComponent(QModelIndex index)
{

    generateSubMCpLayout(index.row());
    msubMachWidget->setCurrentIndex(index.row());
}

void LayoutAndDisplay::initView()
{
    mWorkStateWidget = new QWidget();
    mWorkStateWidget->setMinimumHeight(minRowInterval*2+mItemsHeight);
    mWorkStateWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mWorkStateWidget->setLayout(new QGridLayout());

    mSwPgWidget = new SwitchPage(SwitchPage::PageNumAppearAtStart);
    mSwPgWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    mSwPgWidget->bindCaller(this);

   //以下的布局内容或许会更改
    QWidget *workStateWidget = new QWidget();
    QVBoxLayout *vlayout = new QVBoxLayout();
    vlayout->setContentsMargins(1,1,1,1);
    vlayout->addWidget(mWorkStateWidget);
    vlayout->addWidget(mSwPgWidget);
    workStateWidget->setLayout(vlayout);

    msubMachWidget = new QStackedWidget;
    msubMachList = new QListWidget();
    msubMachList->setFixedWidth(100);
    connect(msubMachList,SIGNAL(clicked(QModelIndex)),this,SLOT(choooseComponent(QModelIndex)));
    StateLamp::initColorMap();

    QWidget *subStateInfoWidget = new QWidget();
    QHBoxLayout *hlayout1 = new QHBoxLayout();
    hlayout1->setContentsMargins(0,0,0,0);
    hlayout1->addWidget(msubMachList);
    hlayout1->addWidget(msubMachWidget);
    subStateInfoWidget->setLayout(hlayout1);

    tabWidget = new QTabWidget();
    tabWidget->addTab(workStateWidget,tr("WorkState"));
    tabWidget->addTab(subStateInfoWidget,tr("Sub-component"));

    QVBoxLayout *vlayout2 = new QVBoxLayout();
    vlayout2->setContentsMargins(0,0,0,0);
    vlayout2->addWidget(tabWidget);
    this->setLayout(vlayout2);

    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(reLayout_BySize()));

    msubMachWidget->setCurrentIndex(0);

    //update
    tableView = new QTableView;
    standItemModel = new QStandardItemModel();

//    QTableView ta;

//    QHeaderView hv(Qt::Vertical);

//    hv.hide();
//    tableView.setVerticalHeader(&hv);

//    tableView->horizontalHeader()->hide();
//    tableView->verticalHeader()->hide();
//    QTableView::horizontalHeader()->hide();
}

void LayoutAndDisplay::retranslateUi()
{
    mSwPgWidget->retranslateUi();
    tabWidget->setTabText(0,tr("WorkState"));
    tabWidget->setTabText(1,tr("Sub-component"));
}

void StateLamp::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    QColor color;
    QMap<int,QColor>::const_iterator  ite = colorMap.find(this->value);
    if(ite == colorMap.end())
        color = othersColor;
    else
        color= ite.value();
    painter.setPen(color);
    QPainterPath path;
    double wid = qMin(this->width(),this->height());
    path.addEllipse(0,0,wid,wid);
    painter.drawPath(path);
    painter.fillPath(path,QBrush(color));
    event->accept();
}

void StateLamp::resizeEvent(QResizeEvent *event)
{
    double radius  = qMin(event->size().width(), event->size().height());
    this->setGeometry(this->geometry().x(),this->geometry().y(),radius,radius);
}
