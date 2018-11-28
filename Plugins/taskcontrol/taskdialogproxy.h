/*!
 *  @brief     任务窗口代理
 *  @details   1.负责对传递的文件解析；
 *             2.生成显示界面；
 *             3.验证并存储用户输入；
 *             4.显示历史输入信息。
 *  @author    wey
 *  @version   1.0
 *  @date      2018.10.31
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TASKDIALOGPROXY_H
#define TASKDIALOGPROXY_H

#include <QSharedPointer>
#include "head.h"
#include <Core/protocol/datastruct.h>
#include <Base/selfwidget/dialogproxy.h>

namespace TaskControlModel {

struct Container;
class TaskDialogProxyPrivate;

class TaskDialogProxy  : public DialogProxy
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TaskDialogProxy)
public:
    explicit TaskDialogProxy(QWidget *parent = 0);
    ~TaskDialogProxy();

    bool parseLayout(QString & fileName);
    void showMe();

    void setTaskInfo(NewTaskInfo * info);
    NewTaskInfo * getTaskInfo();

protected:
    void respButtClicked(StandardButton butt);

private slots:
    void respOk();
    void respCancel();

private:
    void fillFiled(Core::Datastruct::Container * container);
    void findFiled(Core::Datastruct::Container * container);

private:
    QSharedPointer<TaskDialogProxyPrivate> d_ptr;
};

}

#endif // TASKDIALOGPROXY_H
