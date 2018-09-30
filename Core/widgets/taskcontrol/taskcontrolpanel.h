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

#include "Base/pluginmanager/rcomponent.h"

using namespace Base;

namespace TaskControlModel {

class TaskControl;

class TaskControlPanel : public RComponent
{
    Q_OBJECT
public:
    explicit TaskControlPanel(QWidget * parent = 0);
    ~TaskControlPanel();

    bool initialize();
    void release();
    QString pluginName();

    void onMessage(MessageType::MessType type);

    QSize sizeHint()const;

private:
    void retranslateUi();

private:
    TaskControl * controlWidget;

};

} //namespace TaskControlModel

#endif // TASKCONTROLPANEL_H
