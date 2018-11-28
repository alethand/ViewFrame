/*!
 *  @brief     表格单元格修改代理
 *  @details
 *  @author    wlc
 *  @version   1.0
 *  @date      2018.09.20
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TableViewDelegate_H
#define TableViewDelegate_H

#include <QItemDelegate>
#include "../head.h"

namespace TaskControlModel {

class TableViewDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    TableViewDelegate(QObject *parent = 0);
    ~TableViewDelegate();
    
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor,
                       const QModelIndex &index) const;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model, 
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
    
};

} //namespace TaskControlModel

#endif // TableViewDelegate_H
