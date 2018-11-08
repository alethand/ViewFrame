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
             specifedCol = -1;
         }

         int HugeData_Model::rowCount(const QModelIndex &parent) const
         {
             Q_UNUSED(parent)
             return 50;//mdata->getRowCount();
         }

         int HugeData_Model::columnCount(const QModelIndex &parent) const
         {
              Q_UNUSED(parent)
             if(specifedCol == -1)
                return mdata->getColCount();
             return 1;
         }

         QVariant HugeData_Model::data(const QModelIndex &index, int role) const
         {
             if (!index.isValid())
                 return QVariant();

             int row = index.row()+1;
             int col=index.column();
             switch(role)
             {
             case Qt::TextAlignmentRole:
                 return int(Qt::AlignHCenter | Qt::AlignVCenter);
                 break;
             case Qt::DisplayRole:
                 if(rowsLatestMsg == -1) //未设置仅显示最新消息
                 {
                    if(row < mdata->getRowCount() && col < mdata->getColCount())
                    {
                        if(specifedCol == -1 ||specifedCol == col)
                              return mdata->getData(row,col);
                    }
                 }
                 else
                     if(row < rowsLatestMsg &&
                             row < mdata->getRowCount() &&col < mdata->getColCount() )
                     {
                         if(specifedCol == -1 ||specifedCol == col)
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
                 if(specifedCol == -1) {
                     if (Qt::Horizontal == orientation && section < mdata->coreData.first()->getCount()){
                         return mdata->getHeadName(section);
                     }
                 }
                 else
                      return mdata->getHeadName(section);
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
    return QVariant();
}

QVariant Datastruct::AllPluseInfo::Statistic::Core::getData(int index)
{
    switch(index)
    {
    case 0:return maxVal;
    case 1:return minVal;
    case 2:return meanVal;
    case 3:return variance;
    default:return QVariant();
    }
}

QVariant Datastruct::MidFreqInfo::Core::getData(int index)
{
    switch((MidFreqInfo::MFAcquisitionHead)index) {
    case T_AcqTime:  return acqTime;
    case T_AcqModel: return acqMode;
    case T_AcqNum:   return listInfo.count();
    case T_AcqDotNum:
        {   int sum = 0;
            for(int i=0; i < listInfo.count(); i++)
                sum += listInfo.at(i).adData.count();
            return sum;
        }
    default:return QVariant();
    }
}


namespace Datastruct{

QDataStream & operator<<(QDataStream & stream,const FieldData & data)
{
    stream<<data.name<<data.index<<data.bytes<<data.isSigned<<data.bits<<data.offset
         <<data.weight<<data.precision<<data.unit<<data.displayText;

    stream<<static_cast<int>(data.type);
    switch(data.type){
        case ComboBox :
                stream<<data.value.toInt();
                break;
        case CheckBox :
        case RadioButton :
                stream<<data.value.toBool();
            break;
        case TextEdit :
                stream<<data.value.toString();
            break;
        case ValueIntEdit :
                stream<<data.value.toInt();
            break;
        case ValueFloatEdit :
                stream<<data.value.toDouble();
            break;
        case DateEdit :
                stream<<data.value.toDate();
            break;
        case TimeEdit :
                stream<<data.value.toTime();
            break;
        case Empty :
        default:
            break;
    }

    return stream;
}

QDataStream & operator>>(QDataStream & stream,FieldData & data)
{
    stream>>data.name>>data.index>>data.bytes>>data.isSigned>>data.bits>>data.offset
         >>data.weight>>data.precision>>data.unit>>data.displayText;

    int type = 0;
    stream >> type;
    data.type = static_cast<ControlType>(type);

    switch(data.type){
        case ComboBox :
                {
                    int tmp = 0;
                    stream>>tmp;
                    data.value = QVariant::fromValue(tmp);
                }
                break;
        case CheckBox :
        case RadioButton :
                {
                    bool tmp = false;
                    stream>>tmp;
                    data.value = QVariant::fromValue(tmp);
                }
                break;
        case TextEdit :
                {
                    QString tmp;
                    stream>>tmp;
                    data.value = QVariant::fromValue(tmp);
                }
            break;
        case ValueIntEdit :
                {
                    int tmp = 0;
                    stream>>tmp;
                    data.value = QVariant::fromValue(tmp);
                }
                break;
        case ValueFloatEdit :
                {
                    double tmp = 0;
                    stream>>tmp;
                    data.value = QVariant::fromValue(tmp);
                }
                break;
        case DateEdit :
                {
                    QDate tmp;
                    stream>>tmp;
                    data.value = QVariant::fromValue(tmp);
                }
                break;
        case TimeEdit :
                {
                    QTime tmp;
                    stream>>tmp;
                    data.value = QVariant::fromValue(tmp);
                }
                break;
        case Empty :
        default:
            break;
    }
    return stream;
}

}

