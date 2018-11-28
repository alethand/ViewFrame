#include "model_cover.h"
#include <QVariant>

namespace DataView{



Model_Cover::Model_Cover(DataManager *subject,Core::Data_Word *settings,bool openFilter)
        :DataView::Observer(subject,openFilter)
{
    mField_Appointed = settings;

}



QVariant Model_Cover::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal) {
        if(section < Observer::columnCount())
            return Observer::headerData(section,orientation,role);
        else {
            switch(section - Observer::columnCount()) {
            case 1:return tr("截获次数");
            case 2:return tr("截获时间");
            default:break;
            }
        }
    }
    return QVariant();
}

void Model_Cover::getNewestInfo(Core::NetParse::Protocol *info)
{
     //for(int i=0;i < mProtocol->)
     QMap<QVariant,CoverData>::iterator ite = mapInterceptInfo.begin();
     //偷个小懒 只加入第一个信息。
    // while()



}

int Model_Cover::rowCount(const QModelIndex &parent) const
{
    return mapInterceptInfo.count();
}

QVariant Model_Cover::getData(int row, int column) const
{
    QMap<QVariant,CoverData>::const_iterator ite = mapInterceptInfo.cbegin();
    int count =0;
    if(row >=0 && row <mapInterceptInfo.count()){
        ite +=row;
        if(column < Observer::columnCount()){

             Core::NetParse::Elem *temp = Core::NetParse::getData_InPos(ite->pProtocol,0,column,isFilter);
             if(temp!= NULL)
                 return temp->getValue(this->isFilter);

             return QVariant();
             }
        else {
            switch(column - Observer::columnCount()) {
            case 1:return ite->times;
            case 2:return ite->time.toString("hh/mm/ss");
            default:break;
            }
        }
    }
    return QVariant();
}

}
