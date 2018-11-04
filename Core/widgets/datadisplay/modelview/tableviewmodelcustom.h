/*!
 *  @brief     列表模式(中频数据、全脉冲使用)
 *  @details
 *  @author    zl
 *  @version   1.0
 *  @date      2018.09.25
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TABLEVIEWMODELCUSTOM_H
#define TABLEVIEWMODELCUSTOM_H

#include <QAbstractTableModel>
#include <QSharedPointer>
#include "Base/pluginmanager/observer.h"
#include "protocol/datastruct.h"

using namespace Datastruct;
namespace DataView {

class TableViewModelCustom: public QAbstractTableModel,public Base::Observer
{
    Q_OBJECT
public:
    explicit TableViewModelCustom(QObject *parent = 0);
    ~TableViewModelCustom();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    inline virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    void onMessage(MessageType::MessType type);

    void resetHeadInfo(const QStringList& head);
    void setTableCustomKind(TableCustomKind kind);
 //   void updateAllPluseStatisticInfoList(const AllPluseStatisticInfoList& list);
    void updateAllPulseOriginalInfoList(const AllPulseOriginalInfoList& list);
    void updateMFAcquistionInfoList(const MFAcquistionInfoList& list);

private:
    void resetData();
    void retranslateUi();

private:
    QStringList headInfo;
    int tableTotalRows;             //总函数
    TableCustomKind tableCustomKind;//表格显示类型
   // AllPluseStatisticInfoList allPluseStatisticInfoList;        //全脉冲统计信息列表
    AllPulseOriginalInfoList allPulseOriginalInfoList;          //全脉冲原始信息列表
    MFAcquistionInfoList mfAcquistionInfoList;                  //中频数据表格信息

};

} //namespace DataView


#endif // TABLEVIEWMODELCUSTOM_H
