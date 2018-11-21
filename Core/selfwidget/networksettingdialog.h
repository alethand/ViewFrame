/*!
 *  @brief     网络配置页面
 *  @details   1.加载本地xml文件；
 *             2.新增或删除网络节点；
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.20
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef NETWORKSETTINGDIALOG_H
#define NETWORKSETTINGDIALOG_H

#include "Base/selfwidget/dialogproxy.h"
#include <QSharedPointer>

class QListWidgetItem;

namespace Datastruct{
struct NetworkInfo;
}

namespace Core{

class NetworkSettingDialogPrivate;

class NetworkSettingDialog : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(NetworkSettingDialog)
public:
    explicit NetworkSettingDialog(QWidget *parent = 0);
    ~NetworkSettingDialog();

    QSize sizeHint()const;

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void respOk();
    void respCancel();
    void respListItemPressed(QListWidgetItem * item);
    void controlMultiSetting(bool flag);

    void applyCurrentModify();
    void cancelCurrentModify();

    void newNetwork();
    void removeNetwork();
    void switchListRow(int row);
    void switchMultiState(int index);

private:
    void updateNetworks();
    void releaseTmpData();
    void copyDataFromConfig();
    void fillData(Datastruct::NetworkInfo * info);
    bool validateUserInput();

private:
    QSharedPointer<NetworkSettingDialogPrivate> d_ptr;

};

} //namespace Core

#endif // NETWORKSETTINGDIALOG_H
