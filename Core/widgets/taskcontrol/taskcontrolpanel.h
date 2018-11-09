/*!
 *  @brief     任务控制模块
 *  @details
 *  @author    wlc
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TASKCONTROLPANEL_H
#define TASKCONTROLPANEL_H

#include "pluginmanager/rcomponent.h"

namespace TaskControlModel {

class TaskControl;

class TaskControlPanel : public Core::RComponent
{
    Q_OBJECT
public:
    explicit TaskControlPanel(QWidget * parent = 0);
    ~TaskControlPanel();

    bool initialize();
    void release();
    QString pluginName();
    Core::RComponent * clone();

    void onMessage(MessageType::MessType type);
    void onNetwork(int protocolType,Datastruct::FieldValues & data);

    QSize sizeHint()const;

private:
    void retranslateUi();

private:
    TaskControl * controlWidget;

};

} //namespace TaskControlModel

#endif // TASKCONTROLPANEL_H
