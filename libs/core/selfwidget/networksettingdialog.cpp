#include "networksettingdialog.h"

#include <core/pluginmanager/pluginloader.h>
#include <base/util/rsingleton.h>

#include <QLabel>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QListView>
#include <QPushButton>
#include <QRadioButton>
#include <QCheckBox>
#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QMessageBox>

#include "Base/util/rutil.h"
#include "Base/selfwidget/ripwidget.h"
#include "Base/common/validator/rcombinevalidator.h"

namespace Core{

typedef QMap<QString,Datastruct::NetworkInfo *> PNetworkMap;

class NetworkSettingDialogPrivate
{
    Q_DECLARE_PUBLIC(NetworkSettingDialog)
public:
    NetworkSettingDialogPrivate(NetworkSettingDialog * q):q_ptr(q),clickedButt(DialogProxy::NoButton),tmpMapData(NULL),tmpNewData(NULL),
        isNewElement(false){
        init();
    }

    void init();

    void clearContent();

    QWidget * leftContainer;
    QListWidget * networkList;

    NetworkSettingDialog * q_ptr;
    DialogProxy::StandardButton clickedButt;

    QComboBox * protocolBox;
    QComboBox * typeBox;
    QLineEdit * idEdit;
    RIPWidget * ipWidget;
    QLineEdit * portEdit;
    QCheckBox * multiCheckBox;
    RIPWidget * multiIpWidget;
    QLabel * multiIpLabel;

    QPushButton * applyButton;
    QPushButton * resetButton;

    PNetworkMap * tmpMapData;
    Datastruct::NetworkInfo * tmpNewData;
    bool isNewElement;
};

void NetworkSettingDialogPrivate::init()
{
    QWidget * mainWidget = new QWidget(q_ptr);
    mainWidget->setObjectName("mainWidget");

    //左侧控制区
    leftContainer = new QWidget(mainWidget);
    leftContainer->setFixedWidth(120);

    networkList = new QListWidget();
    networkList->setFocusPolicy(Qt::NoFocus);
    QObject::connect(networkList,SIGNAL(itemPressed(QListWidgetItem*)),q_ptr,SLOT(respListItemPressed(QListWidgetItem *)));
    QObject::connect(networkList,SIGNAL(currentRowChanged(int)),q_ptr,SLOT(switchListRow(int)));
    networkList->setObjectName("networsetting_netlist");

    QWidget * leftControlWidget = new QWidget(leftContainer);

    leftControlWidget->setFixedHeight(22);
    QPushButton * addNetwork = new QPushButton(leftControlWidget);
    QObject::connect(addNetwork,SIGNAL(pressed()),q_ptr,SLOT(newNetwork()));
    addNetwork->setText("+");

    QPushButton * removeNetwork = new QPushButton(leftControlWidget);
    QObject::connect(removeNetwork,SIGNAL(pressed()),q_ptr,SLOT(removeNetwork()));
    removeNetwork->setText("-");

    QHBoxLayout * toolLayout = new QHBoxLayout;
    toolLayout->setContentsMargins(1,1,1,1);
    toolLayout->addWidget(addNetwork);
    toolLayout->addWidget(removeNetwork);
    leftControlWidget->setLayout(toolLayout);

    QVBoxLayout * leftLayout = new QVBoxLayout(leftContainer);
    leftLayout->setContentsMargins(1,1,1,1);
    leftLayout->addWidget(networkList);
    leftLayout->addWidget(leftControlWidget);
    leftContainer->setLayout(leftLayout);

    //右侧信息显示区
    QWidget  * rightContainer = new QWidget(mainWidget);
    rightContainer->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);

    QGridLayout * rightLayout = new QGridLayout;

    int widgetFixedHeight = 23;
    //protocol
    QLabel * protocolLabel = new QLabel;
    protocolLabel->setFixedHeight(widgetFixedHeight);
    protocolLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    protocolLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    protocolLabel->setText(QObject::tr("Protocol:"));

    protocolBox = new QComboBox;
    QObject::connect(protocolBox,SIGNAL(activated(int)),q_ptr,SLOT(switchMultiState(int)));
    protocolBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    protocolBox->setView(new QListView());
    QStringList protList;
    protocolBox->addItem("Tcp",Datastruct::N_TCP);
    protocolBox->addItem("Udp",Datastruct::N_UDP);
    protocolBox->addItems(protList);

    //type
    QLabel * typeLabel = new QLabel;
    typeLabel->setFixedHeight(widgetFixedHeight);
    typeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    typeLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    typeLabel->setText(QObject::tr("Type:"));

    typeBox = new QComboBox;
    typeBox->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Preferred);
    typeBox->setView(new QListView());
    QStringList typeList;
    typeList<<"Server"<<"Client";
    typeBox->addItems(typeList);

    //id
    QLabel * idLabel = new QLabel;
    idLabel->setFixedHeight(widgetFixedHeight);
    idLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    idLabel->setText(QObject::tr("Network id:"));

    idEdit = new QLineEdit;

    //ip
    QLabel * ipLabel = new QLabel;
    ipLabel->setFixedHeight(widgetFixedHeight);
    ipLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ipLabel->setText(QObject::tr("Ip address:"));

    ipWidget = new RIPWidget;

    //port
    QLabel * portLabel = new QLabel;
    portLabel->setFixedHeight(widgetFixedHeight);
    portLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    portLabel->setText(QObject::tr("Port:"));

    portEdit = new QLineEdit;

    //multicast
    QLabel * multiLabel = new QLabel;
    multiLabel->setFixedHeight(widgetFixedHeight);
    multiLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    multiLabel->setText(QObject::tr("Multicast:"));

    multiCheckBox = new QCheckBox;

    multiIpLabel = new QLabel;
    multiIpLabel->setText(QObject::tr("Multicast ip:"));

    multiIpWidget = new RIPWidget;

    QObject::connect(multiCheckBox,SIGNAL(clicked(bool)),q_ptr,SLOT(controlMultiSetting(bool)));

    applyButton = new QPushButton;
    applyButton->setText(QObject::tr("Apply"));
    QObject::connect(applyButton,SIGNAL(pressed()),q_ptr,SLOT(applyCurrentModify()));

    resetButton = new QPushButton;
    resetButton->setText(QObject::tr("Cancel"));
    QObject::connect(resetButton,SIGNAL(pressed()),q_ptr,SLOT(cancelCurrentModify()));

    rightLayout->addWidget(protocolLabel,0,0,1,1);
    rightLayout->addWidget(protocolBox,0,1,1,1);

    rightLayout->addWidget(typeLabel,0,2,1,1);
    rightLayout->addWidget(typeBox,0,3,1,1);

    rightLayout->addWidget(idLabel,2,0,1,1);
    rightLayout->addWidget(idEdit,2,1,1,1);

    rightLayout->addWidget(ipLabel,3,0,1,1);
    rightLayout->addWidget(ipWidget,3,1,1,3);

    rightLayout->addWidget(portLabel,4,0,1,1);
    rightLayout->addWidget(portEdit,4,1,1,1);

    rightLayout->addWidget(multiLabel,5,0,1,1);
    rightLayout->addWidget(multiCheckBox,5,1,1,1);

    rightLayout->addWidget(multiIpLabel,6,0,1,1);
    rightLayout->addWidget(multiIpWidget,6,1,1,3);

    rightLayout->setVerticalSpacing(12);
    rightLayout->setRowStretch(7,1);

    rightLayout->addWidget(resetButton,8,2,1,1);
    rightLayout->addWidget(applyButton,8,3,1,1);

    rightContainer->setLayout(rightLayout);

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(4,4,4,4);
    mainLayout->addWidget(leftContainer);
    mainLayout->addWidget(rightContainer);
    mainWidget->setLayout(mainLayout);

    q_ptr->setContentWidget(mainWidget);
}

void NetworkSettingDialogPrivate::clearContent()
{
    protocolBox->setCurrentIndex(0);
    typeBox->setCurrentIndex(0);
    idEdit->clear();
    ipWidget->clear();
    portEdit->clear();
    multiCheckBox->setChecked(false);
    multiIpWidget->clear();
    q_ptr->controlMultiSetting(false);
}

NetworkSettingDialog::NetworkSettingDialog(QWidget *parent):DialogProxy(parent),
    d_ptr(new NetworkSettingDialogPrivate(this))
{
    setTitle(tr("Net settings"));
    setButton(DialogProxy::Ok|DialogProxy::Close);
    controlMultiSetting(false);
    updateNetworks();

    setAttribute(Qt::WA_DeleteOnClose);
}

NetworkSettingDialog::~NetworkSettingDialog()
{
    releaseTmpData();
}

QSize NetworkSettingDialog::sizeHint() const
{
    QRect screenSize = RUtil::screenGeometry();
    return QSize(screenSize.width() * 0.28,screenSize.height() * 0.42);
}

void NetworkSettingDialog::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(NetworkSettingDialog);
    d->clickedButt = butt;
    switch(butt){
        case DialogProxy::Ok:
                respOk();
            break;
        case DialogProxy::Close:
                respCancel();
            break;
        default:
            break;
    }
}

void NetworkSettingDialog::respOk()
{
    Q_D(NetworkSettingDialog);

    if(!validateUserInput())
        return;

    NetworkMap * maps = RSingleton<Core::PluginLoader>::instance()->getNetworks();
    maps->clear();

    PNetworkMap::iterator iter = d->tmpMapData->begin();
    while(iter != d->tmpMapData->end()){
        maps->insert(iter.key(),*(iter.value()));
        iter++;
    }
    if(!RSingleton<Core::PluginLoader>::instance()->saveConfigFile())
        return;

    d->clearContent();
    close();
}

void NetworkSettingDialog::respCancel()
{
    Q_D(NetworkSettingDialog);
    d->clearContent();
    close();
}

void NetworkSettingDialog::respListItemPressed(QListWidgetItem *item)
{
    Q_D(NetworkSettingDialog);
    if(d->tmpMapData->contains(item->text())){
        d->clearContent();
        Datastruct::NetworkInfo * netInfo = d->tmpMapData->value(item->text());
        d->idEdit->setText(netInfo->id);
        d->ipWidget->setIP(netInfo->baseInfo.ip);
        d->protocolBox->setCurrentIndex(static_cast<int>(netInfo->protocol));
        d->typeBox->setCurrentIndex(static_cast<int>(netInfo->baseInfo.connectionType));
        d->portEdit->setText(QString::number(netInfo->baseInfo.port));
        d->multiCheckBox->setChecked(netInfo->baseInfo.multicast);
        controlMultiSetting(netInfo->baseInfo.multicast);
        d->multiIpWidget->setIP(netInfo->baseInfo.multicastIp);
    }
}

/*!
 * @brief 控制多播设置是否可见
 * @param[in]  flag 为true启用
 */
void NetworkSettingDialog::controlMultiSetting(bool flag)
{
    Q_D(NetworkSettingDialog);
    d->multiIpLabel->setVisible(flag);
    d->multiIpWidget->setVisible(flag);
    d->multiIpWidget->clear();
}

/*!
 * @brief 应用当前修改
 * @details 1.若在当前基础上修改，则保存更新;
 *          2.若新建item上修改，则创建节点保存;
 */
void NetworkSettingDialog::applyCurrentModify()
{
    Q_D(NetworkSettingDialog);

    if(!validateUserInput())
        return;

    d->isNewElement = false;

    if(d->networkList->isVisible()){
        if(d->networkList->selectedItems().size() != 1)
            return;

        QListWidgetItem * selectedItem = d->networkList->selectedItems().at(0);
        QString selectedKey = selectedItem->text();
        if(!d->tmpMapData->contains(selectedKey))
            return;

        Datastruct::NetworkInfo * info = d->tmpMapData->value(selectedKey);
        fillData(info);

        selectedItem->setText(info->id);
        d->tmpMapData->remove(selectedKey);
        d->tmpMapData->insert(info->id,info);
    }else{
        d->tmpNewData = new Datastruct::NetworkInfo;
        fillData(d->tmpNewData);

        d->networkList->addItem(d->tmpNewData->id);
        d->tmpMapData->insert(d->tmpNewData->id,d->tmpNewData);
        d->clearContent();
        d->leftContainer->show();
        d->networkList->setCurrentRow(d->tmpMapData->size() - 1);

        d->tmpNewData = NULL;
    }
}

/*!
 * @brief 取消当前设置
 */
void NetworkSettingDialog::cancelCurrentModify()
{
    Q_D(NetworkSettingDialog);
    d->isNewElement = false;
    d->clearContent();
    d->leftContainer->show();
}

/*!
 * @brief 创建新的网路节点
 */
void NetworkSettingDialog::newNetwork()
{
    Q_D(NetworkSettingDialog);
    d->isNewElement = true;
    d->clearContent();
    d->leftContainer->hide();
}

void NetworkSettingDialog::removeNetwork()
{
    Q_D(NetworkSettingDialog);

    if(d->networkList->selectedItems().size() != 1)
        return;

    QListWidgetItem * curItem = d->networkList->selectedItems().at(0);
    QString delItem = curItem->text();
    if(d->tmpMapData->contains(delItem)){

        int result = QMessageBox::question(this,tr("question"),tr("Are you sure you want to delete selected item?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if(result == QMessageBox::No)
            return;

        delete d->tmpMapData->value(delItem);
        d->tmpMapData->remove(delItem);
        d->clearContent();
        d->networkList->takeItem(d->networkList->currentRow());
    }
}

/*!
 * @brief 响应ListWidget中行的改变
 * @param[in] row 新行
 */
void NetworkSettingDialog::switchListRow(int row)
{
    Q_D(NetworkSettingDialog);
    if(row >= 0)
        respListItemPressed(d->networkList->item(row));
}

void NetworkSettingDialog::switchMultiState(int)
{
    Q_D(NetworkSettingDialog);
    Datastruct::NetworkType type = static_cast<Datastruct::NetworkType>(d->protocolBox->currentData().toInt());
    bool enable = type == Datastruct::N_UDP;
    d->multiCheckBox->setEnabled(enable);
    if(!enable)
        controlMultiSetting(false);
    else
        controlMultiSetting(d->multiCheckBox->isChecked());
}

void NetworkSettingDialog::updateNetworks()
{
    Q_D(NetworkSettingDialog);

    copyDataFromConfig();

    QList<QString> keys = d->tmpMapData->keys();
    std::for_each(keys.begin(),keys.end(),[&](QString item){
        d->networkList->addItem(item);
    });

    if(d->networkList->count() > 0)
        d->networkList->setCurrentRow(0);
}

/*!
 * @brief 释放缓冲区内容
 */
void NetworkSettingDialog::releaseTmpData()
{
    Q_D(NetworkSettingDialog);
    if(d->tmpMapData){
        PNetworkMap::iterator iter = d->tmpMapData->begin();
        while(iter != d->tmpMapData->end()){
            delete iter.value();
            iter = d->tmpMapData->erase(iter);
        }
        d->tmpMapData->clear();
    }
}

/*!
 * @brief 从配置文件中拷贝最新的数据，并在此基础上修改
 */
void NetworkSettingDialog::copyDataFromConfig()
{
    Q_D(NetworkSettingDialog);

    if(d->tmpMapData == NULL){
        d->tmpMapData = new PNetworkMap;
    }else{
        releaseTmpData();
    }

    NetworkMap * maps = RSingleton<Core::PluginLoader>::instance()->getNetworks();

    NetworkMap::iterator iter = maps->begin();
    while(iter != maps->end()){
        Datastruct::NetworkInfo * netWork = new Datastruct::NetworkInfo;
        *netWork = iter.value();

        d->tmpMapData->insert(iter.key(),netWork);
        iter++;
    }
}

/*!
 * @brief 获取最新的字段内容
 * @param[in]  info 暂存区
 */
void NetworkSettingDialog::fillData(Datastruct::NetworkInfo *info)
{
    Q_D(NetworkSettingDialog);
    info->id =d->idEdit->text();
    info->baseInfo.ip = d->ipWidget->getIPString();
    info->baseInfo.port = d->portEdit->text().toUShort();
    info->baseInfo.connectionType = static_cast<Datastruct::NetworkConnectionType>(d->typeBox->currentIndex());
    info->baseInfo.multicast = d->multiCheckBox->isChecked();
    if(d->multiCheckBox->isChecked())
        info->baseInfo.multicastIp = d->multiIpWidget->getIPString();
    info->protocol = static_cast<Datastruct::NetworkType>(d->protocolBox->currentIndex());
}

/*!
 * @brief 验证用户输入的信息是否正确
 * @return 验证结果
 */
bool NetworkSettingDialog::validateUserInput()
{
    Q_D(NetworkSettingDialog);

    QString errorText;
    do{
        {
            RAndCombineValidator validator;
            validator.addValidator(new RNumericValidator<ushort>(d->portEdit->text().toInt(),RValid::Ge,1024,RValid::Le));
            if(validator.validate() != RValid::Acceptable){
                errorText = tr("port input range is invalid!");
                break;
            }
        }

        if(d->isNewElement){
            QString nid = d->idEdit->text();
            if(d->tmpMapData->contains(nid)){
                errorText = tr("network id has existed!");
                break;
            }
        }

        {
            RAndCombineValidator validator;
            validator.addValidator(new RStringValidator(d->ipWidget->getIPString(),RValid::R_IP));
            if(validator.validate() != RValid::Acceptable){
                errorText = tr("network ip is invalid!");
                break;
            }
        }

        {
            if(d->multiCheckBox->isChecked()){
                RAndCombineValidator validator;
                validator.addValidator(new RStringValidator(d->multiIpWidget->getIPString(),RValid::R_IP));
                if(validator.validate() != RValid::Acceptable){
                    errorText = tr("network ip is invalid!");
                    break;
                }
            }
        }

        return true;
    }while(0);

    QMessageBox::warning(this,tr("warning"),errorText,QMessageBox::Yes);
    return false;
}

} //namespace Core
