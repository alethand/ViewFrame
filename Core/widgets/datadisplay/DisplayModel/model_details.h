#ifndef MODEL_DETAILS_H
#define MODEL_DETAILS_H

#include "dateview_superstruct.h"

namespace DataView{

/*!
 * \brief The Model_Details class 详细数据显示--具体数据列表组织形式再规定
 */
class Model_Details:public DataView::Observer
{
public:
    Model_Details(DataManager *subject);

    virtual void getNewestInfo(NetParse::Protocol* info);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant getData(int row,int column) const ;

};

}

#endif // MODEL_DETAILS_H
