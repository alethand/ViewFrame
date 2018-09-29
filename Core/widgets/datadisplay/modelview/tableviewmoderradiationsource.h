/*!
 *  @brief     数据源列表模式
 *  @details
 *  @author    zl
 *  @version   1.0
 *  @date      2018.09.25
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TABLEVIEWMODERRADIATIONSOURCE_H
#define TABLEVIEWMODERRADIATIONSOURCE_H

#include <QAbstractTableModel>
#include <QSharedPointer>
#include "Base/pluginmanager/observer.h"
#include "protocol/datastruct.h"
using namespace Datastruct;

namespace DataView {
class TableViewModelRadiationSource: public QAbstractTableModel , public Base::Observer
{
    Q_OBJECT
public:
    explicit TableViewModelRadiationSource(QObject *parent = 0);
    ~TableViewModelRadiationSource();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    inline virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    void onMessage(MessageType::MessType type);

    void resetHeadInfo(const QStringList& head);
    void resetRefreshModel(const DataRefreshModel &model);
    void updateTableData(const RSDataList& rsList, const bool isDialog);
    void updateTableData(const RSDataMap& rsMap);
    void resetHigetLightRow(int row);

private:
    void resetData();
    void retranslateUi();

private:
    QStringList headInfo;
    DataRefreshModel refreshModel;  //刷新模式
    RSDataList rsDataList;          //数据源信息列表
    RSDataMap rsDataMap;            //覆盖刷新下的map
    int tableTotalRows;             //总函数
    int hightLightRow;              //覆盖刷新模式下的高亮行
};
}

#endif // TABLEVIEWMODERRADIATIONSOURCE_H
