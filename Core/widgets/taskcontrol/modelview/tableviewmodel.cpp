#include "TableViewModel.h"

#include <QDebug>
#include "Base/util/rsingleton.h"

#include <iostream>
using namespace std;

namespace TaskControlModel {

TableViewModel::TableViewModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    retranslateUi();
    RSingleton<Base::Subject>::instance()->attach(this);
}

TableViewModel::~TableViewModel()
{

}

int TableViewModel::rowCount(const QModelIndex & index) const
{
    return taskList.count();
}

int TableViewModel::columnCount(const QModelIndex & index) const
{
    return headInfo.size();
}

bool TableViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid() || index.row() >= taskList.size())
        return false;

    int col = index.column();
    int row = index.row();
    
    TaskInfo * taskInfo = taskList.at(row);
    
    switch (role) {
        case Qt::CheckStateRole:
            taskInfo->userChecked = value.toBool();
        break;

        case Qt::EditRole:
            if (col == PARAMENTER) {
                taskInfo->parameter = value.toString();
            } else if (col == EXCUTIONTTIME) {
                taskInfo->excuteTime = value.toDateTime();
            } else if (col == EXCUTIONTTIMELONG) {
                taskInfo->lastTime = value.toInt();
            }
            break;
        default:
            return false;
    }
    resetData();
    return true;
}

QVariant TableViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= taskList.size())
        return QVariant();

    int row = index.row();

    if(static_cast<TaskHead>(index.column()) == T_No && role == Qt::CheckStateRole){
        return taskList.at(row)->userChecked ? Qt::Checked : Qt::Unchecked;
    }

    switch(role)
    {
        case Qt::TextAlignmentRole:
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        break;

        case Qt::DisplayRole:
            {
                switch (static_cast<TaskHead>(index.column())) {
                    case T_No:
                        return QString("%1").arg(row+1);
                    case T_TYPE:
                        {
                            switch(taskList.at(row)->taskType){
                                case Type::Band:
                                        return QObject::tr("Band control");
                                case Type::State:
                                        return QObject::tr("State control");
                                case Type::Gather:
                                        return QObject::tr("Gather control");
                                case Type::SelfCheck:
                                        return QObject::tr("Selfcheck control");
                                case Type::Instrument:
                                        return QObject::tr("Instrument Control");
                                case Type::Turntable:
                                        return QObject::tr("Turning table control");
                                case Type::PlayBack:
                                        return QObject::tr("Playback control");
                                default:
                                    return QVariant("");
                            }
                        }
                        break;
                    case T_PARAMETERS:
                        return taskList.at(row)->parameter;
                    case T_E_TIME:
                        return taskList.at(row)->excuteTime.toString("yyyy-MM-dd hh:mm:ss");
                    case T_E_TIME_LONG:
                        return taskList.at(row)->lastTime;
                    case T_DIS_STATE:
                        if(taskList.at(row)->dstate == Issued)
                            return QObject::tr("Issued");
                        else if(taskList.at(row)->dstate == Error_Issued)
                            return QObject::tr("Error Issued");
                        else
                            return QObject::tr("Not-Issued");
                    default:
                        return QVariant("");
                }
            }
            break;

        default:
            break;
    }

    return QVariant();
}

QVariant TableViewModel::headerData(int section,Qt::Orientation orientation,int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    
    if (Qt::Horizontal == orientation && section < headInfo.size()){
        return headInfo.at(section);
    }
        
    return QVariant();
}

Qt::ItemFlags TableViewModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    switch (static_cast<TaskHead>(index.column())) {
        case T_No:
                return Qt::ItemIsUserCheckable | flags;
            break;
        case T_PARAMETERS:
        case T_E_TIME:
        case T_E_TIME_LONG:
                return Qt::ItemIsEditable | flags;
            break;
        default:
            break;
    }
    return  flags;
}

void TableViewModel::updateTaskList(TaskInfoList &list)
{
    taskList = list;
    resetData();
}

void TableViewModel::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
                retranslateUi();
                resetData();
        break;
        default:
            break;
    }
}

void TableViewModel::resetData()
{
    beginResetModel();
    endResetModel();
}

void TableViewModel::retranslateUi()
{
//    cout<<"the2"<<endl;
    headInfo.clear();
    headInfo<<QObject::tr("Index")<<QObject::tr("Type")<<QObject::tr("Parameter")
                  <<QObject::tr("Dispatch Time")<<QObject::tr("Execute Time")<<QObject::tr("Issued status");
}

} //namespace TaskControlModel
