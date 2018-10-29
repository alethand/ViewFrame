/*!
 *  @details   全脉冲表格
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.13
 *  @copyright NanJing RenGu.
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
#include "widgets/CustomWidget/energybar.h"

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
        RealTime,        /*!< 实时显示模式 */
        Historical       /*!< 历史显示模式 */
    };

    bool initialize();
    void release();
    void initView();
    void ininData();
    QString pluginName();
    void onMessage(MessageType::MessType type);

signals:
   // void sendAllPulseInfoList(AllPluseStatisticInfoList* statisticInfoList,AllPulseOriginalInfoList* originalInfoList); //全脉冲信息列表信号

public slots:
    void recvAllPlusePara(char *buff, int len);

private slots:
    void on_radioButton_RealityShow_clicked();
    void on_radioButton_HistoryShow_clicked();
    void on_btn_load_clicked();
    void doubleClickedTable(QModelIndex index);
    void clearTable();

private:
    void switchModeTo(WorkMode mode);
    void retranslateUi();
    void setTableData();
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
};


} //namespace DataView

#endif // ALLPLUSETABLE_H
