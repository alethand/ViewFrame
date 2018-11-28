#include "taskcontrolpanel.h"

#include <Base/constants.h>
#include <Base/util/rsingleton.h>
#include "taskcontrol.h"

#include <QDebug>

using namespace TaskControlModel;

TaskControlPanel::TaskControlPanel(QWidget *parent):
    RComponent(Constant::PLUGIN_TASK_CONTROL,parent),controlWidget(nullptr)
{
    pluginId = "0x0002";
    m_name = tr("TaskControl panel");
}

TaskControlPanel::~TaskControlPanel()
{

}

QWidget * TaskControlPanel::initialize(QWidget *parent)
{
    controlWidget = new TaskControl(parent);
    return controlWidget;
}

QString TaskControlPanel::pluginName()
{
    return QObject::tr("TaskControl panel");
}

Core::RComponent * TaskControlPanel::clone()
{
    return new TaskControlPanel();
}


void TaskControlPanel::onNetwork(int protocolType, Core::Datastruct::FieldValues &data)
{

}

QSize TaskControlPanel::sizeHint() const
{
    return QSize(350,200);
}

void TaskControlPanel::retranslateUi()
{

}
