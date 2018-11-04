#include "healthinfopannel.h"

#include "base/constants.h"
#include "Base/actionmanager/action.h"
#include "Base/actionmanager/actioncontainer.h"
#include "Base/actionmanager/actionmanager.h"
#include "Base/util/rsingleton.h"
//
#include <iostream>
using namespace std;

HealthInfoDockPanel::HealthInfoDockPanel(QWidget *parent)
    :Base::RComponent(Constant::PLUGIN_HEALTH_MANAGER,parent)
{
    retranslateUi();
    RSingleton<Base::Subject>::instance()->attach(this);
}

bool HealthInfoDockPanel::initialize()
{
    infoWidget = new HealthState_Display();
    setWidget(infoWidget);

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
    return true;
}

QString HealthInfoDockPanel::pluginName()
{
//    cout<<"1"<<endl;
    return tr("HealthInfo panel");
}

QString HealthInfoDockPanel::MachineName()
{
//    cout<<"2"<<endl;
    return tr("Machine");
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

QSize HealthInfoDockPanel::sizeHint() const
{
    return QSize(300,200);
}

void HealthInfoDockPanel::retranslateUi()
{
//    cout<<"3"<<endl;
    m_name = tr("HealthInfo panel");
    setWindowTitle(m_name);
}
