/*!
 *  @brief     协议解析线程
 *  @details   1.处理网络线程打包好的原始协议数据；
 *             2.将解析后的数据分发给不同的插件；
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.08
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PROTOCOLPARSETHREAD_H
#define PROTOCOLPARSETHREAD_H

#include "rtask.h"

#include "protocol/datastruct.h"
#include "global.h"
using namespace RGlobal;

#include <QHash>

namespace Core{

class ProtocolParseThread : public RTask
{
    Q_OBJECT
public:
    explicit ProtocolParseThread();
    ~ProtocolParseThread();

    void registNetworkObserver(QString mid,QStringList protos,NetworkType ntype = Datastruct::N_TCP);

    void startMe();
    void stopMe();

    struct ModuleProtocol{
        NetworkType ntype;          /*!< 网络类型 */
        QString moduleId;           /*!< 模块Id,组件的objectName */
    };

    struct ProtocolHead{
        int count;
        int memoryBytes;
    };

    typedef QList<ModuleProtocol> MProtocolList;

protected:
    void run();

private:
    bool beforeParsing(Datastruct::ProtocolArray & array);
    bool parsedProtocol(ProtocolArray &array, ParsedResult &values);
    bool parsedSignalProtocol(const ProtocolArray &array, int & posPointer, Datastruct::SignalProtocol & protocol, Datastruct::ParsedResult *result);
    int getProtocolRepeatTime(const ProtocolArray &array,int memoryBytes,int count,int &pos);
    QVariant getBytesData(const ProtocolArray &array,Datastruct::ControlType type,int & pos,int bytes,bool & error);
    FieldValues getBitsData(const ProtocolArray &array,Datastruct::ControlType type,int & pos,int bytes,bool & error);

private:
    std::mutex pmutex;
    QHash<int,MProtocolList *> interstedProtocol;

};

}

#endif // PROTOCOLPARSETHREAD_H
