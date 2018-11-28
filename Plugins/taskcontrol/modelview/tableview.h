/*!
 *  @brief     任务显示表格
 *  @details
 *  @author    wlc
 *  @version   1.0
 *  @date      2018.09.20
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QTableView>
#include <QSharedPointer>

class QModelIndex;
class QAction;

namespace TaskControlModel{

class TableViewPrivate;

class TableView : public QTableView
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TableView)
public:
    explicit TableView(QWidget *parent = 0);
    virtual ~TableView() = default;
    
    void addAction(QAction *action);

protected:
    void contextMenuEvent(QContextMenuEvent * event);
    
signals:
    void tableCheckDoubleSignal(QModelIndex);
    void contextMenuPos(QPoint);
    
public slots:
    void On_TableSectionClicked(int);
    void doubleClicked(QModelIndex);
    
private:
    QSharedPointer<TableViewPrivate> d_ptr;
};

} //namespace TaskControlModel

#endif // TABLEVIEW_H
