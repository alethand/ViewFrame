/*!
 *  @brief     通用组件窗口类
 *  @details   定义了组件对组件管理者暴露接口
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.24
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef RCOMPONENT_H
#define RCOMPONENT_H

#include <QDockWidget>

#include "Base/actionmanager/id.h"
#include "Base/messagetype.h"
#include "pluginmanager/observer.h"
#include "protocol/datastruct.h"

namespace Core{

class RComponent : public QDockWidget,public Core::Observer
{
    Q_OBJECT
public:
    RComponent(const char * id,QWidget * parent  = 0);
    virtual ~RComponent();

    Id id();
    QString getPluginId();
    QString name();

    virtual bool initialize() = 0;
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
};

} //namespace Base

#endif // RCOMPONENT_H
