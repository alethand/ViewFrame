#include "model_details.h"

namespace DataView{

Model_Details::Model_Details(DataManager *subject)
    :DataView::Observer(subject,true)
{

}

void Model_Details::getNewestInfo(QSharedPointer<NetParse::Protocol> *info)
{

}

int Model_Details::rowCount(const QModelIndex &parent) const
{
    return 0;
}

QVariant Model_Details::getData(int row, int column) const
{
    return QVariant();
}



}
