#include "taskcontrolpanel.h"

#include "Base/constants.h"
#include "taskcontrol.h"
#include "Base/util/rsingleton.h"

#include <QDebug>

using namespace TaskControlModel;

TaskControlPanel::TaskControlPanel(QWidget *parent):
    RComponent(Constant::PLUGIN_TASK_CONTROL,parent),controlWidget(nullptr)
{
    retranslateUi();
    RSingleton<Core::Subject>::instance()->attach(this);
}

TaskControlPanel::~TaskControlPanel()
{

}

bool TaskControlPanel::initialize()
{
    controlWidget = new TaskControl(this);
    setWidget(controlWidget);

    return true;
}

void TaskControlPanel::release()
{

}

QString TaskControlPanel::pluginName()
{
    return tr("TaskControl panel");
}

Core::RComponent * TaskControlPanel::clone()
{
    return new TaskControlPanel;
}

void TaskControlPanel::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void TaskControlPanel::onNetwork(int protocolType, Datastruct::FieldValues &data)
{

}

QSize TaskControlPanel::sizeHint() const
{
    return QSize(350,200);
}

void TaskControlPanel::retranslateUi()
{
    setObjectName("taskControl");
    m_name = tr("TaskControl panel");
    pluginId = "0x0002";
    setWindowTitle(m_name);
}
