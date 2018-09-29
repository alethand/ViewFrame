#include "action.h"

#include <QAction>

Action::Action(Id id, QObject *parent):
    CommandPrivate(id,parent),m_action(nullptr),initKey(false)
{

}

Action::~Action()
{

}

void Action::setDefaultKey(QKeySequence dkey)
{
    if(!initKey){
        m_defaultKey = dkey;
        initKey = true;
    }
    if(m_action)
        m_action->setShortcut(dkey);
}

QKeySequence Action::defaultKey()
{
    return m_defaultKey;
}

void Action::resetDefaultKey()
{
    if(m_action && initKey)
        m_action->setShortcut(m_defaultKey);
}

void Action::setAction(QAction *act)
{
    m_action = act;
}

QAction *Action::action() const
{
    return m_action;
}

QShortcut *Action::shortcut() const
{
    return 0;
}
