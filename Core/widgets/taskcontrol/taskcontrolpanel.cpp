﻿#include "taskcontrolpanel.h"

#include "Base/constants.h"
#include "taskcontrol.h"
#include "Base/util/rsingleton.h"

using namespace TaskControlModel;

TaskControlPanel::TaskControlPanel(QWidget *parent):
    RComponent(Constant::PLUGIN_TASK_CONTROL,parent),controlWidget(nullptr)
{
    retranslateUi();
    RSingleton<Base::Subject>::instance()->attach(this);
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

void TaskControlPanel::onMessage(MessageType::MessageType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

QSize TaskControlPanel::sizeHint() const
{
    return QSize(350,200);
}

void TaskControlPanel::retranslateUi()
{
    m_name = tr("TaskControl panel");
    setWindowTitle(m_name);
}
