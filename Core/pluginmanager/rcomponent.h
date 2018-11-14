/*!
 *  @brief     通用组件窗口类
 *  @details   定义了组件对组件管理者暴露接口
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 *      20181113:wey:将父类从QDockWidget改为QObject，将继承关系改为依赖
 */
#ifndef RCOMPONENT_H
#define RCOMPONENT_H

#include "Base/actionmanager/id.h"
#include "Base/messagetype.h"
#include "pluginmanager/observer.h"
#include "protocol/datastruct.h"
#include "selfwidget/mydockwidget.h"

namespace Core{

class MyDockWidget;

class RComponent : public QObject,public Core::Observer
{
    Q_OBJECT
public:
    RComponent(const char * id,QObject * parent  = 0);
    virtual ~RComponent();

    Id id();
    QString getPluginId();
    QString name();
    void setDockContainer(MyDockWidget * container){this->dockContainer = container;}
    MyDockWidget * getDockContainer(){return this->dockContainer;}

    virtual QWidget * initialize(QWidget * parent) = 0;
    virtual void release() = 0;
    virtual QString pluginName() = 0;
    virtual RComponent * clone() = 0;

    virtual void onMessage(MessageType::MessType type) = 0;
    virtual void onNetwork(int protocolType,Datastruct::FieldValues & data) = 0;

protected:
    Id m_id;
    QString pluginId;
    QString m_name;               /*!< dockwidget titlebar */
    static int compIndex;         /*!< 组件索引 */
    MyDockWidget * dockContainer;    /*!<  插件绑定的容器 */
};

} //namespace Base

#endif // RCOMPONENT_H
