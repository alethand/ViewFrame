#include "table.h"
#include <QHeaderView>

Table::Table(QWidget *parent)
    : QTableWidget(parent)
    , rightClickedItem(NULL)
    , dupFlag(false)
{
    rowHeghtList.clear();
    rowWidthList.clear();
    fileMap.clear();
    this->setMouseTracking(true);
    //setWindowTitle(tr("table"));
   // horizontalHeader()->setDefaultSectionSize(100);
    //verticalHeader()->setDefaultSectionSize(30);    //设置默认行高
    tableWidth = 100;
    tableHeight = 30;
    //horizontalHeader()->setClickable(false);      //设置表头不可点击（默认点击后进行排序
  //  horizontalHeader()->setClickable(true);         //设置表头不可点击（默认点击后进行排序

 //   QFont font = horizontalHeader()->font();        //设置表头字体加粗
 //   font.setBold(true);
  //  horizontalHeader()->setFont(font);
   // horizontalHeader()->setStretchLastSection(true);    //设置充满表宽度
   // horizontalHeader()->setMovable(false);              //表头左右互换
    //verticalHeader()->setResizeMode(QHeaderView::ResizeToContents);

    setFrameShape(QFrame::NoFrame);      //设置无边框
    //setShowGrid(false);                //设置不显示格子线
//    verticalHeader()->setVisible(false); //设置垂直头不可见
    verticalHeader()->setVisible(false); //设置垂直头不可见
    setSelectionMode(QAbstractItemView::ExtendedSelection);  //可多选（Ctrl、Shift、  Ctrl+A都可以）
    setSelectionBehavior(QAbstractItemView::SelectRows);  //设置选择行为时每次选择一行
    setEditTriggers(QAbstractItemView::NoEditTriggers); //设置不可编辑


    //setStyleSheet("selection-background-color:lightblue;"
                  //"QTableCornerButton::section{background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(134, 245, 99, 255),stop:0.5 rgba(134, 148, 99, 255),stop:1 rgba(115, 87, 128, 255));color:rgb(25, 70, 100);padding-left: 1px;border: 1px solid #FFFF00;}");  //设置选中背景色
    //horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色

    //设置表头背景色
    //horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(134, 245, 99, 255),stop:0.5 rgba(134, 148, 99, 255),stop:1 rgba(115, 87, 128, 255));color:rgb(25, 70, 100);padding-left: 1px;border: 1px solid #FFFF00;}"); //设置表头背景色
   // horizontalHeader()->setStyleSheet("QHeaderView::section{background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 rgba(18, 228, 207, 255),stop:0.5 rgba(82, 158, 165, 255),stop:1 rgba(115, 87, 128, 255));color:rgb(25, 70, 100);padding-left: 1px;border: 1px solid #FFFF00;}"); //设置表头背景色

    setAlternatingRowColors(true);

    //setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    //设置水平、垂直滚动条样式
   /* horizontalScrollBar()->setStyleSheet("QScrollBar{background:transparent; height:12px;}"
                                         "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                         "QScrollBar::handle:hover{background:gray;}"
                                         "QScrollBar::sub-line{background:transparent;}"
                                         "QScrollBar::add-line{background:transparent;}");

    verticalScrollBar()->setStyleSheet("QScrollBar{background:transparent; width:12px;}"
                                       "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                       "QScrollBar::handle:hover{background:gray;}"
                                       "QScrollBar::sub-line{background:transparent;}"
                                       "QScrollBar::add-line{background:transparent;}");

    horizontalScrollBar()->setVisible(false);
    // set the item delegate to your table widget
    setItemDelegate(new NoFocusDelegate());             //虚线边框去除
    //setFocusPolicy(Qt::NoFocus);   //去除选中虚线框

    horizontalHeader()->setHighlightSections(false);    //点击表时不对表头行光亮（获取焦点）
    //horizontalHeader()->setHighlightSections(true);    //点击表时对表头行光亮（获取焦点）
*/
    //createActions();
    setItemFixed(false);
    //connect(this, SIGNAL(itemEntered(QTableWidgetItem*)), this , SLOT(slotItemEntered(QTableWidgetItem*)));
    //connect(this, SIGNAL(itemSelectionChanged()), this , SLOT(slotItemSelectionChanged()));
    //this->resize(600, 600);
}

Table::~Table()
{

}

void Table::setColumnValue(const int &columnSum, const QStringList &header)
{
    setColumnCount(columnSum);                //设置列数
    this->setHorizontalHeaderLabels(header);  //设置列的标签
}

void Table::setHeaderWidth(const int &index, const int &width)
{
    setColumnWidth(index,width);
    //horizontalHeader()->resizeSection(index,width);
}

void Table::setHeaderHeight(const int &height)
{
    setHeaderHeight(height);        //设置表头的高度
    if (rowHeghtList.isEmpty())
    {
      rowHeghtList.append(height);
    }
    else
    {
      rowHeghtList[0] = height;
    }
    tableHeight = 0;
    for(int index = 0; index < rowHeghtList.count(); index++)
       tableHeight += rowHeghtList.at(index);
    resize(tableWidth, tableHeight);
}

void Table::addRowValue(const int &height, const QStringList &value, const QIcon &fileIcon)
{
    int row_count = rowCount();    //获取表单行数
    insertRow(row_count);          //插入新行
    setRowHeight(row_count, height);
    for (int index = 0; index < columnCount(); index++)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        if (index == 0)
        {
            item->setIcon(fileIcon);
            item->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            fileMap.insert(item, value.at(index));
        }
        else
        {
            item->setTextAlignment(Qt::AlignCenter);
        }
        item->setText(value.at(index));
        setItem(row_count, index, item);
    }
    rowHeghtList.append(height);
    tableHeight += height;
    resize(tableWidth, tableHeight);
}


void NoFocusDelegate::paint(QPainter* painter, const QStyleOptionViewItem & option, const QModelIndex &index) const
{
    QStyleOptionViewItem itemOption(option);
    if (itemOption.state & QStyle::State_HasFocus)
        itemOption.state = itemOption.state ^ QStyle::State_HasFocus;
    QStyledItemDelegate::paint(painter, itemOption, index);
}

void Table::setRowH(const int &index, const int &height)
{
  setRowHeight(index, height);
  if (rowHeghtList.count() <= index + 1)
  {
    rowHeghtList.append(height);
  }
  else
  {
    rowHeghtList[index+1] = height;
  }
  tableHeight = 0;
  for(int index = 0; index < rowHeghtList.count(); index++)
     tableHeight += rowHeghtList.at(index);
  resize(tableWidth, tableHeight);
}

void Table::createActions()
{
/*
  popMenu = new QMenu();
  actionName = new QAction(this);
  actionSize = new QAction(this);
  actionType = new QAction(this);
  actionDate = new QAction(this);
  actionOpen = new QAction(this);
  actionDownload = new QAction(this);
  actionFlush = new QAction(this);
  actionDelete = new QAction(this);
  actionRename = new QAction(this);
  actionCreateFolder = new QAction(this);

  actionOpen->setText(tr("打开"));
  actionDownload->setText(tr("下载"));
  actionFlush->setText(tr("刷新"));
  actionDelete->setText(tr("删除"));
  actionRename->setText(tr("重命名"));
  actionCreateFolder->setText(tr("新建文件夹"));
  actionName->setText(tr("名称"));
  actionSize->setText(tr("大小"));
  actionType->setText(tr("项目类型"));
  actionDate->setText(tr("修改日期"));
*/
  //actionFlush->setShortcut(QKeySequence::Refresh);
  //connect(actionRename, SIGNAL(triggered()), this, SLOT(slotActionRename()));
}

void Table::contextMenuEvent(QContextMenuEvent *event)
{
  return;
  /*
  popMenu->clear();
  QPoint point = event->pos();
  rightClickedItem = this->itemAt(point);
  if(rightClickedItem != NULL)
  {
    popMenu->addAction(actionDownload);
    popMenu->addAction(actionFlush);
    popMenu->addSeparator();
    popMenu->addAction(actionDelete);
    popMenu->addAction(actionRename);
    popMenu->addSeparator();
    popMenu->addAction(actionCreateFolder);
    QMenu *sortStyle = popMenu->addMenu(tr("排序"));
    sortStyle->addAction(actionName);
    sortStyle->addAction(actionSize);
    sortStyle->addAction(actionType);
    sortStyle->addAction(actionDate);

    popMenu->exec(QCursor::pos());
    event->accept();
  }
  */
}

QModelIndex Table::moveCursor(QAbstractItemView::CursorAction cursorAction, Qt::KeyboardModifiers modifiers)
{
    //重写移动光标事件，当存在编辑项的时候，让光标永远位于当前项（编辑项）,否则返回父类
    if(rightClickedItem && rightClickedItem->row() >= 0)
    {
        return currentIndex();
    }
    else
    {
       return QTableWidget::moveCursor(cursorAction, modifiers);
    }
}


void Table::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        QTableWidgetItem *item = currentItem();
        if (item)
        {
            closePersistentEditor(item);
            openPersistentEditor(item);
            slotItemSelectionChanged();
            dupFlag = false;
        }
    }
}

void Table::slotItemSelectionChanged()
{
    //关闭编辑项
    if (rightClickedItem && dupFlag == false)
    {
        int editRow = rightClickedItem->row();
        QTableWidgetItem *item = this->item(editRow, 0);
        QMap<QTableWidgetItem *, QString>::iterator it;
        for (it = fileMap.begin(); it != fileMap.end(); ++it)
        {
            if (it.key() != item)
            {
                if (it.value() == item->text())
                {
                    dupFlag = true;
                }
            }
        }
        if (dupFlag == false)
        {
            this->closePersistentEditor(item);
            rightClickedItem = NULL;
        }
        else
        {
            //QMessageBox::critical(this,tr("错误提示"),tr("文件重名"), tr("确定"));
            setCurrentItem(item);
        }
    }
    else
    {
        dupFlag = false;
    }
}

void Table::setItemFixed(bool flag)
{
  if (flag == true)
  {
      horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  }
  else
  {
      horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
  }
}

bool Table::getSelectedRow(QList<int> &rowList)
{
    //多选并获取所选行
    QList<QTableWidgetItem *> items = this->selectedItems();
    int itemCount = items.count();
    if(itemCount <= 0)
    {
        return false;
    }
    for (int index = 0; index < itemCount; index++)
    {
        int itemRow = this->row(items.at(index));
        rowList.append(itemRow);
    }
    return  true;
}

void Table::slotItemEntered(QTableWidgetItem *item)
{
  return;
  if(!item)
    return;
  QString name = item->text();
  if (name.isEmpty())
    return;
  //QToolTip::showText(QCursor::pos(), name);
}

void Table::slotActionRename()
{
    //获得当前节点并获取编辑名称
    if (rightClickedItem)
    {
        int editRow = rightClickedItem->row();
        QTableWidgetItem *item = this->item(editRow, 0);   //编辑的行号及第一列
        this->setCurrentCell(editRow, 0);
        this->openPersistentEditor(item);                  //打开编辑项
        this->editItem(item);
    }
}

//向某行、某列插入某值
void Table::addRowValue(const int &row,const int &col,const QString str)
{
    if(row<0||col<0)
    {
        return;
    }
    int row_count=rowCount();
    if(row+1>row_count)
    {
        for(;row_count<=row+1;row_count++)  //插入新行
        {
            int height=tableHeight;
            addRowValue(height,row_count,false);
        }
    }

    QTableWidgetItem *item0;
    item0=new QTableWidgetItem;
    item0->setTextAlignment(Qt::AlignCenter);

    //item0->setBackgroundColor(Qt::red);

    item0->setText(str);
    setItem(row,col,item0);       //设置表格的内容
    scrollToBottom();
}

//插入到第N行，设置所插入行的高度
//参数：
//height：N行高度
//rowNum: N行
//blCreatFlag:首次创建标识 true:首次创建或者清除后重建表格
//返回值:无
void Table::addRowValue(const int &height,const int &rowNum,const bool blCreatFlag)
{
    QString txt;
    int row_count;
    if(blCreatFlag)
    {
        row_count=0;
    }
    else
    {
        row_count=rowCount();
    }

    for(int row=row_count;row<rowNum;row++)
    {
        if(!blCreatFlag)
        {
            row_count = rowCount();         //获取表单行数
        }

        insertRow(row_count);               //插入新行

        setRowHeight(row_count, height);
        tableHeight=height;

        QTableWidgetItem *item0;
        item0=new QTableWidgetItem;
        item0->setTextAlignment(Qt::AlignCenter);

//        if(row_count%2!=0)
//        {
//            QColor color(134, 148, 99, 255);
//            item0->setBackgroundColor(color);
//        }
        txt=QString("%1").arg(row_count+1);
        item0->setText(txt);
        setItem(row_count,0,item0);       //设置表格的内容

        if(blCreatFlag)
        {
            row_count++;
        }
    }
}

