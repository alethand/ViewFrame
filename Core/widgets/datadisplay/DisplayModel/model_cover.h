/*覆盖刷新：当有新的消息到来的时候，如果与原先的信息具有相同的字段，
 * 则顶替掉原有的信息，进行显示
 *
 * 使用条件：一般为协议中嵌套使用协议的情况。覆盖刷新指定字段一般出现在一层/二层协议中的某个字段
 */
#ifndef MODEL_COVER_H
#define MODEL_COVER_H

#include "dateview_superstruct.h"
#include "Core/protocol/datastruct.h"
#include <QList>
#include <QMap>
#include <QTime>
namespace DataView{
const int EVERY = -2;
const int NOSET = -1;

/*!
 * \brief The Model_Cover class 覆盖模型
 * \对指定的字段进行覆盖刷新
 */
using namespace DataView;
class Model_Cover:public DataView::Observer
{
public:

    Model_Cover(DataManager *subject,Data_Word *settings,bool openFilter = false);


    virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    virtual void getNewestInfo(QSharedPointer<NetParse::Protocol>* info);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant getData(int row,int column) const ;


private:

    struct CoverData{
        CoverData();
        NetParse::Protocol *pProtocol;    /*! 截获协议*/
        QTime         time;         /*! 截获时间*/
        unsigned int  times;        /*! 截获次数*/
    };

    Data_Word *mField_Appointed;                    //指定字段

    QMap<QVariant,CoverData>  mapInterceptInfo;      /*! 截获信息映射*/
};


}

#endif // MODEL_COVER_H
