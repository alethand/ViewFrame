#include "healthinfopannel.h"

#include "base/constants.h"
#include "Base/actionmanager/action.h"
#include "Base/actionmanager/actioncontainer.h"
#include "Base/actionmanager/actionmanager.h"
#include "Base/util/rsingleton.h"
//
#include <iostream>
using namespace std;

#include "file/programfilepath.h"
#include "protocol/protocolmanager.h"

HealthInfoDockPanel::HealthInfoDockPanel(QWidget *parent)
    :Core::RComponent(Constant::PLUGIN_HEALTH_MANAGER,parent)
{
    retranslateUi();
    RSingleton<Core::Subject>::instance()->attach(this);
}

QWidget *HealthInfoDockPanel::initialize(QWidget *parent)
{
    //TODO 【20181108将字段设置至表格上】
    ProgramFilePath filePath;
    RSingleton<Core::ProtocolManager>::instance()->parseLocalDir(filePath.healthManagePath);

    bool existed = false;
    Datastruct::BaseProtocol bprotocol = RSingleton<Core::ProtocolManager>::instance()->getProtocol(QStringLiteral("健康管理"),existed);

    infoWidget = new HealthState_Display(parent);

    //todo 以下需要删除
    infoWidget->openDynamicLayout(true);
    HealthData  netdata;
    netdata.workStateNums = 100;
    WorkStruct temp;
    for(int i=0;i< netdata.workStateNums;i++)
    {   temp.name = QString::number(i);
        netdata.wsInfo.append(temp);
    }
    netdata.subMachineNums = 10;

    for(int i=0;i<netdata.subMachineNums;i++)
    {
        SubMachine sm;
        Componet cp;
        sm.name = QStringLiteral("分机")+QString::number(i);
//        sm.name = MachineName()+QString::number(i).toStdString().data();
        sm.componetNum = 10;
        for(int j = 0;j<sm.componetNum;j++)
        {
            cp.name = "CP"+QString::number(i)+"--"+QString::number(j);
            sm.compInfo.append(cp);
        }
        netdata.smInfo.append(sm);
    }

    infoWidget->handleNetData(netdata);//注意这里调用的是测试接口

    connect(this,SIGNAL(sendForHealthPanelResize()),infoWidget,SLOT(recForHealthPanelResize()));

    return infoWidget;
}

QString HealthInfoDockPanel::pluginName()
{
    return tr("HealthInfo panel");
}

QString HealthInfoDockPanel::MachineName()
{
    return tr("Machine");
}

Core::RComponent *HealthInfoDockPanel::clone()
{
    return new HealthInfoDockPanel();
}

void HealthInfoDockPanel::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void HealthInfoDockPanel::onNetwork(int protocolType, Datastruct::FieldValues &data)
{

}

QSize HealthInfoDockPanel::sizeHint() const
{
    return QSize(300,200);
}

void HealthInfoDockPanel::retranslateUi()
{
    setObjectName("healthControl");
    m_name = tr("HealthInfo panel");
    pluginId = "0x0001";
//    setWindowTitle(m_name);
}

/**
 * @brief 健康管理界面布局重置
 */
void HealthInfoDockPanel::recForHealthPanelResize(){
    emit sendForHealthPanelResize();
}
