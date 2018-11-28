/*!
 *  @brief     插件管理器
 *  @details   用于维护加载、初始化、卸载插件
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QMap>
#include <mutex>

#include "Base/actionmanager/id.h"

namespace Core{

class RComponent;

class PluginManager : public QObject
{
public:
    explicit PluginManager(QObject * parent = 0);

    typedef QMap<Id,RComponent *> ComponentMap;

    void load();

    //可用插件
    void addAvailblePlugin(RComponent * component);
    RComponent* getAvailblePlugin(QString pluginId);
    ComponentMap getAllAvailblePlugins();

    //被实例化插件
    void addActivePlugin(RComponent * component);
    RComponent * getActivePlugin(QString objectName);
    ComponentMap getAllActivePlugins();

private:
    ComponentMap availbleComponentMap;
    ComponentMap activeComponentMap;
    std::mutex  m_mutex;
};

} //namespace Base

#endif // PLUGINMANAGER_H
