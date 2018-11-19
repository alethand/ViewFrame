#include "protocolparsethread.h"

#include "pluginmanager/pluginmanager.h"
#include "pluginmanager/rcomponent.h"
#include "Base/util/rsingleton.h"
#include "protocol/protocolmanager.h"
#include "network/netparseinfo.h"

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
            beforeParsing(orginData);
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
bool ProtocolParseThread::beforeParsing(ProtocolArray &array)
{
    //[1]
    if(!RSingleton<ProtocolManager>::instance()->isExisted(array.protocolType))
        return false;

    //[2]
    MProtocolList * list = interstedProtocol.value(array.protocolType);
    if(list == NULL)
        return false;

    //[3]
   // Datastruct::ParsedResult parsedResult;
    //if(parsedProtocol(array,parsedResult))
    bool existed = false;
    Datastruct::BaseProtocol *protocol =const_cast< Datastruct::BaseProtocol*>
            (RSingleton<ProtocolManager>::instance()->getProtocol(array.protocolType,&existed) );

    QSharedPointer<NetParse::Protocol> pcurProtocol = NetParse::DataGenertor::startParse(array,protocol);
    {

        //[4]
        std::for_each(list->begin(),list->end(),[&](ModuleProtocol module){
            //BUG 20181117通过module.moduleId获取不到对应的插件
            RComponent * comp = RSingleton<PluginManager>::instance()->getActivePlugin(module.moduleId);
            if(comp)
                 comp->onNetWork(array.protocolType,pcurProtocol.data());
//                comp->onNetwork(array.protocolType,parsedResult);
        });
        return true;
    }
    return false;
}

/*!
 * @brief 对照协议解析原始数据
 * @details 所解析的协议格式可分为3段：【head】【data】【tail】，而【data】区又可进一步的细分为：
 *          【Num】【{[n1][a1][n2][b1]..}{[n1][a1][n2][b1]..}】
 *          其中Num字段通过BaseProtocol中count、memoryBytes确定； @n
 *          a1、b1等代表不同的协议， @n
 *          n1、n2在代表a1、b1协议字段数量，可根据SignalProtocol中count、memoryBytes确定 @n
 *          1.解析协议中count字段，确定数据体data部分 @n
 *          2.循环解析items中字段值 @n
 * @param[in]  array 原始数据
 * @param[in]  values 保存解析后的结果值
 * @return 是否解析成功
 */
bool ProtocolParseThread::parsedProtocol(ProtocolArray &array,Datastruct::ParsedResult & values)
{
    bool existed = false;
    const Datastruct::BaseProtocol *protocol = RSingleton<ProtocolManager>::instance()->getProtocol(array.protocolType,&existed);
    if(NULL == protocol)
        return false;

    int posPointer = 0;
    if(protocol->startLen > 0 && protocol->startLen < array.data.length()){
        posPointer = protocol->startLen;
    }else{
        return false;
    }

    //[1]
    int dataRepeatTime = getProtocolRepeatTime(array,protocol->memoryBytes,protocol->count,posPointer);
    if(dataRepeatTime == -1)
        return false;

    //[2]
    for(int i = 0 ; i < dataRepeatTime; i++){//最外层协议重复次数
        ParsedResult * outerResult = new ParsedResult;
        for(int j = 0; j < protocol->contents.size();j++){//协议循环体中单层协议个数
            Datastruct::SingleProtocol signalProtcol = protocol->contents.at(j);
            ParsedResult * innerResult = new ParsedResult;
            if(!parsedSignalProtocol(array,posPointer,signalProtcol,innerResult)){
                delete innerResult;
                return false;
            }
            outerResult->results.push_back(innerResult);
        }
        values.results.append(outerResult);
    }

    return true;
}

/*!
 * @brief 解析单个协议数据
 * @details 遍历当前协议中字段结合，根据字段的类型、长度等信息从原始数据中提取对应的结果，并按照顺序保存至集合。
 * @param[in] array 原始数据
 * @param[in/out] pos 数据指针位置
 * @param[in] protocol 协议文件
 * @return true：解析成功
 *         false：解析失败
 */
bool ProtocolParseThread::parsedSignalProtocol(const ProtocolArray &array,int & posPointer,Datastruct::SingleProtocol & protocol,Datastruct::ParsedResult * result)
{
    int sprotocolRepeatTime = getProtocolRepeatTime(array,protocol.memoryBytes,protocol.count,posPointer);
    if(sprotocolRepeatTime == -1)
        return false;

    bool isError = false;


    for(int i = 0; i<sprotocolRepeatTime; i++)//单层协议循环次数
    {
        Datastruct::ParsedResult * protocolResult = new Datastruct::ParsedResult;
        for(int j=0;j<protocol.fields.size();j++)//单层协议字段个数
        {
            Data_Word fieldData = protocol.fields.at(j);

            //1.根据字段中repeat节点来获取多个值
            int repeatTime = 0;
            if(fieldData.repeat <= 1){
                repeatTime = 1;
            }else{
                repeatTime = fieldData.repeat;
            }

            Datastruct::Field_Protocol fieldValue;
            fieldValue.index = fieldData.index;
            for(int k = 0;k<repeatTime;k++){
                //2`.判断是为位操作还是字节操作
                if(fieldData.bitOperator){
                    Datastruct::FieldValues values = getBitsData(array, fieldData.type, posPointer,fieldData.bytes,isError);
                }else{
                    QVariant tmpData = getBytesData(array, fieldData.type, posPointer,fieldData.bytes,isError);
                    if(!isError)
                        return false;
                    if(repeatTime == 1){
                        fieldValue.index = fieldData.index;
                        fieldValue.signalValue.value = tmpData;
                    }else{
                        if(fieldValue.valList == NULL){
                            fieldValue.valList = new ByteValues;
                        }

                        ByteValue multiValue;
                        multiValue.value = tmpData;
                        fieldValue.valList->append(multiValue);
                    }
                }
            }
            protocolResult->fieldResults.append(fieldValue);
        }
        result->results.append(protocolResult);
    }

    return true;
}

/*!
 * @brief 获取协议头循环次数
 * @param[in/out] array 原始数据
 * @param[in] memoryBytes 占用字节长度
 * @param[in] count 循环次数，>0表示固定循环次数；-1：表示不固定长度，需要通过截取协议中的memoryBytes来确定长度
 * @param[in] pos 数据指针位置
 * @return -1：获取结果失败；
 *         n：协议体循环实际次数
 */
int ProtocolParseThread::getProtocolRepeatTime(const ProtocolArray &array,int memoryBytes,int count,int &pos)
{
    int repeatTime = 0;
    if(count > 0 && memoryBytes == 0 ){
        repeatTime = count;
    }else{
        if(memoryBytes <= 0 || memoryBytes > 4){
            return -1;
        }
        bool isError = false;
        QVariant tmpData = getBytesData(array,Datastruct::ValueIntEdit,pos,memoryBytes,isError);
        if(isError)
            return -1;
        repeatTime = tmpData.toInt();
    }
    return repeatTime;
}

/*!
 * @brief 从原始数据中获取指定位置，指定数量字节的数据
 * @attention 为了简化过程，将相同类型的按照向上转型: @n
 *            char、short、int、long 转换为 int @n
 *            float、double 转换为 double @n
 *            字符型保持不变 @n
 * @param[in] array 原始数据信息
 * @param[in] type 字段类型
 * @param[in] pos 位置偏移量
 * @param[in] bytes 获取字节的长度
 * @param[in/out] error 是否出现转换错误
 * @return 通用类型数据
 */
QVariant ProtocolParseThread::getBytesData(const ProtocolArray &array, ControlType type, int &pos, int bytes, bool & error)
{
    QVariant result;
    error = false;

    if(pos + bytes >= array.data.length()){
        error = true;
        return result;
    }

    QByteArray newArray = array.data.mid(pos,bytes);

    switch(type){
        case Datastruct::ComboBox:
        case Datastruct::CheckBox:
        case Datastruct::RadioButton:
        case Datastruct::ValueIntEdit:
            {
                int tmpData = 0;
                memcpy(&tmpData,newArray.data(),newArray.size());
                result.fromValue(tmpData);
            }
            break;

        case Datastruct::ValueFloatEdit:
            break;

        case Datastruct::TextEdit:
            break;

        default:
            break;
    }

    return result;
}

/*!
 * @brief 按位解析数据信息
 * @param[in] array 原始数据信息
 * @param[in] type 字段类型
 * @param[in] pos 位置偏移量
 * @param[in] bytes 获取字节的长度
 * @param[in/out] error 是否出现转换错误
 * @return 位数据信息描述
 */
FieldValues ProtocolParseThread::getBitsData(const ProtocolArray &array, ControlType type, int &pos, int bytes, bool &error)
{
    FieldValues values;
    error = false;

    return values;
}

}
