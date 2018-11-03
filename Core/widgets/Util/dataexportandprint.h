#ifndef DATAEXPORTANDPRINT_H
#define DATAEXPORTANDPRINT_H

#include <QObject>
#include <QTableView>
#include "exporttoexcelthread.h"
#include "tipdialog.h"
#include "widgets/datadisplay/modelview/tableviewdata.h"
/*
 * 说明： 数据导出和打印的功能类
 *
 */

class DataExportAndPrint : public QObject
{
    Q_OBJECT
public:
    explicit DataExportAndPrint(QObject *parent = 0);

    static bool exportToExcel(QTableView *table, QString filepath); // 将table的数据导出到excel
signals:

public slots:
    void exportsEnd(); // 导出Excel结束
};

#endif // DATAEXPORTANDPRINT_H
