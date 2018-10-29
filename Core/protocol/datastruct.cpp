#include "datastruct.h"
#include <QDebug>

namespace Protocol
{
    namespace Data
    {
         HugeData_Model::HugeData_Model(HugeData_Gram *data)
            :QAbstractTableModel()
         {
             mdata = data;
             rowsLatestMsg = -1;
         }

         int HugeData_Model::rowCount(const QModelIndex &parent) const
         {
             return 50;// mdata->getRowCount();
         }

         int HugeData_Model::columnCount(const QModelIndex &parent) const
         {
             return mdata->getColCount();
         }

         QVariant HugeData_Model::data(const QModelIndex &index, int role) const
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
                 if(rowsLatestMsg == -1) //未设置仅显示最新消息
                 {
                    if(row+1 < mdata->getRowCount() && col < mdata->getColCount())
                    {
                        if(specifedCol.find(col) != specifedCol.end())
                              return mdata->getData(row,col);
                    }
                 }
                 else
                     if(row < rowsLatestMsg &&
                             row+1 < mdata->getRowCount() &&col < mdata->getColCount() )
                     {
                         if(specifedCol.find(col) != specifedCol.end())
                            return  mdata->getData(mdata->getRowCount() - row,col);
                     }

             case Qt::BackgroundColorRole:
                    break;
             }
             return QVariant();
         }

         QVariant HugeData_Model::headerData(int section, Qt::Orientation orientation, int role) const
         {
             if (role != Qt::DisplayRole)
                 return QVariant();

             if(!mdata->coreData.isEmpty())
             {
                 if (Qt::Horizontal == orientation && section < mdata->coreData.first()->getCount()){
                     return mdata->getHeadName(section);
                 }
             }
             else
                 qDebug()<<"Error-HugeData_Model::when you pass by 'HugeData_Gram' into the class,/n"
                           "the 'HugeData_Gram's coreData isEmpty!";

             return QVariant();
         }




         QString HugeData_Gram::getHeadName(int index) const {
             if(!coreData.isEmpty())
                 if(index < coreData.first()->getCount())
                    return coreData.first()->getName(index);
             return QString();
         }

         int HugeData_Gram::getRowCount()const {
             if(!isReverse)
                 return coreData.count();
             else
                 if(!coreData.isEmpty())
                    return coreData.first()->getCount();
                  else
                     return 0;
         }

         int HugeData_Gram::getColCount() const
         {
             if(!isReverse)
             {
                 if(!coreData.isEmpty())
                     return coreData.first()->getCount();
                 else
                     return 0;
             }
             else
                 return coreData.count();
         }

         QVariant HugeData_Gram::getData(int row, int col)const
         {
             if(!coreData.isEmpty())
             {
                 if(!isReverse)
                 {
                     if(row < coreData.count()&&col < coreData.at(row)->getCount())
                        return coreData.at(row)->getData(col);
                 }
                 else
                     if(col < coreData.count()&&row < coreData.at(col)->getCount())
                         return coreData.at(col)->getData(row);
             }
             return QVariant();
         }




    }
}


QVariant Datastruct::AllPluseInfo::OriginData::Core::getData(int index)
{
    if(index < valuelist.count())
    return valuelist.at(index);
}

QVariant Datastruct::AllPluseInfo::Statistic::Core::getData(int index)
{
    switch(index)
    {
    case 0:return maxVal;
    case 1:return minVal;
    case 2:return meanVal;
    case 3:return variance;
    }
}
