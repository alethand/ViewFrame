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
    pluginId = "0x0002";
    m_name = tr("TaskControl panel");
    RSingleton<Core::Subject>::instance()->attach(this);
}

TaskControlPanel::~TaskControlPanel()
{

}

QWidget * TaskControlPanel::initialize(QWidget *parent)
{
    controlWidget = new TaskControl(parent);
    return controlWidget;
}

void TaskControlPanel::release()
{

}

QString TaskControlPanel::pluginName()
{
    return QObject::tr("TaskControl panel");
}

Core::RComponent * TaskControlPanel::clone()
{
    return new TaskControlPanel();
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

}
