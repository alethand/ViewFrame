/*!
 *  @details   全脉冲表格
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.13
 *  @copyright NanJing RenGu.
 *
 * note1:18-10-12:修改全脉冲网络接收部分，并将数据连接。
 *                提供时间特性图的显示与直方图接口-未调用
 */
#ifndef ALLPLUSETABLE_H
#define ALLPLUSETABLE_H

#include <QRadioButton>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QStackedWidget>

#include <QModelIndex>
#include "Base/pluginmanager/rcomponent.h"
#include "protocol/datastruct.h"
#include "modelview/tableviewdata.h"
#include "Graphics/energybar.h"

using namespace Datastruct;
using namespace Protocol::Data;
using namespace CustomWidget;

namespace DataView {

class AllPluseTablePrivate;

class AllPluseDock : public Base::RComponent
{
    Q_OBJECT
public:
    explicit AllPluseDock(QWidget *parent = 0);
    ~AllPluseDock();

    enum WorkMode{
        RealTime =0,        /*!< 实时显示模式 */
        Historical       /*!< 历史显示模式 */
    };

    bool initialize();
    void release();
    void initView();
    void ininData();
    QString pluginName();
    void onMessage(MessageType::MessType type);

public slots:
    void recvAllPlusePara(char *buff, int len);

private slots:
    void on_radioButton_RealityShow_clicked();                                  /*! 实时模式 */
    void on_radioButton_HistoryShow_clicked();                                  /*! 历史模式 */
    void on_btn_load_clicked();
    void statistic_ShowOrginInfo(QModelIndex index);                            /*! 历史模式显示对应原始信息 */


    /*********  以下2个图已提供接口，但是未调用  ************/
    void showTimeProperties(double cycleStart,double cycleEnd,int properties); /*! 显示时间特性图 */
    void showFreqHistogram(int properties);                                     /*! 显示参数频度直方图 */
private:

    void updateView();                                                                                  //更新视图
    void switchModeTo(WorkMode mode);
    void retranslateUi();

    QString getCurrentDate();

private:
    QRadioButton * radioButton_RealityShow;
    QRadioButton * radioButton_HistoryShow;
    QPushButton *btn_load;

    QStackedWidget *mWidget;



    HugeData_Gram *originData;        //原始数据
    HugeData_Gram *staticsData;       //统计数据

    TableView viewRT_Origin;
    TableView viewHS_Origin;
    TableView view_Statistic;

    HugeData_Model modelRT_originData;                  /*! 实时模型-源数据*/
    HugeData_Model modelHS_orginData;                   /*! 历史模型-源数据*/
    HugeData_Model model_StatisticData;

    EnergyBar  *energvGraphics;       /*! 能量图 - 单条统计信息*/
    QModelIndex filterIndexRT;
    QModelIndex filterIndexHS;
    QModelIndex filterIndexStatistic;

public slots:
    void filterRT(QModelIndex index);
    void recFilterMessageRT(QList<double> filterMessage);
    void filterHS(QModelIndex index);
    void recFilterMessageHS(QList<double> filterMessage);
    void filterStatistic(QModelIndex index);
    void recFilterMessageStatistic(QList<double> filterMessage);

};


} //namespace DataView

#endif // ALLPLUSETABLE_H
