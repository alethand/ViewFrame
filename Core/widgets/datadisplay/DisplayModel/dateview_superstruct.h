#ifndef DATEVIEW_SUPERSTRUCT_H
#define DATEVIEW_SUPERSTRUCT_H
#include "Core/protocol/datastruct.h"
#include "Core/network/netparseinfo.h"
#include <QList>
#include <QAbstractTableModel>


namespace DataView {


/*!
 * \brief The DataManager class 数据管理器
 * \note  功能：处理网络数据，并通知相应的显示模块更新数据
 * \note 模式使用：
 */
class Observer;
class DataManager
{
public:
    explicit DataManager(const Datastruct::BaseProtocol *protocol);

    /*! @warning 此时默认Datastruct::ParsedResult 为一条网络数据报文 */
    void handleNetInfo(NetParse::Protocol *netInfo);            ///处理网络数据
    void register_DataObserver(Observer *obj);                                  ///注册成为观察者

    const QList<NetParse::Protocol*>*                           ///获取所有历史数据
            getAllHistoryData(){return &originData;}

private:
    void positionFast();                                                        ///快速定位

private:
    QList<NetParse::Protocol*> originData;    /*! 原始数据-随时间递增*/
    QList<Observer*> observers;                               /*! 观察者集合 */
    const Datastruct::BaseProtocol *mProtocol;                /*! 解析的协议   */
};


/*!
 * \brief The DataDisplay_Observer class 数据显示观察者
 */
class  Observer:public QAbstractTableModel{

public:
    explicit Observer(DataManager *subject,bool openFilter = false);

    virtual void getNewestInfo(NetParse::Protocol* info)=0;    ///获取最新信息
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const=0;
    virtual QVariant getData(int row,int column) const =0;

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QVariant headerData(int section, Qt::Orientation orientation,int role) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
private:
    void ensureDisplayData();

protected:
    DataManager *manager;                               /*! 数据管理者 */
    const Datastruct::BaseProtocol *mProtocol;          /*! 解析的协议 */
    bool isFilter;                                      /*! 是否过滤   */

    QList<Data_Elem*>    mNeedsDisplayData;             /*! 需要进行显示的数据-不是全部数据 */


    friend class DataManager;
};

}


#endif // DATEVIEW_SUPERSTRUCT_H
