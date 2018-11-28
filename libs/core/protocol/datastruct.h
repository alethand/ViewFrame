/*!
 *  @brief     类型定义
 *  @details   定义了通用的结构体、枚举等变量
 *  @file      datastruct.h
 *  @author    wey
 *  @version   1.0
 *  @date      2018.03.02
 *  @warning
 *  @copyright NanJing RenGu.
 */
#ifndef DATASTRUCT_H
#define DATASTRUCT_H

/****************************new DataStruct***********************************/
#include <QString>
#include <QList>
#include <QVariant>
#include <QDataStream>
#include <QQueue>
#include <QRect>
#include <QDate>
#include <QTime>
#include <core/core_global.h>

#define RECV_BUFF 1024*64       /*!< 网络接收缓冲区空间，Tcp、Udp使用 */

namespace Core{

namespace Datastruct {

/*!
 *  @brief 系统支持的服务类型
 *  @details 用户以命令行方式输入对应的服务.
 */
enum ServiceType
{
    SERVICE_TEXT,               /*!< 使用传输文本服务,默认类型 */
    SERVICE_FILE                /*!< 使用传输文件服务 */
};

/*!
 *  @brief 数据库类型
 */
enum DatabaseType
{
    DB_NONE = 0,
    DB_MYSQL,               /*!< 使用MySql数据库*/
    DB_ORACLE               /*!< 使用Oracle数据库*/
};

/*!
 * @brief 解析结果
 */
enum ResultType
{
    PARSE_ERROR,            /*!< 发生错误*/
    VIEW_PROGRAM,           /*!< 查看程序信息*/
    EXEC_PROGRAM            /*!< 执行程序*/
};

struct CommandParameter
{
    CommandParameter()
    {
        serviceType = SERVICE_TEXT;
        dbType = DB_MYSQL;
        parseResult = PARSE_ERROR;
    }

    ServiceType serviceType;
    DatabaseType dbType;
    ResultType parseResult;
};


/*!
 *  @brief 数据库特征信息
 *  @note 保存当前使用的数据库中的功能特点
 */
struct DBFeature
{
    bool lastInsertId;          /*!< 是否支持查询最后插入数据的id */
    bool transactions;          /*!< 是否支持事务 */
};

/*!
 *  @brief 数据库配置信息
 */
struct DatabaseConfigInfo
{
    DatabaseType dbType;
    QString hostName;
    QString dbName;
    QString dbUser;
    QString dbPass;
    ushort port;
};

/*!
 *  @brief 系统配置信息
 */
struct SystemConfigInfo
{
    SystemConfigInfo():defaultKeySchemes(true),fullscreen(true),topHint(false),menubarVisible(false){
    }
    bool defaultKeySchemes;             /*!< 是否采用默认的快捷键设置，默认为true */
    bool fullscreen;                    /*!< 是否采用全屏显示 */
    bool topHint;                       /*!< 是否置顶显示 */
    bool menubarVisible;                /*!< 菜单栏是否显示 */
    QString userKeySchemesName;         /*!< 自定义快捷键设置名称 @see defaultKeySchemes 为false时有效 */
    QString locale;                     /*!< 显示的语言 */
    QString style;                      /*!< 默认样式 */
};

/*****************新框架*********************/

struct ProtocolArray{
    int protocolType;
    QByteArray data;
};

typedef QQueue<ProtocolArray> ProtocolQueue;

/*!
 *  @brief 控件类型
 */
enum ControlType{
    Empty = 0,
    /*! 输入框 */
    ComboBox = 1,        /*! 下拉框 */
    CheckBox,            /*! 复选框 */
    RadioButton,         /*! 单选框 */
    TextEdit,            /*! 文本输入框 */
    ValueIntEdit,        /*! 整形输入框 */
    ValueFloatEdit,      /*! 浮点形输入框 */
    DateEdit,            /*! 日期输入框 */
    TimeEdit,            /*! 时间输入框 */

    /*! 容器框 */
    Dialog,              /*! 对话框 */
    Widget,              /*! 普通框 */
    Groupbox,            /*! 组合框*/
    Table,               /*! 表格 */
    List,                 /*! 列表 */

    /*! 协议类型*/
    Count,               /*! 计数类型 */
};

struct PubHead{
    PubHead():widget(NULL){}
    virtual ~PubHead(){}
    QWidget  * widget;     /*!< 实际生成的控件 */
    ControlType  type;     /*!< 字段类型 */
};

/*!
 * @brief 数据范围
 * @details 根据Field的type来转换为对应的值，可转换的包括int、double、date等
 */
struct DataRange{
    QString minValue;
    QString maxValue;
};

/*!
 * \brief The Data_Elem struct 协议数据元
 */
struct Data_Elem{
    Data_Elem(){
        weight = 1;                 ///默认权值=1
        precision = 1;              ///默认精度=1
        type = Empty;               ///默认-使用Char类型存储
        enable = true;
        visible = true;
        name.clear();
    }

    QString  name;        /*!< 显示信息 --可以为空*/
    ControlType  type;    /*!< 字段类型 */
    //待定，删除下个数据
    int index;            /*!< 字段唯一标识符，初始为0。无需从xml中解析，新添加字段后，此值+1 */

    bool enable;          /*!< 控件是否可用 */
    bool visible;         /*!< 是否可见，可控制此字段是否显示在表格等控件上 */
    float weight;         /*!< 权值 */
    float precision;      /*!< 精度 */

    QString unit;         /*!< 单位 */
    QString displayText;  /*!< 显示文本 */
    QString defaultValue; /*!< 默认值 */
    QVariant value;       /*!< 控件中显示值 */
    QStringList list;     /*!< type为ComboBox时，保存下拉框中子项名称 */
};

/*!
 *  @brief 位数据信息描述
 *  @details
 */
struct CORESHARED_EXPORT Data_Bit:public Data_Elem{

    friend QDataStream & operator<< (QDataStream & stream,const Data_Bit & field);
    friend QDataStream & operator>> (QDataStream & stream,Data_Bit & field);


    int startPos;         /*!< 起始位 */
    int last;             /*!< 持续位数 */
};
typedef QList<Data_Bit> BitList;

/*!
 *  @brief 位字段
 *  @details
 */
struct CORESHARED_EXPORT BitField{

    friend QDataStream & operator<< (QDataStream & stream,const BitField & field);
    friend QDataStream & operator>> (QDataStream & stream,BitField & field);

    BitList bitList;        /*!< 位字段集合 */
};

/*!
 *  @brief 单个字段内容数据
 *  @warning (字节数-符号位) 与 (位操作集合) 这两对值不能同时使用，属于互斥关系
 */
struct CORESHARED_EXPORT Data_Word:public Data_Elem{
public:
    Data_Word():Data_Elem(){
        bytes =0;isSigned = true;   ///默认无符号
        bitOperator = false;
        pos_InProtocol = 0;
    }

    __declspec(dllexport) friend QDataStream & operator<<(QDataStream & stream,const Data_Word & data);
    __declspec(dllexport) friend QDataStream & operator>>(QDataStream & stream,Data_Word & data);
    bool bitOperator;     /*!< 位操作字段，当bits中不为0，此值为true */

    ushort bytes;         /*!< 字节数 */
    bool   isSigned;      /*!< 有无符号位 */

    BitField bits;        /*!< 位操作集合 */

    int repeat;           /*!< 重复的次数，此字段之后连续出现的次数 */

    DataRange range;      /*!< 数值范围 */

    //用于快速定位
    short pos_InProtocol;
};

/*!
 * \brief The ProtoCtlOperate enum 协议控制操作
 */
enum ProtoCtlOperate{
    EMPTY,
    COUNT   //计数功能
};


/*!
 *  @brief  协议字段信息
 *  @details 协议每项的信息描述
 */
struct Field : public PubHead{
    Field():PubHead(){}
    ~Field(){}
    void update();

    Data_Word data;        /*!< 字段属性 */
};

struct ByteValue{
    QVariant value;
};
typedef QList<ByteValue> ByteValues;


struct Field_Protocol{
    Field_Protocol():index(0),protocolIndex(0),valList(NULL){}
    int protocolIndex;          /*!< 所属协议在所有协议中索引 */
    int index;                  /*!< 同一协议中字段索引 */
    ByteValue signalValue;      /*!< 适用于单个字段值 */
    ByteValues * valList;       /*!< 适用于同一个字段被重复多次;字段属于bit位操作 */
};

typedef QList<Field_Protocol> FieldValues;

/*!
 *  @brief  保存解析后的结果值
 */
struct ParsedResult{
    FieldValues fieldResults;
    QList<ParsedResult * > results;
};

struct NodeInfo{
    NodeInfo():window("window"),name("name"),width("width"),height("height"),layout("layout")
    ,type("type"),widget("widget"),groupBox("groupbox"),item("item"),items("items"),nodeShow("show"),nodeColumn("column"),nodeEnabled("enabled"),
    itemName("name"),itemBytes("bytes"),itemSigned("signed"),itemBits("bits"),itemText("text"),
    itemWeight("weight"),itemPrecision("precision"),itemUnit("unit"),itemType("type"),itemComboxList("list"),itemRange("range"),itemEnable("enable"),
    itemVisible("visible"),itemDefaultValue("default"),bitStart("start"),bitLast("last"),itemMax("max"),itemMin("min"),itemRepeat("repeat"){

    }
    const QString window;
    const QString name;
    const QString width;
    const QString height;
    const QString layout;
    const QString type;

    const QString widget;
    const QString groupBox;

    const QString item;
    const QString items;

    const QString nodeShow;
    const QString nodeColumn;
    const QString nodeEnabled;

    const QString itemName;
    const QString itemBytes;
    const QString itemSigned;
    const QString itemEnable;
    const QString itemVisible;
    const QString itemBits;
    const QString itemWeight;
    const QString itemText;
    const QString itemPrecision;
    const QString itemUnit;
    const QString itemRange;
    const QString itemType;
    const QString itemComboxList;
    const QString itemDefaultValue;
    const QString itemMax;
    const QString itemMin;
    const QString itemRepeat;

    const QString bitStart;
    const QString bitLast;
};

/*!
 *  @brief xml文件中类型名称
 */
struct WidgetType{
    WidgetType():combox("combox"),checkBox("checkBox"),radioButton("radioButton"),textEdit("textEdit"),valueint("valueint"),
        valuefloat("valuefloat"),dateEdit("dateEdit"),timeEdit("timeEdit"),dialog("dialog"),widget("widget"),table("table"),list("list"),
    count("count"),length("length"){}

    /***显示控件类型***/
    QString combox;
    QString checkBox;
    QString radioButton;
    QString textEdit;
    QString valueint;
    QString valuefloat;
    QString dateEdit;
    QString timeEdit;
    QString dialog;
    QString widget;
    QString table;
    QString list;

    /*** 协议控制类型 ***/
    const QString count;
    const QString length;

};

/******************************任务控制模块*****************************************/

/*!
 *  @brief 布局类型
 *  @details 20181030暂支持3种类型
 */
enum Layout
{
    None,
    Horizonal,          /*!< Gridlayout布局一行，多列 */
    Vertical,           /*!< Gridlayout布局一列，多行 */
    Grid                /*!< Gridlayout布局多行，多列 */
};

/*!
 *  @brief 窗口数据信息
 */
struct WindowData{
    WindowData(){
        isShown = true;
        isEnabled = true;
        column = 1;
    }

    QString  name;        /*!< 控件前面label显示信息 */

    Layout layout;        /*!< 布局类型 */
    bool isShown;         /*!< 可进行显示 */
    bool isEnabled;       /*!< 可编辑 */
    ushort column;        /*!< 进行显示的列数 */
    ushort width;         /*!< 宽度 */
    ushort height;        /*!< 高度 */
};

/*!
 *  @brief 容器信息显示描述
 */
struct Window : public PubHead{
    Window():PubHead(){ }
    ~Window(){}
    WindowData data;       /*!< 窗口属性 */
};

/*!
 *  @brief 节点容器
 *  @details 1.容器可嵌套；
 *           2.大的容器中可包含子容器，由 @see dataElem 来定义；
 *           3.最内的容器只包含字段信息
 */
struct Container{
    Window     continer;                  /*!< 描述当前容器的属性 */
    QList<Field*> fileds;                 /*!< 节点集合 */
    QList<Container*>  childContainer;    /*!< 子容器 */
};
/******************************任务控制模块*****************************************/


/**************************健康管理/数据显示模块************************************/
/*!
 *  @brief 单个协议描述
 */
struct CORESHARED_EXPORT SingleProtocol{
    int count;                   /*!< 当前协议数量：>0：表示固定的数量，-1：表示数量不定，需结合memoryBytes */
    int memoryBytes;            /*!< 当count字段为变长时，count字段占用几字节空间,用于解析协议数量 */
    int length;                 /*!< 协议长度 */
    bool isProtocol;            /*!< 代表这是一个协议 yww新增*/
    QList<Data_Word> fields;    /*!< 协议字段集合 */
    short getBytes_To(int pos,int base = 0)const ;
};

/*!
 *  @brief 基本协议
 */
struct CORESHARED_EXPORT BaseProtocol{
    BaseProtocol(){bytes_Contents = 0;}

    QString name;       /*!< 协议名 */
    int startCode;      /*!< 开始标志码 */
    int type;           /*!< 协议类型 */
    int length;         /*!< 整包数据长度 */
    int startLen;       /*!< 数据头部固定长度 */
    int count;          /*!< 最小协议包数据量，>0:表示固定数量，-1:表示数量不定，需结合memoryBytes */
    int memoryBytes;    /*!< 当count字段为变长时，count字段占用几字节空间,用于解析协议数量 */
    QList<SingleProtocol> contents;    /*!< 数据内容 */
    int endLen;         /*!< 数据尾部固定长度 */
    int endCode;        /*!< 结束标志码 */

    //用于快速定位
    short bytes_Contents;//数据内容的长度
    short getBytes_Content();
};
/**************************健康管理/数据显示模块************************************/

/*!
 *  @brief 插件描述信息
 *  @details
 */
struct PluginInfo{
    QString id;         /*!< 插件标识 */
    QString name;       /*!< 名称 */
};

enum NetworkType{
    N_TCP,
    N_UDP
};

/*!
 *  @brief 网络连接类型
 */
enum NetworkConnectionType{
    N_Server,
    N_Client
};

/*!
 *  @brief 网络基本信息
 */
struct NetworkBase{
    QString ip;
    ushort port;
    NetworkConnectionType connectionType;
    bool multicast;         /*!< 是否开启组播 */
    QString multicastIp;    /*!< 组播Ip */
};

/*!
 *  @brief 网络配置信息
 *  @details 初始化网络模块
 */
struct NetworkInfo{
    QString id;             /*!< 网络Id */
    NetworkBase baseInfo;   /*!< 基础信息 */
    NetworkType protocol;   /*!< 传输类型 */
};

enum WindowLayout{
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

/*!
 *  @brief  模块信息
 *  @details
 */
struct ModuleInfo{
    WindowLayout layout;    /*!< 在mainwindow中的位置方向 */
    QRect geometry;         /*!< 尺寸信息 */
    bool closeable;         /*!< 是否可关闭 */
    bool visible;           /*!< 是否可见 */
    bool floatable;         /*!< 是否可移动 */
    QString name;           /*!< 模块名称 */
    QStringList protocols;  /*!< 协议id， @see  ProtocolInfo */
    QString pluginId;       /*!< 内部插件ID @see PluginInfo */
    QString networkId;      /*!< 网络模块id， @see BaseProtocol */
};

/*!
 * @brief Tcp/Udp注册待需处理数据结构信息
 */
struct ModuleProtocol{
    int startCode;      /*!< 开始标志码 */
    int type;           /*!< 协议类型 */
    int length;         /*!< 整包数据长度 */
    int endCode;        /*!< 结束标志码 */
    QString moduleId;   /*!< 模块Id */
};

} //namespace Datastruct

} //namespace Core

#endif // DATASTRUCT_H
