/*!
 *  @brief     表格数据模型
 *  @details
 *  @author    wlc
 *  @version   1.0
 *  @date      2018.09.20
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TABLEVIEWMODEL_H
#define TABLEVIEWMODEL_H

#include <QAbstractTableModel>
#include <QSharedPointer>

#include "pluginmanager/observer.h"
#include "../head.h"

namespace TaskControlModel {

class TableViewModel : public QAbstractTableModel , public Core::Observer
{
    Q_OBJECT
public:
    explicit TableViewModel(QObject *parent = 0);
    ~TableViewModel();
    
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual int columnCount(const QModelIndex &parent) const;
    inline virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void updateTaskList(NewTaskList &list);

    void onMessage(MessageType::MessType type);
  
public:
    enum ColumnType
    {
        NO,                 //状态
        TYPE,               //数据类型
        PARAMENTER,         //任务参数
        EXCUTIONTTIME,      //任务执行时间
        EXCUTIONTTIMELONG   //任务执行时长
    };

private:
    void resetData();
    void retranslateUi();
    
private:
    QStringList headInfo;
    NewTaskList taskList;
};

} //namespace TaskControlModel

#endif // TABLEVIEWMODEL_H
