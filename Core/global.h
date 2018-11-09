#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include "protocol/datastruct.h"

#include <mutex>
#include <condition_variable>

using namespace Datastruct;
class GlobalConfigFile;

namespace RGlobal
{

extern ServiceType G_SERVICE_TYPE;             /*!< 当前服务器运行的模式，根据不同的模式，网络数据解析存在不同 */
extern QString G_FILE_UPLOAD_PATH;             /*!< 文件上传保存的路径 */
extern DBFeature G_DB_FEATURE;                 /*!< 当前数据库驱动支持的功能 */

extern GlobalConfigFile * G_GlobalConfigFile;

extern std::mutex G_ProtocolQueueMutex;
extern std::condition_variable G_ProtocolQueueCondition;
extern Datastruct::ProtocolQueue G_ProtoQueue;

}

#endif // GLOBAL_H
