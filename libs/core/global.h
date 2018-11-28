#ifndef CORE_GLOBAL_H_2018_22_27
#define CORE_GLOBAL_H_2018_22_27

#include <QString>
#include <mutex>
#include <condition_variable>

#include <core/protocol/datastruct.h>
#include <core/core_global.h>

namespace Core{

using namespace Datastruct;
class GlobalConfigFile;

namespace RGlobal
{

extern ServiceType G_SERVICE_TYPE;             /*!< 当前服务器运行的模式，根据不同的模式，网络数据解析存在不同 */
extern QString G_FILE_UPLOAD_PATH;             /*!< 文件上传保存的路径 */
extern DBFeature G_DB_FEATURE;                 /*!< 当前数据库驱动支持的功能 */

extern GlobalConfigFile CORESHARED_EXPORT * G_GlobalConfigFile;

//Tcp
extern std::mutex G_ProtocolQueueMutex;
extern std::condition_variable G_ProtocolQueueCondition;
extern Datastruct::ProtocolQueue G_ProtoQueue;

//Udp
extern std::mutex G_Udp_ProtocolQueueMutex;
extern std::condition_variable G_Udp_ProtocolQueueCondition;
extern Datastruct::ProtocolQueue G_Udp_ProtoQueue;

}

} //namespace Core

#endif // GLOBAL_H
