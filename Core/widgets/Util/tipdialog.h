#ifndef TIPDIALOG_H
#define TIPDIALOG_H

#include <QObject>
#include <QMessageBox>

class TipDialog : public QObject
{
    Q_OBJECT
public:
    explicit TipDialog(QObject *parent = 0);

    static int TipMessageBox(QString str, int type = 1);

};

#endif // TIPDIALOG_H
