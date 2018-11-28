#ifndef MODEL_HISTORY_H
#define MODEL_HISTORY_H

#include "dateview_superstruct.h"
#include <QList>
namespace DataView{


class Model_History:public DataView::Observer
{
public:
    explicit Model_History(DataManager *subject,bool openFilter = false);


    virtual void getNewestInfo(Core::NetParse::Protocol* info){ Q_UNUSED(info)}
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant getData(int row,int column) const ;
private:

};


}

#endif // MODEL_HISTORY_H
