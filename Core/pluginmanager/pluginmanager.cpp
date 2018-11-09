#include "pluginmanager.h"

#include "rcomponent.h"

namespace Core{

PluginManager::PluginManager(QObject *parent):
    QObject(parent)
{

}

//TODO 加载插件
void PluginManager::load()
{

}

/*!
 * @brief 添加可用的插件
 * @details 插件只创建，但未实例化
 * @param[in]  component 待添加的插件
 */
void PluginManager::addAvailblePlugin(RComponent *component)
{
    if(component == NULL)
        return;

    std::lock_guard<std::mutex> lg(m_mutex);
    if(availbleComponentMap.contains(component->id()))
        return;

    availbleComponentMap.insert(component->id(),component);
}

RComponent *PluginManager::getAvailblePlugin(QString pluginId)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    ComponentMap::iterator iter = availbleComponentMap.begin();
    while(iter != availbleComponentMap.end()){
        if(iter.value()->getPluginId() == pluginId)
            return iter.value();
        iter++;
    }

    return NULL;
}

PluginManager::ComponentMap PluginManager::getAllAvailblePlugins()
{
    std::lock_guard<std::mutex> lg(m_mutex);
    return availbleComponentMap;
}

/*!
 * @brief 根据配置文件，将可用的插件添加至待激活的区
 * @param[in]  component 待添加的插件
 */
void PluginManager::addActivePlugin(RComponent *component)
{
    if(component == NULL)
        return;

    std::lock_guard<std::mutex> lg(m_mutex);
    if(activeComponentMap.contains(component->id()))
        return;

    activeComponentMap.insert(component->id(),component);
}

RComponent *PluginManager::getActivePlugin(QString pluginId)
{
    std::lock_guard<std::mutex> lg(m_mutex);
    ComponentMap::iterator iter = activeComponentMap.begin();
    while(iter != activeComponentMap.end()){
        if(iter.value()->getPluginId() == pluginId)
            return iter.value();
        iter++;
    }

    return NULL;
}

PluginManager::ComponentMap PluginManager::getAllActivePlugins()
{
    std::lock_guard<std::mutex> lg(m_mutex);
    return activeComponentMap;
}

} //namespace Base
