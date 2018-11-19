#include "datastruct.h"
#include <QDebug>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QTimeEdit>


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

//TODO 20181109保存时只需要保存字段的name、index以及索引就好，基本信息从协议中获取
QDataStream & operator<< (QDataStream &stream, const Data_Bit &data)
{
    stream<<data.name<<data.index<<data.enable<<data.visible<<data.weight<<data.precision<<data.unit<<data.displayText;
    return stream;
}

QDataStream & operator>> (QDataStream &stream, Data_Bit &data)
{

    return stream;
}

QDataStream & operator<< (QDataStream &stream, const BitField &field)
{
    stream<<field.bitList.size();
    std::for_each(field.bitList.begin(),field.bitList.end(),[&](const Data_Bit data){
        stream<<data;
    });
    return stream;
}

QDataStream & operator>> (QDataStream &stream, BitField &field)
{
    int size = 0;
    stream >> size;
    for(int i = 0;i < size;i++){
        Data_Bit data;
        stream>>data;
        field.bitList.append(data);
    }
    return stream;
}

QDataStream & operator<<(QDataStream & stream,const Data_Word & data)
{
    stream<<data.name<<data.index<<data.bytes<<data.isSigned<<data.bits
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

QDataStream & operator>>(QDataStream & stream,Data_Word & data)
{
    stream>>data.name>>data.index>>data.bytes>>data.isSigned>>data.bits
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



void operator<<(QWidget* w,const Data_Word &data)
{
    if(data.enable) {
        if(data.bytes != 0) {
            switch(data.type){
                case Empty:
                    break;
                case ComboBox:
                {
                    QComboBox* widget = dynamic_cast<QComboBox *>(w);

                    widget->setCurrentIndex(data.value.toInt());
                }
                    break;
                case Datastruct::CheckBox:
                {
                    QCheckBox * widget = dynamic_cast<QCheckBox*>(w);

                    widget->setChecked(data.value.toBool());
                }
                    break;
                case Datastruct::RadioButton:
                {
                    QRadioButton * widget = dynamic_cast<QRadioButton *>(w);

                    widget->setChecked(data.value.toBool());
                }
                    break;
                case Datastruct::TextEdit:
                {
                    QLineEdit * widget = dynamic_cast<QLineEdit *>(w);

                    widget->setText(data.value.toString());
                }
                    break;
                case Datastruct::ValueIntEdit:
                {
                    QSpinBox * widget = dynamic_cast<QSpinBox *>(w);

                    widget->setValue(data.value.toInt());
                }
                    break;
                case Datastruct::ValueFloatEdit:
                {
                    QDoubleSpinBox * widget = dynamic_cast<QDoubleSpinBox *>(w);

                    widget->setValue(data.value.toDouble());
                }
                    break;
                case Datastruct::DateEdit:
                {
                    QDateEdit * widget = dynamic_cast<QDateEdit *>(w);

                    widget->setDate(data.value.toDate());
                }
                    break;
                case Datastruct::TimeEdit:
                {
                    QTimeEdit * widget = dynamic_cast<QTimeEdit *>(w);

                    widget->setTime(data.value.toTime());
                }
                    break;
                default:
                    break;
                }
        }
        else{
            //todo bit的布局显示
        }
    }
}

void Field::update()
{
    if(widget)
        widget<<data;
}

short BaseProtocol::getBytes_Content()
{
    if(0 == bytes_Contents)  {
        for(int i=0;i < contents.count();i++)
            bytes_Contents += contents.at(i).getBytes_To(contents.at(i).count);
    }
    return bytes_Contents;
}

short SingleProtocol::getBytes_To(int pos,int base)const
{
    short allBytes = base;
    if(pos == fields.count())
        if(fields.at(pos -1).pos_InProtocol !=0)
            return fields.at(pos -1).pos_InProtocol + fields.at(pos -1).bytes;

    for(int i=0;i < pos;i++)
        allBytes +=fields.at(i).bytes;
    return allBytes;
}



}

