﻿#include "taskcontrol.h"

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
    QWidget * m_centralWidget;

    QWidget * mainWidget;

    TaskInfoList taskInfoList;          /*!< 任务集合 */
    QPoint contextPoint;                /*!< 表格右键事件菜单 */
    TaskInfo * cacheTaskInfo;           /*!< 复制的任务信息 */
    TaskDispatcher * taskDispatcher;    /*!< 任务下发 */

    QAction *bandAction;
    QAction *stateAction;
    QAction *gatherAction;
    QAction *selfCheckAction;
    QAction *instrumentAction;
    QAction *trunTableAction;
    QAction *playBackAction;
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

    distrbuteButt = new QPushButton(mainWidget);
    QObject::connect(distrbuteButt, SIGNAL(pressed()), q_ptr, SLOT(distributeTask()));
    distrbuteButt->setMinimumWidth(120);
    distrbuteButt->setFixedHeight(26);

    resetButt = new QPushButton(mainWidget);
    QObject::connect(resetButt, SIGNAL(pressed()), q_ptr, SLOT(resetTask()));
    resetButt->setMinimumWidth(120);
    resetButt->setFixedHeight(26);

    QWidget * toolWidget = new QWidget;
    QHBoxLayout * hlayout = new QHBoxLayout;
    hlayout->setContentsMargins(2,2,2,2);
    hlayout->addStretch(1);
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
    taskView->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    taskViewModel = new TableViewModel;
    taskViewDelegate = new TableViewDelegate(taskView);

    taskView->setModel(taskViewModel);
    taskView->setItemDelegate(taskViewDelegate);
    QObject::connect(taskView,SIGNAL(contextMenuPos(QPoint)),q_ptr,SLOT(tableContextPoint(QPoint)));
    QObject::connect(taskView, SIGNAL(tableCheckDoubleSignal(QModelIndex)), q_ptr, SLOT(viewTask(QModelIndex)));
}

void TaskControlPrivate::initTableViewMenu()
{
    QMenu *m_AddMenu = new QMenu(q_ptr);
    bandAction = new QAction();
    bandAction->setProperty(ACTION_TYPE,Type::Band);
    QObject::connect(bandAction, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(bandAction);

    stateAction = new QAction();
    stateAction->setProperty(ACTION_TYPE,Type::State);
    QObject::connect(stateAction, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(stateAction);

    gatherAction = new QAction();
    gatherAction->setProperty(ACTION_TYPE,Type::Gather);
    QObject::connect(gatherAction, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(gatherAction);

    selfCheckAction = new QAction();
    selfCheckAction->setProperty(ACTION_TYPE,Type::SelfCheck);
    QObject::connect(selfCheckAction, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(selfCheckAction);

    instrumentAction = new QAction();
    instrumentAction->setProperty(ACTION_TYPE,Type::Instrument);
    QObject::connect(instrumentAction, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(instrumentAction);

    trunTableAction = new QAction();
    trunTableAction->setProperty(ACTION_TYPE,Type::Turntable);
    QObject::connect(trunTableAction, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(trunTableAction);

    playBackAction = new QAction();
    playBackAction->setProperty(ACTION_TYPE,Type::PlayBack);
    QObject::connect(playBackAction, SIGNAL(triggered(bool)), q_ptr, SLOT(addNewTask()));
    m_AddMenu->addAction(playBackAction);

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
    QObject::connect(copyAction, SIGNAL(triggered(bool)), q_ptr, SLOT(copyTable()));
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
    d->bandAction->setText(QObject::tr("Band Control"));
    d->stateAction->setText(QObject::tr("State Control"));
    d->gatherAction->setText(QObject::tr("Gather Control"));
    d->selfCheckAction->setText(QObject::tr("SelfCheck Control"));
    d->instrumentAction->setText(QObject::tr("Instrument Control"));
    d->trunTableAction->setText(QObject::tr("Turntable Control"));
    d->playBackAction->setText(QObject::tr("PlayBack Control"));
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
 */
void TaskControl::distributeTask()
{
    Q_D(TaskControl);
    if(d->taskInfoList.size() == 0){
        QMessageBox::warning(this,tr("warning"),tr("Task list is empty!"),QMessageBox::Yes);
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
    switch(d->taskInfoList.at(index.row())->taskType){
        case Type::Band:
            {
                BandControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<BandControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
                dialog.getWindowData();
            }
            break;
        case Type::State:
            {
                StateControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<StateControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
                dialog.getWindowData();
            }
            break;
        case Type::Gather:
            {
                GatherControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<GatherControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
                dialog.getWindowData();
            }
            break;
        case Type::SelfCheck:
            {
                SelfCheckControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<SelfCheckControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
                dialog.getWindowData();
            }
            break;
        case Type::Instrument:
            {
                InstrumentControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<InstrumentControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
                dialog.getWindowData();
            }
            break;
        case Type::Turntable:
            {
                TurntableControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<TurntableControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
                dialog.getWindowData();
            }
            break;
        case Type::PlayBack:
            {
                PlayBackControlDialog dialog(this);
                dialog.setWindowData(dynamic_cast<PlayBackControl *>(d->taskInfoList.at(index.row())));
                dialog.exec();
                dialog.getWindowData();
            }
            break;
        default:
            break;
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
 * @brief 删除指定任务
 * @return
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
            TaskInfo * deletedTask = d->taskInfoList.at(index.row());
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
 * @brief 添加新的任务信息
 * @details 根据触发此事件的对象类型，创建不同的输入提示框，可供用户填写。
 * @param[in]  无
 * @return 无
 */
void TaskControl::addNewTask()
{
    Q_D(TaskControl);
    Type tt =  static_cast<Type>(QObject::sender()->property(ACTION_TYPE).toInt());
    switch(tt)
    {
        case Type::Band:
            {
                BandControlDialog dialog(this);
                dialog.exec();
                BandControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::State:
            {
                StateControlDialog dialog(this);
                dialog.exec();
                StateControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::Gather:
            {
                GatherControlDialog dialog(this);
                dialog.exec();
                GatherControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::SelfCheck:
            {
                SelfCheckControlDialog dialog(this);
                dialog.exec();
                SelfCheckControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::Instrument:
            {
                InstrumentControlDialog dialog(this);
                dialog.exec();
                InstrumentControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::Turntable:
            {
                TurntableControlDialog dialog(this);
                dialog.exec();
                TurntableControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        case Type::PlayBack:
            {
                PlayBackControlDialog dialog(this);
                dialog.exec();
                PlayBackControl * info = dialog.getWindowData();
                if(info){
                    d->taskInfoList.append(info);
                }
            }
            break;
        default:
            break;
    }

    d->taskViewModel->updateTaskList(d->taskInfoList);
}

/*!
 * @brief 复制任务信息
 * @details 复制时根据当前的任务类型，创建对应的任务信息，并将复制行的内容拷贝至暂存区；
 *          每次复制时删除上一次复制的缓冲区内容，防止出现内存泄漏问题；
 */
void TaskControl::copyTable()
{
    Q_D(TaskControl);
    bool existed = false;
    QModelIndex index = currentIndex(existed);
    if(existed){
        if(d->cacheTaskInfo)
            delete d->cacheTaskInfo;

        bool copyResult = false;
        TaskInfo * selectedTask = d->taskInfoList.at(index.row());
        switch(selectedTask->taskType)
        {
            case Band:
                       copyResult = executeCopy<BandControl>(selectedTask);
                 break;
            case State:
                       copyResult = executeCopy<StateControl>(selectedTask);
                 break;
            case Gather:
                       copyResult = executeCopy<GatherControl>(selectedTask);
                 break;
            case SelfCheck:
                       copyResult = executeCopy<SelfCheckControl>(selectedTask);
                 break;
            case Instrument:
                       copyResult = executeCopy<InstrumentControl>(selectedTask);
                 break;
            case Turntable:
                       copyResult = executeCopy<TurntableControl>(selectedTask);
                 break;
            case PlayBack:
                       copyResult = executeCopy<PlayBackControl>(selectedTask);
                 break;
            default:
                break;
        }

        if(!copyResult){
            d->cacheTaskInfo = NULL;
            QMessageBox::warning(this,tr("warning"),tr("Copy task failed!"),QMessageBox::Yes);
        }
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
    if(existed){
        d->taskInfoList.insert(index.row(),d->cacheTaskInfo);
    }else{
        d->taskInfoList.append(d->cacheTaskInfo);
    }
    d->cacheTaskInfo = NULL;
    d->taskViewModel->updateTaskList(d->taskInfoList);
}

template<class T>
bool TaskControl::executeCopy(TaskInfo *selectedTask)
{
    Q_D(TaskControl);
    T * copyTask = dynamic_cast<T *>(selectedTask);
    if(copyTask){
        T * control = new T(*copyTask);
        d->cacheTaskInfo = control;
        return true;
    }
    return false;
}

} //namespace TaskControlModel
