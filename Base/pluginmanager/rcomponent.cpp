#include "rcomponent.h"

#include "constants.h"
#include "../actionmanager/actioncontainer.h"
#include "../actionmanager/actionmanager.h"
#include "../actionmanager/action.h"
#include "util/rutil.h"

namespace Base{

RComponent::RComponent(const char *id, QWidget *parent):
    QDockWidget(parent),m_id(id)
{
}

RComponent::~RComponent()
{

}

Id RComponent::id()
{
    return m_id;
}

QString RComponent::name()
{
    return m_name;
}

} //namespace Base
