/*!
 *  @brief     表格
 *  @details
 *  @author    zl
 *  @version   1.0
 *  @date      2018.09.25
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#include "tableviewdata.h"

#include <QMenu>
#include <QModelIndex>
#include <QPoint>
#include <QHeaderView>
#include <QContextMenuEvent>


namespace DataView{

class TableViewPrivate
{
    Q_DECLARE_PUBLIC(TableView)
public:
    explicit TableViewPrivate(TableView * q):q_ptr(q){
        m_Menu = nullptr;
    }

    TableView * q_ptr;

    QMenu *m_Menu;
};

TableView::TableView(QWidget *parent) : QTableView(parent),
    d_ptr(new TableViewPrivate(this))
{
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setVisible(false);
//    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
//    horizontalHeader()->setHighlightSections(false);
    setAlternatingRowColors(true);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    
    //setShowGrid(false);
    setFocusPolicy(Qt::NoFocus);
    
    resizeColumnsToContents();
    resizeRowsToContents();
    
    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(On_TableSectionClicked(int)));
    connect(this, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(doubleClicked(QModelIndex)));
}

void TableView::addAction(QAction *action)
{
    Q_D(TableView);
    if(d->m_Menu == nullptr){
        d->m_Menu = new QMenu;
    }
    d->m_Menu->addAction(action);
}

void TableView::contextMenuEvent(QContextMenuEvent *event)
{
    Q_D(TableView);
    if(d->m_Menu){
        emit contextMenuPos(event->pos());
        d->m_Menu->exec(cursor().pos());
    }
}

//点击表头排序
void TableView::On_TableSectionClicked(int column)
{
    static bool f = true;
    this->sortByColumn(column, f ? Qt::AscendingOrder : Qt::DescendingOrder);
    f = !f;
}

//双击表格
void TableView::doubleClicked(QModelIndex index)
{
//    if (static_cast<TaskHead>(index.column()) == TaskHead::T_TYPE)
//        emit tableCheckDoubleSignal(index);
    emit tableCheckDoubleSignal(index);
}

}
