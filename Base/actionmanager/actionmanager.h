/*!
 *  @brief     工具按钮管理
 *  @details   管理所创建的工具按钮，根据ID返回对应的工具按钮
 *  @file      toolactionmanager.h
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note      20171219:wey:添加创建和获取action;
 */
#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include <QObject>
#include <QMenu>
#include <QMap>
#include <QKeySequence>
#include <QMenuBar>
#include "id.h"
#include "../base_global.h"

class QToolButton;
class LoginDialog;
class Action;
class ActionContainer;

class BASESHARED_EXPORT ActionManager : public QObject
{
    Q_OBJECT
public:
    typedef QMap<Id,Action*> ActionMap;
    typedef QMap<Id,Action*>::const_iterator ActionMapIterator;

    typedef QMap<Id,ActionContainer*> ActionContainerMap;
    typedef QMapIterator<Id,ActionContainer*> ActionContainerMapIterator;

    static ActionManager * instance();
    ~ActionManager();

    Action * registAction(Id id, QAction * newAction);
    Action * action(Id id);

    ActionContainer * createMenu(Id id);
    ActionContainer * createMenuBar(Id id);

    ActionContainer * actionContainer(Id id);

    ActionMap getAllActions(){return actions;}

    void releaseMenu(Id id);

private slots:
    void itemDestroyed();

private:
    ActionManager(QObject * parent = 0);

    static ActionManager * actionManager;

    ActionMap actions;
    ActionContainerMap menus;
};

#endif // TOOLACTIONMANAGER_H
