#include "rcomponent.h"

#include "Base/constants.h"
#include "Base/actionmanager/actioncontainer.h"
#include "Base/actionmanager/actionmanager.h"
#include "Base/actionmanager/action.h"
#include "Base/util/rutil.h"
#include "Base/util/rsingleton.h"
#include <QDebug>

namespace Core{

int RComponent::compIndex = 0;

RComponent::RComponent(const char *id, QObject *parent):
    QObject(parent),m_id(QString("%1_%2").arg(id).arg(compIndex++).toLocal8Bit().data()),dockContainer(NULL)
{
    setObjectName(m_id.toString());
    retranslateUi();
    RSingleton<Core::Subject>::instance()->attach(this);
}

RComponent::~RComponent()
{

}

Id RComponent::id()
{
    return m_id;
}

QString RComponent::getPluginId()
{
    return pluginId;
}

QString RComponent::name()
{
    return m_name;
}

QString RComponent::pluginName()
{
    return m_name;
}

void RComponent::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

} //namespace Base
