#include "exporttoexcelthread.h"
#include <QFileDialog>
#include <QTextEdit>
#include <QDebug>
#include "tipdialog.h"

ExportToExcelThread::ExportToExcelThread(QTableView *table, QString filepath)
{
    this->table = table;
    this->filepath = filepath;
}

void ExportToExcelThread::run()
{
    //构建html文档内容,将本地字节序转为Unicode,防止汉语显示乱码
    QString Begin = QString::fromLocal8Bit("<html><head></head><body><table border=\"1\" >");
    QString end = QString::fromLocal8Bit("</table></body></html>");
    //定义一个链表，用于存放table里的内容
    QList<QString> list;
    int row = table->model()->rowCount();
    int col = table->model()->columnCount();
    QString titleStr = "<tr>";
    for(int j=0;j<col;j++)
    {
        QString cel = table->model()->headerData(j,Qt::Horizontal,Qt::DisplayRole).toString();
        titleStr += QString("<td>%1</td>").arg(cel);
    }
    titleStr += "</tr>";
    list.push_back(titleStr);

    for(int i=0;i<row;i++)
    {
        QString rowStr = "<tr>";
        for(int j=0;j<col;j++)
        {
            //QString cel = table->item(i,j)->text();
            //QModelIndex index = table->model()->index(i, j);
            QString cel= table->model()->data(table->model()->index(i, j)).toString();
            rowStr += QString("<td>%1</td>").arg(cel);
        }
        rowStr += "</tr>";
        list.push_back(rowStr);
    }
    QString text = Begin;
    for(int i=0;i<list.size();++i)
    {
        text.append(list.at(i));
    }
    QFile file(filepath);
    text.append(end);
    QTextEdit textEdit;
    textEdit.setText(text);


    if(file.open(QFile::WriteOnly | QIODevice::Text))
    {
        QTextStream ts(&file);
        ts.setCodec("UTF-8");
        ts<<textEdit.document()->toHtml("UTF-8");
    }

}

QString ExportToExcelThread::getFilePath()
{
    return filepath;
}
