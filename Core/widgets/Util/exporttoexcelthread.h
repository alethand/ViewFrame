#ifndef EXPORTTOEXCELTHREAD_H
#define EXPORTTOEXCELTHREAD_H

#include <QThread>
#include <QTableWidget>
#include <QString>
#include <QTextStream>
#include "widgets/datadisplay/modelview/tableviewdata.h"

class ExportToExcelThread : public QThread
{
    Q_OBJECT
public:
    explicit ExportToExcelThread(QTableView *table, QString filepath);
    void run();
    QString getFilePath();
signals:

public slots:

private:
    QTableView *table;
    QString filepath;
};

#endif // EXPORTTOEXCELTHREAD_H
