/*!
 *  @brief     网络层协议文件
 *  @details   定义即时通讯软件网路层传输控制协议
 *  @file      head.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.02.23
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef HEAD_H
#define HEAD_H

#include <QByteArray>
#include <QLinkedList>
#include <functional>

#ifdef Q_OS_WIN
#include <WinSock2.h>
#include <Windows.h>
#endif

#define IO_BUFF_SIZE  1024
#define MAX_RECV_SIZE 1024*8

#define MAX_PACKET 512                          /*!< 应用层数据最大发送长度，超过此长度后，网络层会进行拆包。 @attention 未加入网络层协议头， @link DataPacket @endlink */
#define MAX_SEND_BUFF (MAX_PACKET + 24)         /*!< 网络层最大发送长度 */

#define SOCK_CHAR_BUFF_LEN  32                  /*!< 存储ip地址集合长度 */

/*!
 *  @brief  接收/发送魔数
 *  @details 用于对接收数据的校验，服务器的接收与客户端的发送保持一致，服务器的发送与客户端的接收保持一致。
 */
#define RECV_MAGIC_NUM 0x7DBCD6AC
#define SEND_MAGIC_NUM 0xD7CB6DCA

typedef int(*Func)(const char *,const int);
typedef std::function<void(QByteArray &)> ByteArrayHandler;

namespace Network{
class IocpContext;
}
//(socketfd,content,len)
typedef std::function<bool(int,Network::IocpContext *,DWORD &)> IocpContextSender;

//(socketfd,buff,len)
typedef std::function<int(int,const char*,int)> ByteSender;

//(socketfd,buff,len)
typedef std::function<int(const char*,int,const char *,int)> UDPByteSender;

#pragma pack(push)
#pragma pack(1)

/*!
 *  @brief 网络层数据传输控制头
 *  @details 在传输时网络层包裹了应用层的数据内容，用于即时通讯软件网络层交互时对大数据的包的分包传输以及组包，同时可对数据的正确性进行校验。
 */
struct DataPacket
{
    unsigned int magicNum;                   /*!< 魔数 @link RECV_MAGIC_NUM @endlink */
    unsigned int packId;                     /*!< 数据包身份，一个id至少对应一个数据帧。 */
    unsigned int currentIndex;               /*!< 当前帧索引 @attention 起始索引为0  */
    unsigned int totalIndex;                 /*!< 总帧数量 */
    unsigned int currentLen;                 /*!< 当前数据长度 */
    unsigned int totalLen;                   /*!< 总数据长度 */
};


/*!
 *  @brief 数据包接收缓冲
 *  @details 可用于接收断包数据
 */
struct PacketBuff
{
    PacketBuff()
    {
        totalPackIndex = 0;
        recvPackIndex = 0;
        recvSize = 0;
        isCompleted = false;
    }

    /*!
     * @brief 获取缓存接收的分段数据
     * @note 将缓存接收的数据重新组装，拼接成新的数据；并在拼接后的数据头部插入协议头(21+2051) \n
     * @param[in] container 数据容器
     * @param[in] perPacketOffset 截取每个分段的起始点
     */
    void packDataWidthPrtocol(QByteArray & container,int perPacketOffset){
        if(isCompleted && recvSize > 0)
        {
            if(buff.size() > 0){
                container.append(buff.takeFirst());
            }

            while(!buff.isEmpty())
            {
                container.append(buff.takeFirst().mid(perPacketOffset));
            }
        }
    }

    QByteArray getFullData()
    {
        QByteArray data;
        if(isCompleted && recvSize > 0)
        {
            while(!buff.isEmpty())
            {
                data.append(buff.takeFirst());
            }
        }
        return data;
    }

    bool isCompleted;                           //该缓冲数据是否完整
    unsigned int recvSize;                      //接收数据的长度
    unsigned short totalPackIndex;              //总数据包分包大小
    unsigned short recvPackIndex;               //已经接收到数据的索引，在recvPackIndex==totalPackIndex时由处理线程进行重新组包

    qint64 totalPackLen;                        /*!< 总数据长度(分片数量*495+数据部分) */

    QLinkedList<QByteArray> buff;               //存放接收到数据(不包含网络数据头DataPacket)，插入时recvPackIndex+1
};

/*!
 *  @brief 传输方式
 *  @details 数据从本机出去的方式包含TCP、UDP、总线，至于北斗等方式，也是UDP传输至中间某一台位后，再转发。
 */
enum CommMethod{
    C_NONE,     /*!< 错误方式*/
    C_UDP,      /*!< UDP方式 */
    C_TCP,      /*!< TCP方式 */
    C_BUS       /*!< 总线方式 */
};


/*!
 *  @brief 对数据进行协议组包的时候，必须要用到的外部描述信息集合
 *  @warning 【若此结构发生变动，同步修改SQL/Datatable/RChat716Cache类】
 *  @details 填写协议头部信息时，像包长这类的字段信息可以实时获取，但是像站点等类型的信息的无法在实时组包的时候获取
 */
struct ProtocolPackage
{
    QByteArray data;               /*!< 正文内容 */

    ProtocolPackage()
    {
    }

    ProtocolPackage(QByteArray dataArray){
        this->data = dataArray;
    }

    ProtocolPackage operator=(const ProtocolPackage & package)
    {
        this->data = package.data;
        return *this;
    }
};

/*!
 *  @brief 待发送数据单元描述，包含了发送时使用的方式，以及待发送的数据 \n
 *         ProtocolPackage可作为通用的数据发送格式，用户可在此结构体内部扩充信息。
 */
struct SendUnit
{
    SendUnit(){}
    SOCKET sockId;                  /*!< 客户端Socket标识Id */
    CommMethod method;              /*!< 发送方式 */
    ProtocolPackage dataUnit;       /*!< 待发送数据包及描述 */
};

/*!
 *  @brief  接收时由网络层提取出的信息字段，该结构信息可用于应用层进行数据处理
 *  @note   需要由网络层向应用层传递数据时，可在此结构体内添加字段，并在网络层中将对应的信息赋值即可。
 */
struct ExtendData
{
    ExtendData(){
        sockId = 0;
    }
    SOCKET sockId;                  /*!< 客户端Socket标识Id */
    CommMethod method;              /*!< 接收方式 */
};

/*!
 *  @brief 数据处理单元
 *  @details 封装了客户端的每个请求，服务器处理线程从列表中取出每个单元，根据应用层传输协议进行相应处理。
 */
struct RecvUnit
{
    ExtendData extendData;      /*!< 可扩充数据信息，包含网络层相关信息 */
    QByteArray data;            /*!< 请求数据内容，已经去除网络层( @link DataPacket @endlink )数据头，但其它应用层的数据格式头没有去除 */
};

typedef std::function<void(RecvUnit &)> DataHandler;

#pragma pack(pop)

#endif // HEAD_H
