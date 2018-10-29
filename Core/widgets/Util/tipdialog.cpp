#include "tipdialog.h"

TipDialog::TipDialog(QObject *parent) :
    QObject(parent)
{
}

/*
 * para:
 *    type = 1: 提示对话框
 *    type = 2：确定取消对话框
 *
 *  return:
 *    如果确认返回1，否则返回0值
 */
int TipDialog::TipMessageBox(QString str,int type)
{
    if(type == 1)
        QMessageBox::warning(NULL,QObject::tr("Information"),str,QMessageBox::Ok);
    else if(type == 2)
    {
        int buton = QMessageBox::question(NULL,QObject::tr("Information"),str,QMessageBox::Yes,QMessageBox::Cancel);
        if(buton == QMessageBox::Yes)
            return 1;
        else
            return 0;
    }
    else if(type == 3)
        QMessageBox::warning(NULL,QObject::tr("Information"),str,QMessageBox::Ok);
    else if(type == 4)
    {
        int buton = QMessageBox::question(NULL,QObject::tr("Information"),str,QMessageBox::Yes,QMessageBox::Cancel);
        if(buton == QMessageBox::Yes)
            return 1;
        else
            return 0;
    }
}
