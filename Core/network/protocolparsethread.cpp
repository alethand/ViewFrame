#include "protocolparsethread.h"

#include "pluginmanager/pluginmanager.h"
#include "pluginmanager/rcomponent.h"
#include "Base/util/rsingleton.h"
#include "protocol/protocolmanager.h"

#include <QDebug>

namespace Core{

ProtocolParseThread::ProtocolParseThread()
{

}

ProtocolParseThread::~ProtocolParseThread()
{
    runningFlag = false;
    G_ProtocolQueueCondition.notify_one();
    wait();
}

/*!
 * @brief 注册解析的网络数据协议
 * @param[in] mid   模块id
 * @param[in] protos   此模块感兴趣的协议集合，为整形，与网络协议中数据类型字段保持一致
 * @param[in] ntype   网络通信链路方式
 */
void ProtocolParseThread::registNetworkObserver(QString mid, QStringList protos, NetworkType ntype)
{
    std::lock_guard<std::mutex> lg(pmutex);
    std::for_each(protos.begin(),protos.end(),[&](QString protocol){
        MProtocolList * list = interstedProtocol.value(protocol.toInt());
        if(list == NULL){
            list = new MProtocolList;
            interstedProtocol.insert(protocol.toInt(),list);
        }

        list->append({ntype,mid});
    });

}

void ProtocolParseThread::startMe()
{
    RTask::startMe();
    runningFlag = true;
    start();
}

void ProtocolParseThread::stopMe()
{
    RTask::stopMe();
    runningFlag = false;
}

void ProtocolParseThread::run()
{
    while(runningFlag){

        while(runningFlag && G_ProtoQueue.size() == 0){
            std::unique_lock<std::mutex> lg(G_ProtocolQueueMutex);
            G_ProtocolQueueCondition.wait(lg);
        }

        if(runningFlag){
            G_ProtocolQueueMutex.lock();
            ProtocolArray orginData = G_ProtoQueue.dequeue();
            G_ProtocolQueueMutex.unlock();

            parsedProtocol(orginData);
        }
    }
}

/*!
 * @brief 按照协议解析原始数据
 * @details 1.查找对应的网络协议是否存在,不存在直接跳过返回；
 *          2.查找对此网络协议订阅的模块是否存在，不存在直接跳过返回；
 *          3.按照协议解析对应的字段；
 *          4.将数据转发至对应的模块；
 * @param[in] array 原始数据
 * @return 是否解析成功
 */
bool ProtocolParseThread::parsedProtocol(ProtocolArray &array)
{
    bool existed = false;
    //[1]
    Datastruct::BaseProtocol proto = RSingleton<ProtocolManager>::instance()->getProtocol(array.protocolType,existed);
    if(!existed)
        return false;

    //[2]
    MProtocolList * list = interstedProtocol.value(array.protocolType);
    if(list == NULL)
        return false;

    //[3]
    Datastruct::FieldValues values;
    //TODO 20181108根据协议的字段解析值


    //[4]
    std::for_each(list->begin(),list->end(),[&](ModuleProtocol module){
        RComponent * comp = RSingleton<PluginManager>::instance()->getActivePlugin(module.moduleId);
        if(comp)
            comp->onNetwork(array.protocolType,values);
    });

    return true;
}

}
