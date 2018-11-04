#include "taskcontrol.h"

#include <QMenu>
#include <QTimer>
#include <QDebug>
#include <QDateTime>
#include <QModelIndex>
#include <QTimerEvent>
#include <QPushButton>
#include <QGridLayout>
#include <QList>
#include <QMessageBox>
#include <QFileDialog>
#include <QDataStream>
#include <QDir>

#include "Base/util/rsingleton.h"
#include "modelview/tableview.h"
#include "modelview/tableviewmodel.h"
#include "modelview/tableviewdelegate.h"
#include "utils.h"

#include "bandcontroldialog.h"
#include "turntablecontroldialog.h"
#include "statecontroldialog.h"
#include "playbackcontroldialog.h"
#include "selfcheckcontroldialog.h"
#include "instrumentcontroldialog.h"
#include "gathercontroldialog.h"
#include "net/taskdispatcher.h"
#include "Base/util/fileutils.h"
#include "Core/file/programfilepath.h"
#include "taskdialogproxy.h"
#include "taskparsedmethod.h"

namespace TaskControlModel {

const char ACTION_TYPE[] = "ActionType";
bool IsDistributing;            /*!< 是否正在下发任务 */


class TaskControlPrivate
{
    Q_DECLARE_PUBLIC(TaskControl)
public:
    explicit TaskControlPrivate(TaskControl * q):q_ptr(q),cacheTaskInfo(NULL),taskDispatcher(NULL)
    {
        initTableView();
        initTableViewMenu();
        initView();
    }

    void initView();
    void initTableView();
    void initTableViewMenu();

    TableView* taskView;
    TableViewModel * taskViewModel;
    TableViewDelegate * taskViewDelegate;

    QPushButton * distrbuteButt;
    QPushButton * resetButt;
    QPushButton * importButt;
    QPushButton * exportButt;
    QWidget * m_centralWidget;

    QWidget * mainWidget;

    NewTaskList taskInfoList;           /*!< 任务集合 */
    QPoint contextPoint;                /*!< 表格右键事件菜单 */
    NewTaskInfo * cacheTaskInfo;        /*!< 复制的任务信息,做缓存用 */
    TaskDispatcher * taskDispatcher;    /*!< 任务下发 */

    QAction *addTaskAction;
    QAction *delTaskAction;
    QAction *modifyAction;
    QAction *copyAction;
    QAction *pasteAction;

    TaskControl * q_ptr;

};

void TaskControlPrivate::initView()
{
    mainWidget = new QWidget(q_ptr);
    mainWidget->setObjectName("Container");

    const int buttWidth = 120;
    const int buttHeight = 26;

    importButt = new QPushButton(mainWidget);
    QObject::connect(importButt, SIGNAL(pressed()), q_ptr, SLOT(importTask()));
    importButt->setMinimumWidth(buttWidth);
    importButt->setFixedHeight(buttHeight);


    exportButt = new QPushButton(mainWidget);
    QObject::connect(exportButt, SIGNAL(pressed()), q_ptr, SLOT(exportTask()));
    exportButt->setMinimumWidth(buttWidth);
    exportButt->setFixedHeight(buttHeight);


    distrbuteButt = new QPushButton(mainWidget);
    QObject::connect(distrbuteButt, SIGNAL(pressed()), q_ptr, SLOT(distributeTask()));
    distrbuteButt->setMinimumWidth(buttWidth);
    distrbuteButt->setFixedHeight(buttHeight);


    resetButt = new QPushButton(mainWidget);
    QObject::connect(resetButt, SIGNAL(pressed()), q_ptr, SLOT(resetTask()));
    resetButt->setMinimumWidth(buttWidth);
    resetButt->setFixedHeight(buttHeight);


    QWidget * toolWidget = new QWidget;
    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(2,2,2,2);
    hlayout->addStretch(1);
    hlayout->addWidget(importButt);
    hlayout->addWidget(exportButt);
    hlayout->addWidget(distrbuteButt);
    hlayout->addWidget(resetButt);
    hlayout->addStretch(1);
    toolWidget->setLayout(hlayout);

    distrbuteButt->show();

    QVBoxLayout * vLayout = new QVBoxLayout(mainWidget);
    vLayout->addWidget(taskView);
    vLayout->addWidget(toolWidget);
    vLayout->setSpacing(6);
    vLayout->setContentsMargins(1, 1, 1, 1);

    mainWidget->setLayout(vLayout);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(mainWidget);
    q_ptr->setLayout(layout);
}

void TaskControlPrivate::initTableView()
{
    taskView = new TableView(q_ptr);
    taskView->setMouseTracking(true);
    taskView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    taskViewModel = new TableViewModel;
    taskViewDelegate = new TableViewDelegate(taskView);

    taskView->setModel(taskViewModel);
    taskView->setItemDelegate(taskViewDelegate);
    QObject::connect(taskView,SIGNAL(contextMenuPos(QPoint)),q_ptr,SLOT(tableContextPoint(QPoint)));
    QObject::connect(taskView, SIGNAL(tableCheckDoubleSignal(QModelIndex)), q_ptr, SLOT(viewTask(QModelIndex)));
    QObject::connect(taskView,SIGNAL(entered(QModelIndex)),q_ptr,SLOT(mouseHoverItem(QModelIndex)));
}

/*!
 * @brief 初始化表格右键菜单
 * @details 系统检测指定目录(config/protocol/)中的xml文件，提取xml文件的文件名，作为任务的类型名 @n
 *          在点选某种任务后，解析xml文件，组织界面程序显示，最终呈现给用户。
 */
void TaskControlPrivate::initTableViewMenu()
{
    QMenu *m_AddMenu = new QMenu(q_ptr);

    ProgramFilePath programPath;
    QDir dir(programPath.protocolPath);
    QStringList filters;
    filters << "*.xml";
    QStringList files = dir.entryList(filters);
    std::for_each(files.begin(),files.end(),[&](QString & fileName){
        QAction * action = new QAction();
        action->setProperty(ACTION_TYPE,fileName);
        action->setText(fileName.left(fileName.indexOf(".")));
        QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(showTaskWindow()));
        m_AddMenu->addAction(action);
    });

    addTaskAction = new QAction();
    addTaskAction->setMenu(m_AddMenu);
    taskView->addAction(addTaskAction);

    delTaskAction = new QAction();
    delTaskAction->setShortcut(QKeySequence::Delete);
    QObject::connect(delTaskAction, SIGNAL(triggered(bool)), q_ptr, SLOT(deleteTask()));
    taskView->addAction(delTaskAction);

    modifyAction = new QAction();
    QObject::connect(modifyAction, SIGNAL(triggered(bool)), q_ptr, SLOT(modifyTask()));
    taskView->addAction(modifyAction);

    copyAction = new QAction();
    copyAction->setShortcut(QKeySequence::Copy);
    QObject::connect(copyAction, SIGNAL(triggered(bool)), q_ptr, SLOT(copyTask()));
    taskView->addAction(copyAction);

    pasteAction = new QAction();
    pasteAction->setShortcut(QKeySequence::Paste);
    QObject::connect(pasteAction, SIGNAL(triggered(bool)), q_ptr, SLOT(pasteTask()));
    taskView->addAction(pasteAction);
}

TaskControl::TaskControl(QWidget *parent) :
    QWidget(parent),d_ptr(new TaskControlPrivate(this))
{
    IsDistributing = false;
    retranslateUi();
    RSingleton<Base::Subject>::instance()->attach(this);
}

TaskControl::~TaskControl()
{

}

void TaskControl::retranslateUi()
{

    Q_D(TaskControl);
    d->addTaskAction->setText(QObject::tr("Add task"));
    d->delTaskAction->setText(QObject::tr("Delete task"));
    d->modifyAction->setText(QObject::tr("Modify task"));
    d->copyAction->setText(QObject::tr("Copy task"));
    d->pasteAction->setText(QObject::tr("Paste task"));

    if(!IsDistributing){
        d->distrbuteButt->setText(QObject::tr("Issued task"));
    }else{
        d->distrbuteButt->setText(QObject::tr("Suspend issued"));
    }

    d->importButt->setText(QObject::tr("Import tasks..."));
    d->exportButt->setText(QObject::tr("Export tasks..."));

    d->resetButt->setText(QObject::tr("Reset issued"));
}

void TaskControl::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

/*!
 * @brief 定时下发任务
 * @details 只下发用户勾选的任务信息
 */
void TaskControl::distributeTask()
{
    Q_D(TaskControl);
    NewTaskList selectedList;
    std::for_each(d->taskInfoList.begin(),d->taskInfoList.end(),[&](NewTaskInfo * taskInfo){
        if(taskInfo->userChecked)
            selectedList.push_back(taskInfo);
    });

    if(d->taskInfoList.size() == 0){
        QMessageBox::warning(this,tr("warning"),tr("Selected task list is empty!"),QMessageBox::Yes);
        return;
    }

    initDispatch();

    d->resetButt->setEnabled(IsDistributing);
    IsDistributing = !IsDistributing;
    retranslateUi();

    if(IsDistributing){
        d->taskDispatcher->bindTaskList(d->taskInfoList);
        d->taskDispatcher->startDispatch();
    }else{
        d->taskDispatcher->pauseDispatch();
    }
}

void TaskControl::resetTask()
{
    Q_D(TaskControl);
    initDispatch();
    d->taskDispatcher->resetDispatch();
}

void TaskControl::initDispatch()
{
    Q_D(TaskControl);
    if(d->taskDispatcher == NULL){
        d->taskDispatcher = new TaskDispatcher;
        connect(d->taskDispatcher,SIGNAL(taskDispatch()),this,SLOT(updateTaskState()));
        connect(d->taskDispatcher,SIGNAL(dispatchFinished()),this,SLOT(dispatchOver()));
    }
}

void TaskControl::updateTaskState()
{
    Q_D(TaskControl);
    d->taskViewModel->updateTaskList(d->taskInfoList);
}

void TaskControl::dispatchOver()
{
    Q_D(TaskControl);
    IsDistributing = false;
    d->resetButt->setEnabled(true);
    retranslateUi();
}

/*!
 * @brief 显示创建任务窗口
 * @details 根据触发此事件的对象类型，解析对应的xml文件，创建不同的输入提示框，可供用户填写。
 * @param[in]  无
 * @return 无
 */
void TaskControl::showTaskWindow()
{
    Q_D(TaskControl);
    ProgramFilePath programPath;
    QString filename =  programPath.protocolPath + "/" +QObject::sender()->property(ACTION_TYPE).toString();
    QFileInfo fileinfo(filename);
    if(!fileinfo.exists()){
        QMessageBox::warning(this,tr("warning"),tr("layout file doesn't existed!"),QMessageBox::Yes);
        return;
    }

    TaskDialogProxy dialog(this);
    if(dialog.parseLayout(filename)){
        dialog.showMe();

        NewTaskInfo * info = dialog.getTaskInfo();
        if(info)
            d->taskInfoList.append(info);
    }

    d->taskViewModel->updateTaskList(d->taskInfoList);
}

/*!
 * @brief 表格右键菜单触发的坐标
 * @param[in] point 右键事件触发位置
 */
void TaskControl::tableContextPoint(QPoint point)
{
    Q_D(TaskControl);
    d->contextPoint = point;
}

/*!
 * @brief 获取右键菜单下的单元行
 * @param[in/out] valideIndex 当前行是否有效
 * @return 当前单元格索引
 */
QModelIndex TaskControl::currentIndex(bool & validIndex)
{
    Q_D(TaskControl);
    QModelIndex index = d->taskView->indexAt(d->contextPoint);
    if(index.isValid() && index.row() >= 0 && index.row() < d->taskInfoList.size())
        validIndex = true;
    else
        validIndex = false;
    return index;
}

void TaskControl::showEditWindow(QModelIndex index)
{
    Q_D(TaskControl);

    Q_ASSERT_X(d->taskInfoList.size() > index.row(),"TaskControl","Error selected row!");

    TaskDialogProxy dialog(this);
    NewTaskInfo * taskInfo = d->taskInfoList.at(index.row());
    if(dialog.parseLayout(taskInfo->localParsedFileName)){
        dialog.setTaskInfo(taskInfo);
        dialog.showMe();

        NewTaskInfo * info = dialog.getTaskInfo();
        if(info)
            d->taskInfoList.replace(index.row(),info);
    }

    d->taskViewModel->updateTaskList(d->taskInfoList);
}

/*!
 * @brief 查看任务详细信息
 * @details 直接调用修改的模块，即可查看，也可修改
 * @param[in] index 待查看行单元索引
 */
void TaskControl::viewTask(QModelIndex index)
{
    showEditWindow(index);
}

/*!
 * @brief 鼠标悬停在item上显示对应任务信息
 * @warning 启用此功能时，需要开启鼠标跟踪功能; @n
 * @details 为了避免鼠标的快速切换，导致显示窗口的快速创建或者销毁，此处采用定时器延迟的策略； @n
 *          1.检测是否有定时器开启，但未到触发时间，若存在则终止定时器； @n
 *          2.鼠标离开后，经过一定时间再关闭窗口。 @n
 *          3.再次进入则提示 @n
 * @param[in] index 鼠标悬停的item索引
 */
//TODO 20181010待添加对悬浮信息的显示
void TaskControl::mouseHoverItem(QModelIndex index)
{
    Q_D(TaskControl);
    if(index.row() > 0 && index.row() < d->taskInfoList.size()){
//        Type type = d->taskInfoList.at(index.row())->taskType;
//        switch(type){
//            case Band:
//                break;
//            case State:
//                break;
//            case Gather:
//                break;
//            case SelfCheck:
//                break;
//            case Instrument:
//                break;
//            case Turntable:
//                break;
//            case PlayBack:
//                break;
//            default:
//                break;
//        }
    }
}

/*!
 * @brief 删除指定任务
 */
void TaskControl::deleteTask()
{
    Q_D(TaskControl);

    bool flag = false;
    QModelIndex index = currentIndex(flag);
    if(flag){
        QMessageBox::StandardButton butt = QMessageBox::information(this,tr("information"),tr("Delete selected task?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
        if(butt = QMessageBox::Yes)
        {
            NewTaskInfo * deletedTask = d->taskInfoList.at(index.row());
            d->taskInfoList.removeAt(index.row());
            d->taskViewModel->updateTaskList(d->taskInfoList);
            delete deletedTask;
        }
    }
}

/*!
 * @brief 修改任务信息
 * @param[in]
 * @return
 */
void TaskControl::modifyTask()
{
    bool flag = false;
    QModelIndex index = currentIndex(flag);
    if(flag){
        showEditWindow(index);
    }
}

/*!
 * @brief 复制任务信息
 * @details 复制时根据当前的任务类型，创建对应的任务信息，并将复制行的内容拷贝至暂存区；
 *          每次复制时删除上一次复制的缓冲区内容，防止出现内存泄漏问题；
 */
void TaskControl::copyTask()
{
    Q_D(TaskControl);
    bool existed = false;
    QModelIndex index = currentIndex(existed);
    if(existed){
        if(d->cacheTaskInfo)
            delete d->cacheTaskInfo;

        NewTaskInfo * selectedTask = d->taskInfoList.at(index.row());
        d->cacheTaskInfo = new NewTaskInfo;
        *(d->cacheTaskInfo) = *selectedTask;
    }
}

/*!
 * @brief 粘贴任务信息
 * @details 1.若不是最后一行，将复制的任务信息粘贴到当前行 \p
 *          2.若是最后一行，则粘贴在最后一行 \p
 */
void TaskControl::pasteTask()
{
    Q_D(TaskControl);
    if(d->cacheTaskInfo == NULL){
        QMessageBox::warning(this,tr("warning"),tr("No tasks to paste!"),QMessageBox::Yes);
        return;
    }
    bool existed = false;
    QModelIndex index = currentIndex(existed);
    d->cacheTaskInfo->excuteTime = QDateTime::currentDateTime();
    if(existed){
        d->taskInfoList.insert(index.row(),d->cacheTaskInfo);
    }else{
        d->taskInfoList.append(d->cacheTaskInfo);
    }
    d->cacheTaskInfo = NULL;
    d->taskViewModel->updateTaskList(d->taskInfoList);
}

/*!
 * @brief 从本地导入保存的任务数据信息
 * @warning 导入解析时需要判断文件头是否正确
 */
void TaskControl::importTask()
{
    Q_D(TaskControl);
    QString localFileName = QFileDialog::getOpenFileName(this,tr("choose file"),QDir::homePath(),QString("*.bin"));
    if(!localFileName.isEmpty()){
        RTextFile file(localFileName);
        TaskParsedMethod * method = new TaskParsedMethod;
        file.setParseMethod(method);
        if(file.startParse()){
            d->taskInfoList.append(method->getTaskInfo());
            d->taskViewModel->updateTaskList(d->taskInfoList);
            QMessageBox::information(this,tr("information"),tr("import successfully!"));
        }else{
            QMessageBox::warning(this,tr("warning"),tr("import failed!"));
        }
    }
}

/*!
 * @brief 导出任务信息至本地
 * @warning 导出文件时采用特殊的文件头
 */
void TaskControl::exportTask()
{
    Q_D(TaskControl);
    QString saveName = QFileDialog::getSaveFileName(this,tr("save file"),QDir::homePath(),QString("*.bin"));
    if(!saveName.isEmpty()){
        RTextFile file(saveName);
        TaskParsedMethod * method = new TaskParsedMethod;
        method->setTaskInfo(d->taskInfoList);
        file.setParseMethod(method);
        if(file.startSave(QFile::WriteOnly | QFile::Truncate)){
            QMessageBox::information(this,tr("information"),tr("export successfully!"));
        }else{
            QMessageBox::warning(this,tr("warning"),tr("export failed!"));
        }
    }
}

} //namespace TaskControlModel
