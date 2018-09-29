#include "tableviewmodelcustom.h"
#include <QDebug>
#include "Base/util/rsingleton.h"

namespace DataView {
const int INIT_TABLE_CUSTOM_ROW=40;
const int INIT_TABLE_ALLPULSE_ROW=18;  //全脉冲初始表格
TableViewModelCustom::TableViewModelCustom(QObject *parent)
    :QAbstractTableModel(parent),
    tableCustomKind(STATISTICAL_INFO)
{
    tableTotalRows=INIT_TABLE_CUSTOM_ROW;
    retranslateUi();
    RSingleton<Base::Subject>::instance()->attach(this);
}

TableViewModelCustom::~TableViewModelCustom()
{

}


int TableViewModelCustom::rowCount(const QModelIndex &index) const
{
    return tableTotalRows;
}

int TableViewModelCustom::columnCount(const QModelIndex &index) const
{
    return headInfo.size();
}

bool TableViewModelCustom::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    return true;
}

QVariant TableViewModelCustom::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col=index.column();
    switch(role)
    {
        case Qt::TextAlignmentRole:
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        break;
        case Qt::DisplayRole:
            {
                if(tableCustomKind==MF_ACQUISITION_INFO)    //中频数据
                {

                }
                else if(tableCustomKind==STATISTICAL_INFO)  //统计信息
                {

                }
                else if(tableCustomKind==ORIGINAL_INFO)     //原始数据
                {

                }
            }
        break;
        case Qt::BackgroundColorRole:
        {

        }
        break;
        default:
            break;
    }

    return QVariant();
}

QVariant TableViewModelCustom::headerData(int section, Qt::Orientation orientation,int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (Qt::Horizontal == orientation && section < headInfo.size()){
        return headInfo.at(section);
    }

    return QVariant();
}

Qt::ItemFlags TableViewModelCustom::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void TableViewModelCustom::onMessage(MessageType::MessType type)
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

void TableViewModelCustom::resetData()
{
    beginResetModel();
    endResetModel();
}

void TableViewModelCustom::retranslateUi()
{
    QStringList headInfoTemp=headInfo;
    headInfo.clear();
    headInfo=headInfoTemp;
}

/*!
 * \brief  更新表头信息
 * \param head
 */
void TableViewModelCustom::resetHeadInfo(const QStringList& head)
{
    headInfo=head;
    resetData();
    retranslateUi();
}

/*!
 * \brief 设置表格显示类型,一个model要显示多张不同表
 * \param kind
 */
void TableViewModelCustom::setTableCustomKind(TableCustomKind kind)
{
    tableCustomKind=kind;

    if(tableCustomKind==MF_ACQUISITION_INFO)    //中频数据
    {

    }
    else if(tableCustomKind==STATISTICAL_INFO)  //统计信息
    {
        tableTotalRows=INIT_TABLE_CUSTOM_ROW;
    }
    else if(tableCustomKind==ORIGINAL_INFO)     //原始数据
    {
        tableTotalRows=INIT_TABLE_CUSTOM_ROW;
    }
}

} //namespace DataView
