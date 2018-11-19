#ifndef NETPARSEINFO_H
#define NETPARSEINFO_H

#include <QList>
#include <protocol/datastruct.h>
#include <QSharedPointer>

/*!
 * 网络解析信息
 * @note 有些装饰者的影子
 */
using namespace Datastruct;
namespace NetParse{

struct Elem;
struct Protocol;
struct ByteData;
struct DomainGroup;

class DataGenertor{//数据生成器
public:

static QSharedPointer<NetParse::Protocol> startParse(ProtocolArray &array,Datastruct::BaseProtocol *protocol);

private:
    explicit DataGenertor(){}
    bool parsedSignalProtocol(const QDataStream &stream,Datastruct::SingleProtocol * protocol,Protocol *result);

    bool parsedDomainGroup(const QDataStream &stream,Datastruct::BaseProtocol *protocol,DomainGroup *result);

    int getRepeatTimes(const QDataStream &stream, SingleProtocol *protocol,Elem *result);
    int getRepeatTimes(const QDataStream &stream, BaseProtocol *protocol,Elem *result);


    static DataGenertor *instance;
};

Elem* getData_InPos(Protocol *protocol,int curCnt,int allCnt,bool isFilter);


enum CtlOperate{///协议控制操作
    Cntl_EMPTY,
    Cntl_LENGTH,
    Cntl_COUNT,   //计数功能
    Cntl_CHECK   //校验功能
};

enum FieldTye{///字段类型
    T_Protocol,
    T_DomainGroup,
    T_ByteData,
    T_BitData

};

struct Elem{
    Elem(){mlenth = 0;}
    FieldTye mType;
    virtual void write(QDataStream &stream){Q_UNUSED(stream)}
    virtual void write(QVariant &data,int len){Q_UNUSED(data)Q_UNUSED(len)}
    virtual void read(QDataStream & stream){Q_UNUSED(stream)}
    virtual void read(QVariant &data,int len){Q_UNUSED(data)Q_UNUSED(len)}

    virtual QString getValue(bool isFilter = true){Q_UNUSED(isFilter)return QString();}

    virtual short getLenth()=0;
protected:
    short mlenth;

};

struct ByteData:public Elem{///字节数据
    explicit ByteData(const Datastruct::Data_Word *defination);
    explicit ByteData(int len,CtlOperate type);
    ~ByteData();
    void setLenth(int i){mlenth = i;}
    virtual void  write(QDataStream &stream);
    virtual void  read(QDataStream & stream);

     virtual QString getValue(bool isFilter = true);

    QVariant value;
    CtlOperate mCtlType;
private:
    const Datastruct::Data_Word *pDef;

    virtual short getLenth();

};

struct BitData:public Elem{///位数据
    /*!！！！ 位数据暂时默认不超过1字节*/
    explicit BitData(Datastruct::Data_Bit *defination = NULL);
    ~BitData();
    virtual void write(QVariant &data,int len=1);
    virtual void read(QVariant &data,int len=1);

    virtual QString getValue(bool isFilter = true);

    char value;
    Datastruct::Data_Bit *pDef;
 private:
    virtual short getLenth();
};


/*!
 * \brief The DomainGroup struct  域组
 *           域组是一种可以对其中的元素进行一些协议控制操作的新型结构
 *           目前支持：计数，校验
 *           内容物：可包括协议，字数据，位数据集合，与这些类别的混合物
 */
struct DomainGroup:public Elem{
    explicit DomainGroup();
    virtual ~DomainGroup();
    virtual void  write(QDataStream &stream);
    virtual void  read(QDataStream & stream);

    QList<Elem*> mElems;


    virtual short getLenth();
private:
    enum BitOperate{
      Write,
      Read,
      Check
    };
    int isAllBitData;      //全位数据 -1,未检测，1是位数据,0是非位数据
    bool handleBitDatas(BitOperate operate,QDataStream *stream);    ///位数据操作
};


/*!
 * \brief The Protocol struct 协议
 */
struct Protocol:public Elem{
    explicit Protocol(){mType = T_Protocol;}
    virtual ~Protocol();

    virtual void  write(QDataStream &stream);
    virtual void  read(QDataStream & stream);

    QList<Elem*> mElems;                /*! 元素集 */

    virtual short getLenth();
};




}
#endif // NETPARSEINFO_H
