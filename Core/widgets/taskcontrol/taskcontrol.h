/*!
 *  @brief     任务控制面板
 *  @details
 *  @author    wlc
 *  @version   1.0
 *  @date      2018.09.20
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TASKCONTROL_H
#define TASKCONTROL_H

#include <QSharedPointer>
#include <QModelIndex>
#include <QWidget>

#include "head.h"
#include "Base/pluginmanager/observer.h"

class QTimerEvent;

namespace TaskControlModel {

class TaskControlPrivate;

class TaskControl : public QWidget , public Base::Observer
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TaskControl)
public:
    explicit TaskControl(QWidget *parent = 0);
    ~TaskControl();

    void retranslateUi();
    void onMessage(MessageType::MessType type);
    
private slots:
    void viewTask(QModelIndex);
    void mouseHoverItem(QModelIndex index);
    void modifyTask();
    void deleteTask();
    void copyTask();
    void pasteTask();

    void importTask();
    void exportTask();
    void distributeTask();
    void resetTask();

    void updateTaskState();
    void dispatchOver();

    void showTaskWindow();

    void tableContextPoint(QPoint point);

private:
    QModelIndex currentIndex(bool &validIndex);
    void showEditWindow(QModelIndex index);

    void initDispatch();

private:
    QSharedPointer<TaskControlPrivate> d_ptr;

};

} //namespace TaskControlModel

#endif // TASKCONTROL_H
