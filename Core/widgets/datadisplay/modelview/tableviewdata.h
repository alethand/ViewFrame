#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QObject>
#include <QTableView>
#include <QSharedPointer>

class QModelIndex;
class QAction;

namespace DataView{

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

} //namespace DataView

#endif // TABLEVIEW_H
