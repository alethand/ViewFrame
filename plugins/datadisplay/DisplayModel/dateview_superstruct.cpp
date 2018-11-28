#include "dateview_superstruct.h"

namespace DataView {

DataManager::DataManager(const Core::Datastruct::BaseProtocol *protocol)
{
    mProtocol = protocol;
//    positionFast();
}

void DataManager::handleNetInfo(Core::NetParse::Protocol *netInfo)
{
    originData.append(netInfo);

    QList<Observer*>::iterator ite = observers.begin();
    while(ite != observers.end())
    {
        (*ite)->getNewestInfo(netInfo);
        ite++;
    }
}

/*!
 * \brief RadiusSrc_DataManager::register_DataObserver 注册成为观察者
 * \param obj
 * \warning 注意：本观察者模式不提供退出功能
 */
void DataManager::register_DataObserver(Observer *obj)
{
    observers.append(obj);
    obj->mProtocol = mProtocol;
}

void DataManager::positionFast()
{
    Core::Datastruct::BaseProtocol*pProt = const_cast<Core::Datastruct::BaseProtocol*>(mProtocol);
    pProt->getBytes_Content();
    QList<Core::SingleProtocol>::iterator ite =  pProt->contents.begin();

    int allLen = 0;
    while(ite != pProt->contents.end()){
        QList<Core::Data_Word>::iterator inner_Ite = ite->fields.begin();
        while(inner_Ite != ite->fields.end()){
            inner_Ite->pos_InProtocol = ite->getBytes_To(inner_Ite - ite->fields.begin(),allLen);

            inner_Ite++;
        }
        allLen = ite->getBytes_To(ite->count);
        ite++;
    }

}


Observer::Observer(DataManager *subject,bool openFilter)
    :manager(subject)
{
    subject->register_DataObserver(this);
    isFilter = openFilter;
//    ensureDisplayData();
}

QVariant Observer::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    switch(role)
    {
    case Qt::TextAlignmentRole:
        return int(Qt::AlignHCenter | Qt::AlignVCenter);
        break;
    case Qt::DisplayRole:
         return getData(index.row(),index.column());
    case Qt::BackgroundColorRole:
        break;
    }
    return QVariant();
}

/*!
 * \brief Observer::ensureDisplayData  确定需要进行显示的数据
 *    isFilter == false;  将所有信息字段名称不为空的字段加入 显示列表
 *             == true;   将所有信息字段名称不为空&&visible为1的字段显示在列表上
 */
void Observer::ensureDisplayData()
{
    QList<Core::SingleProtocol>::iterator ite_SingleProtocol = const_cast <Core::Datastruct::BaseProtocol *>(mProtocol)
                                                            ->contents.begin();
        while(ite_SingleProtocol != const_cast <Core::Datastruct::BaseProtocol *>(mProtocol)
                                     ->contents.end()){

            QList<Core::Data_Word>::iterator ite = ite_SingleProtocol->fields.begin();
            while(ite !=ite_SingleProtocol->fields.end()) {
                if(!ite->name.isEmpty()) {
                    if(false == isFilter)
                        mNeedsDisplayData.append(&(*ite));
                    else
                        if(true == ite->visible )
                            mNeedsDisplayData.append(&(*ite));
                }
                ite++;
            }

        ite_SingleProtocol++;
        }
}


/*!
 * \brief Observer::headerData
 * \warning 因为目前规定的contents中的字段都是统一的数据内容格式，则显示的数据行标题为其中visible的字段
 */
QVariant Observer::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal) {
        if(section < mNeedsDisplayData.count())
            return mNeedsDisplayData.at(section)->name;
    }
    return QVariant();
}

int Observer::columnCount(const QModelIndex &parent) const
{
    return mNeedsDisplayData.count();
}








}
