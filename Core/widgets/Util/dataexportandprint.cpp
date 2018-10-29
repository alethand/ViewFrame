#include "dataexportandprint.h"
#include <QFileDialog>
#include <QTextEdit>
#include <QTextStream>
#include <QPixmap>
#include <QPainter>

DataExportAndPrint *dataAndPrint = new DataExportAndPrint();
DataExportAndPrint::DataExportAndPrint(QObject *parent) : QObject(parent)
{

}

// 将table的数据导出为filepath下的excel文件
bool DataExportAndPrint::exportToExcel(QTableView *table, QString filepath)
{
//    ExportToExcelThread *exportExl = new ExportToExcelThread(table,filepath);
//    connect(exportExl,SIGNAL(finished()),dataAndPrint,SLOT(exportsEnd()));
//    exportExl->start();
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
            QModelIndex index = table->model()->index(i, j);
            QString cel= table->model()->data(index).toString();
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
    QString mesgstr = "导出" + filepath + "成功";
    TipDialog::TipMessageBox(mesgstr);
    return true;
}

// 导出Excel结束
void DataExportAndPrint::exportsEnd()
{
    ExportToExcelThread *thread = (ExportToExcelThread*)sender();
    QString mesgstr = "导出" + thread->getFilePath() + "成功";
    TipDialog::TipMessageBox(mesgstr);
    delete thread;
}
