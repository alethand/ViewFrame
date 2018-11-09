/*!
 *  @details   数据源表格
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.10
 *  @copyright NanJing RenGu.
 */
#ifndef RADIATIONSOURCETABLE_H
#define RADIATIONSOURCETABLE_H

#include "protocol/datastruct.h"
#include <QModelIndex>
using namespace Datastruct;

#include "pluginmanager/rcomponent.h"

namespace DataView {
class RadiationSourceTablePrivate;

class RadiationSourceTable : public Core::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(RadiationSourceTable)
public:
    explicit RadiationSourceTable(QWidget *parent = 0);
    ~RadiationSourceTable();

    bool initialize();
    void release();
    QString pluginName();
    Core::RComponent * clone();
    void onMessage(MessageType::MessType type);
    void onNetwork(int protocolType,Datastruct::FieldValues & data);

    void changeTableHeaderInfo(bool blAddColFlag);
    void recvRSPara(char *buff,int len);

    bool theblAddColFlag;

private slots:
    void on_radioButtonScrollReno_clicked();
    void on_radioButtonCoverReno_clicked();
    void viewRSData(QModelIndex index);
    void clearTable();
    void on_btn_load_clicked();

private:
    void initRSTable();
    void showTSPara(const RadiationSource::RadiationSourceBase& rsData);
    QString getCurrentDate();
    void retranslateUi();
    void showRSDialog(QModelIndex index);
    void showRowCounter();
signals:
    void sendRSDataList(RSDataList* rsList);
private:
    RadiationSourceTablePrivate * d_ptr;
};

} //namespace DataView

#endif // RADIATIONSOURCETABLE_H
