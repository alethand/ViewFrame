/*!
 *  @brief     数据源覆盖刷新模式下的详细信息列表
 *  @details
 *  @author    zl
 *  @version   1.0
 *  @date      2018.09.25
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RADIATIONSOURCETABLERENOVATEDIALOG_H
#define RADIATIONSOURCETABLERENOVATEDIALOG_H

#include <QDialog>
#include <qglobal.h>
#include "protocol/datastruct.h"
using namespace Datastruct;
namespace Ui {
class RadiationSourceTableRenovateDialog;
}


namespace DataView {
//class RadiationSourceTableRenovatePrivate;
class TableViewModelRadiationSource;
class TableView;
class RadiationSourceTableRenovateDialog : public QDialog
{
    Q_OBJECT
    //Q_DECLARE_PRIVATE(RadiationSourceTableRenovateDialog)
public:
    explicit RadiationSourceTableRenovateDialog(QWidget *parent = 0);
    ~RadiationSourceTableRenovateDialog();

    void setRadiationSourceList(const unsigned short rsId, const RSDataList& rsList);
private:
    void initTableHeaderInfo();
    void showTablePara();

public slots:
    void recvRSDataList(RSDataList* rsList);
private:
    Ui::RadiationSourceTableRenovateDialog *ui;

    unsigned short usSourceNo;          //辐射源批号
    RSDataList rsDataList;              //辐射源列表
    TableView* dataView;
    TableViewModelRadiationSource * dataViewModel;
};
}//namespace DataView
#endif // RADIATIONSOURCETABLERENOVATEDIALOG_H
