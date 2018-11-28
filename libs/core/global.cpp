#include "global.h"

namespace Core {

namespace RGlobal
{

ServiceType G_SERVICE_TYPE;             /*!< 当前服务器运行的模式，根据不同的模式，网络数据解析存在不同 */
QString G_FILE_UPLOAD_PATH;             /*!< 文件上传保存的路径 */
DBFeature G_DB_FEATURE;                 /*!< 当前数据库驱动支持的功能 */

GlobalConfigFile * G_GlobalConfigFile;

std::mutex G_ProtocolQueueMutex;
std::condition_variable G_ProtocolQueueCondition;
Datastruct::ProtocolQueue G_ProtoQueue;

//Udp
std::mutex G_Udp_ProtocolQueueMutex;
std::condition_variable G_Udp_ProtocolQueueCondition;
Datastruct::ProtocolQueue G_Udp_ProtoQueue;

} //namespace RGlobal

} //namespace Core

