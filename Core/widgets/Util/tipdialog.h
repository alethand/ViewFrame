#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include <QObject>
#include <QMessageBox>

/*
 *   实现提示对话框
 */

class TipDialog : public QObject
{
    Q_OBJECT
public:
    explicit TipDialog(QObject *parent = 0);
    
signals:
    
public slots:
    
public:
    static int TipMessageBox(QString str, int type = 1);
};

#endif // TIPDIALOG_H
