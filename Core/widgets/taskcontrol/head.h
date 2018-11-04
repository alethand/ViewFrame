/*!
 *  @brief     任务控制协议
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TASK_CONTROL_HEAD_H
#define TASK_CONTROL_HEAD_H

#include <QString>
#include <QVariant>
#include <QList>
#include <QDateTime>
#include <QDataStream>

#include <QDebug>

#define LABEL_MIN_WIDTH 120
#define LABEL_MIN_HEIGHT 25

//编辑框、combobox的固定大小
#define LINEDIT_FIXED_WIDTH 173
#define LINEDIT_FIXED_HEIGHT 25

namespace TaskControlModel {

/**
 * @brief The operationalOrder enum
 */
enum Order{
    ADD,
    DELETE,
    REVISE
};

enum Type{
    Band = 0,           /*!< 频段控制 */
    State,              /*!< 状态控制 */
    Gather,             /*!< 采集控制 */
    SelfCheck,          /*!< 自检控制 */
    Instrument,         /*!< 仪器控制 */
    Turntable,          /*!< 转台控制 */
    PlayBack            /*!< 回放控制 */
};

/*!
 *  @brief 表头字段
 */
enum TaskHead{
    T_No = 0,               /*!< 序号 */
    T_TYPE,                 /*!< 任务类型 */
    T_PARAMETERS,           /*!< 任务参数 */
    T_E_TIME,               /*!< 任务执行时间 */
    T_E_TIME_LONG,          /*!< 任务执行时长 */
    T_DIS_STATE             /*!< 下发状态 */
};

/*!
 *  @brief 下发状态
 */
enum DistuributeState{
    Not_Issued,       /*!< 未下发 */
    Issued,           /*!< 已下发 */
    Error_Issued      /*!< 下发错误 */
};


/*!
 *  @brief 控件类型
 */
enum ControlType{

    Empty = 0,
    /*! 输入框 */
    ComboBox = 1,         /*! 下拉框 */
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
    Groupbox,            /*! */
    Table,               /*! 表格 */
    List,                /*! 列表 */

    /*! 其它协议类型 */
    Length,              /*! 长度 */
    Count                /*! 计数个数 */
};

struct PubHead{
    PubHead():widget(NULL){}
    virtual ~PubHead(){}
    QWidget  * widget;     /*!< 实际生成的控件 */
    ControlType  type;     /*!< 字段类型 */
};

/*!
 *  @brief 单个字段内容数据
 */
struct FieldData{
public:
    FieldData(){
        bytes =0;isSigned = false;
        bits = 0; offset = 0;
        weight = 1;
        precision = 1;
        maxValue = USHRT_MAX;
        minValue = 0;
    }

    friend QDataStream & operator<<(QDataStream & stream,const FieldData & data);
    friend QDataStream & operator>>(QDataStream & stream,FieldData & data);

    QString  name;         /*!< 控件前面label显示信息 */

    /*! 以下内容只有1组有效：字节数-符号位，比特数-偏移量 */
    int index;            /*!< 字段唯一标识符，初始为0。新添加字段后，此值+1 */
    ushort bytes;         /*!< 字节数 */
    bool   isSigned;      /*!< 有无符号位 */
    ushort bits;          /*!< 比特位数 */
    ushort offset;        /*!< 偏移量 */
    float weight;         /*!< 权值 */
    float precision;      /*!< 精度 */
    QString unit;         /*!< 单位 */
    ushort maxValue;      /*!< 最大值 */
    ushort minValue;      /*!< 最小值 */
    QString displayText;  /*!< 显示文本 */

    ControlType  type;    /*!< 字段类型 */
    QVariant value;       /*!< 值 */
};

/*!
 *  @brief  协议字段信息
 *  @details 协议每项的信息描述
 */
struct Field : public PubHead{
    Field():PubHead(){}
    ~Field(){}

    FieldData data;        /*!< 字段属性 */
};

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

/*!
 *  @brief 新基本任务信息
 *  @details 动态解析xml文件，保存不同的任务的字段信息。
 */
struct NewTaskInfo
{
    NewTaskInfo(){
        this->dstate = Not_Issued;
        userChecked = false;
    }

    NewTaskInfo(const NewTaskInfo & info){
        this->userChecked = info.userChecked;
        this->taskName = info.taskName;
        this->parameter = info.parameter;
        this->excuteTime = info.excuteTime;
        this->lastTime = info.lastTime;
        this->dstate = info.dstate;
        this->fields = info.fields;
        this->localParsedFileName = info.localParsedFileName;
    }

    friend QDataStream & operator<<(QDataStream & stream,const NewTaskInfo & info);
    friend QDataStream & operator>>(QDataStream & stream,NewTaskInfo & info);

    bool userChecked;               /*!< 用户是否选中 */
    QString taskName;               /*!< 任务名称(从解析的xml文件名中提取) */
    QString parameter;              /*!< 任务参数 */
    QDateTime excuteTime;           /*!< 任务执行时间 */
    quint32 lastTime;               /*!< 任务执行时长 */
    DistuributeState dstate;        /*!< 下发状态 */
    QMap<int,FieldData> fields;     /*!< 某个任务下字段集合,key:int对应当前字段在所有字段中的索引，value:QVaraint,对应字段的值 */

    QString localParsedFileName;    /*!< 本地xml文件全路径 */

};

typedef QList<NewTaskInfo *> NewTaskList;

/*!
 *  @brief 基本任务信息
 *  @details 作为所有任务的抽象类，包含通用的基本信息
 */
struct TaskInfo
{
    TaskInfo(){
        this->dstate = Not_Issued;
        userChecked = false;
    }
    TaskInfo(const TaskInfo & info){
        this->userChecked = info.userChecked;
        this->taskType = info.taskType;
        this->parameter = info.parameter;
        this->excuteTime = info.excuteTime;
        this->lastTime = info.lastTime;
        this->dstate = info.dstate;
    }
    virtual ~TaskInfo(){}
    bool userChecked;               /*!< 用户是否选中 */
    Type taskType;                  /*!< 任务类型 */
    QString parameter;              /*!< 任务参数 */
    QDateTime excuteTime;           /*!< 任务执行时间 */
    quint32 lastTime;               /*!< 任务执行时长 */
    DistuributeState dstate;        /*!< 下发状态 */

    virtual void dispatch() = 0;
};

typedef QList<TaskInfo *> TaskInfoList;

/*!
 *  @brief 频段控制
 */
struct BandControl: public TaskInfo
{
    BandControl():TaskInfo(){
        taskType = Band;
    }
    BandControl(const BandControl & info):TaskInfo(info){
        this->originFrequency = info.originFrequency;
        this->stopFrequency = info.stopFrequency;
        this->frequencyStopping = info.frequencyStopping;
    }

    friend QDataStream & operator<<(QDataStream & stream,const BandControl & info);
    friend QDataStream & operator>>(QDataStream & stream,BandControl & info);

    ~BandControl(){}
    double originFrequency;         /*!< 起始频率(HMz) */
    double stopFrequency;           /*!< 终止频率(HMz) */
    double frequencyStopping;       /*!< 频率步进(HMz) */

    void dispatch(){}
};

/*!
 *  @brief 状态控制
 */
struct StateControl  : public TaskInfo
{
    StateControl():TaskInfo()
    {
        taskType = State;
        workCyclesNumber = 1;
    }
    StateControl(const StateControl & info):TaskInfo(info){
        this->workmode = info.workmode;
        this->workCycles = info.workCycles;
        this->workCyclesNumber = info.workCyclesNumber;
        this->workingBandwidth = info.workingBandwidth;
        this->antennaSelection1 = info.antennaSelection1;
        this->antennaSelection2 = info.antennaSelection2;
        this->attenuationCode1 = info.attenuationCode1;
        this->attenuationCode2 = info.attenuationCode2;
        this->attenuationCode3 = info.attenuationCode3;
        this->attenuationCode4 = info.attenuationCode4;
        this->attenuationCode5 = info.attenuationCode5;
        this->attenuationCode6 = info.attenuationCode6;
        this->attenuationCode7 = info.attenuationCode7;
        this->attenuationCode8 = info.attenuationCode8;
        this->attenuationCode9 = info.attenuationCode9;
        this->attenuationCode10 = info.attenuationCode10;
    }
    ~StateControl(){}

    char workmode;                   /*!< 工作模式 0自检  1频域搜索  2驻留  3空域搜索  4中频采集  5敌我1030  6敌我1090 */
    char workCycles;                 /*!< 工作周期(50ms)   最大1000ms (20) */
    short workCyclesNumber;          /*!< 工作周期数  默认为1 */
    char workingBandwidth;           /*!< 工作带宽  0:400M  1:200M  2：40M  3:10M  4：5M */
    char antennaSelection1;          /*!< 天线选择1  0前  1后  2左  3右 */
    char antennaSelection2;          /*!< 天线选择2  0前  1后  2左  3右 */
    float attenuationCode1;          /*!< 衰减码1(dB) */
    float attenuationCode2;          /*!< 衰减码2(dB) */
    float attenuationCode3;          /*!< 衰减码3(dB) */
    float attenuationCode4;          /*!< 衰减码4(dB) */
    float attenuationCode5;          /*!< 衰减码5(dB) */
    float attenuationCode6;          /*!< 衰减码6(dB) */
    float attenuationCode7;          /*!< 衰减码7(dB) */
    float attenuationCode8;          /*!< 衰减码8(dB) */
    float attenuationCode9;          /*!< 衰减码9(dB) */
    float attenuationCode10;         /*!< 衰减码10(dB) */

    friend QDataStream & operator<<(QDataStream & stream,const StateControl & info);
    friend QDataStream & operator>>(QDataStream & stream,StateControl & info);

    void dispatch(){}
};

/*!
 *  @brief 采集控制
 */
struct GatherControl  : public TaskInfo
{
    GatherControl():TaskInfo()
    {
        taskType = Gather;
        extractSwitch = false;
    }
    GatherControl(const GatherControl & info):TaskInfo(info){
        this->gatherWay = info.gatherWay;
        this->gatherType = info.gatherType;
        this->sendingPause = info.sendingPause;
        this->extractSwitch = info.extractSwitch;
        this->gatherTime = info.gatherTime;
        this->gatherImpulseNumber = info.gatherImpulseNumber;
        this->gatherFrequencyMin = info.gatherFrequencyMin;
        this->gatherFrequencyMax = info.gatherFrequencyMax;
        this->gatherPulseyMin = info.gatherPulseyMin;
        this->gatherPulseyMax = info.gatherPulseyMax;
        this->gatherPowerMin = info.gatherPowerMin;
        this->gatherPowerMax = info.gatherPowerMax;
        this->gatherTacticsSwitch = info.gatherTacticsSwitch;
    }
    ~GatherControl(){}

    char gatherWay;                 /*!< 采集方式  0：DDR  1:SATA */
    char gatherType;                /*!< 采集类型  0：盲采  1:Vp采  2：Vp触发盲采 */
    char sendingPause;              /*!< 采集指令(只对SATA有效 0停止 1开始采集 2开始读取 3检索 4读取暂停 5发送暂停 其他：无效 */
    bool extractSwitch;             /*!< 抽取开关  0：不抽取(默认)  1：抽取 */
    float gatherTime;               /*!< 采集时长(ms) */
    int gatherImpulseNumber;        /*!< 采集脉冲个数  MAX:1000000 */
    float gatherFrequencyMin;       /*!< 采集频率下限(MHz) */
    float gatherFrequencyMax;       /*!< 采集频率上限(MHz) */
    float gatherPulseyMin;          /*!< 采集脉宽下限(us) */
    float gatherPulseyMax;          /*!< 采集脉宽上限(us) */
    float gatherPowerMin;           /*!< 采集功率下限(dBm) */
    float gatherPowerMax;           /*!< 采集功率上限(dBm) */
    char gatherTacticsSwitch;       /*!< 采集策略开关 Bit0:0时长无效，1时长有效(默认1)
                                                      Bit1:0个数无效，1个数有效(默认1)
                                                      Bit2:0频率无效，1频率有效(默认0)
                                                      Bit3:0脉宽无效，1脉宽有效(默认0)
                                                      Bit4:0功率无效，1功率有效(默认0) */

    friend QDataStream & operator<<(QDataStream & stream,const GatherControl & info);
    friend QDataStream & operator>>(QDataStream & stream,GatherControl & info);

    void dispatch(){}
};

/*!
 *  @brief 自检控制
 */
struct SelfCheckControl  : public TaskInfo
{
    SelfCheckControl():TaskInfo()
    {
        taskType = SelfCheck;
        workCyclesNumber = 1;
    }
    SelfCheckControl(const SelfCheckControl & info):TaskInfo(info){
        this->workmode = info.workmode;
        this->workCycles = info.workCycles;
        this->workCyclesNumber = info.workCyclesNumber;
        this->workingBandwidth = info.workingBandwidth;
        this->originFrequency = info.originFrequency;
        this->stopFrequency = info.stopFrequency;
        this->frequencyStopping = info.frequencyStopping;
        this->attenuationCode1 = info.attenuationCode1;
        this->attenuationCode2 = info.attenuationCode2;
        this->attenuationCode3 = info.attenuationCode3;
        this->attenuationCode4 = info.attenuationCode4;
        this->attenuationCode5 = info.attenuationCode5;
        this->attenuationCode6 = info.attenuationCode6;
        this->attenuationCode7 = info.attenuationCode7;
        this->attenuationCode8 = info.attenuationCode8;
        this->attenuationCode9 = info.attenuationCode9;
        this->attenuationCode10 = info.attenuationCode10;
    }
    ~SelfCheckControl(){}

    char workmode;                   /*!< 工作模式 0自检  1工作 */
    char workCycles;                 /*!< 工作周期(50ms)   最大1000ms (20) */
    short workCyclesNumber;          /*!< 工作周期数  默认为1 */
    char workingBandwidth;           /*!< 工作带宽  0:400M  1:200M  2：40M  3:10M  4：5M */
    double originFrequency;          /*!< 起始频率(MHz) */
    double stopFrequency;            /*!< 终止频率(MHz) */
    double frequencyStopping;        /*!< 频率步进(MHz) */
    float attenuationCode1;          /*!< 衰减码1(dB) */
    float attenuationCode2;          /*!< 衰减码2(dB) */
    float attenuationCode3;          /*!< 衰减码3(dB) */
    float attenuationCode4;          /*!< 衰减码4(dB) */
    float attenuationCode5;          /*!< 衰减码5(dB) */
    float attenuationCode6;          /*!< 衰减码6(dB) */
    float attenuationCode7;          /*!< 衰减码7(dB) */
    float attenuationCode8;          /*!< 衰减码8(dB) */
    float attenuationCode9;          /*!< 衰减码9(dB) */
    float attenuationCode10;         /*!< 衰减码10(dB) */

    friend QDataStream & operator<<(QDataStream & stream,const SelfCheckControl & info);
    friend QDataStream & operator>>(QDataStream & stream,SelfCheckControl & info);

    void dispatch(){}
};

/*!
 *  @brief 仪器控制
 */
struct InstrumentControl  : public TaskInfo
{
    InstrumentControl():TaskInfo()
    {
        taskType = Instrument;
    }
    InstrumentControl(const InstrumentControl & info):TaskInfo(info){
        this->instrumentType = info.instrumentType;
        this->instrumentModel = info.instrumentModel;
        this->instrumentCommType = info.instrumentCommType;
        memcpy(this->IPAddress,info.IPAddress,64);
        this->networkPort = info.networkPort;
        this->GPIBPort = info.GPIBPort;
        this->signalCarrierFrequency = info.signalCarrierFrequency;
        this->signalRepetitionPeriod = info.signalRepetitionPeriod;
        this->signalPulseWidth = info.signalPulseWidth;
        this->signalPower = info.signalPower;
        this->radioFrequencySwitchControl = info.radioFrequencySwitchControl;
        this->intrapulseSwitchControl = info.intrapulseSwitchControl;
        this->ImpulseSwitchControl = info.ImpulseSwitchControl;
        this->centreFrequency = info.centreFrequency;
        this->displayBandwidth = info.displayBandwidth;
        this->IFBandwidth = info.IFBandwidth;
        this->videoBandwidth = info.videoBandwidth;
        this->scanTime = info.scanTime;
        this->scanPoints = info.scanPoints;
        this->ifReadspectrumData = info.ifReadspectrumData;
        this->signalBase = info.signalBase;
        this->powerCalibrationControl = info.powerCalibrationControl;
        this->immediatePowerCalibration = info.immediatePowerCalibration;
    }
    ~InstrumentControl(){}

    short instrumentType;            /*!< 仪器类型  0：信号源  1：频谱仪  2：以上待定 */
    short instrumentModel;           /*!< 仪器型号  0：安捷伦  2：安立  2罗德斯瓦兹  3以上待定 */
    short instrumentCommType;        /*!< 仪器通信类型   0：网络  1：GPIB */
    char IPAddress[64];              /*!< 仪器IP地址  字符串 */
    short networkPort;               /*!< 网络端口 */
    short GPIBPort;                  /*!< GPIB端口 */
    double signalCarrierFrequency;   /*!< 信号载频(MHz) */
    double signalRepetitionPeriod;   /*!< 信号重复周期(us) */
    double signalPulseWidth;         /*!< 信号脉宽(us) */
    double signalPower;              /*!< 信号功率(dBm) */
    int radioFrequencySwitchControl; /*!< 射频开关控制  0关  1开 */
    int intrapulseSwitchControl;     /*!< 脉内调制开关控制  0关  1开 */
    int ImpulseSwitchControl;        /*!< 脉冲开关控制  0关  1开 */
    double centreFrequency;          /*!< 中心频率(Mhz) */
    double displayBandwidth;         /*!< 显示带宽(Mhz) */
    double IFBandwidth;              /*!< 中频带宽(Mhz) */
    double videoBandwidth;           /*!< 视频带宽(Mhz) */
    int scanTime;                    /*!< 扫描时间(ms) */
    int scanPoints;                  /*!< 扫描点数 */
    int ifReadspectrumData;          /*!< 是否读取频谱数据  0否  1是 */
    double signalBase;               /*!< 信号基底(dBm) */
    int powerCalibrationControl;     /*!< 功率校准控制  0：关  1：开 */
    int immediatePowerCalibration;   /*!< 功率立即校准  0：关  1：开 */

    friend QDataStream & operator<<(QDataStream & stream,const InstrumentControl & info);
    friend QDataStream & operator>>(QDataStream & stream,InstrumentControl & info);

    void dispatch(){}
};

/*!
 *  @brief 转台控制
 */
struct TurntableControl  : public TaskInfo
{
    TurntableControl():TaskInfo()
    {
        taskType = Turntable;
    }
    TurntableControl(const TurntableControl & info):TaskInfo(info){
        this->type = info.type;
        this->position = info.position;
        this->speed = info.speed;
        this->directionRotation = info.directionRotation;
        this->zeroPosSetting = info.zeroPosSetting;
    }
    ~TurntableControl(){}

    short type;                 /*!< 转台类型  0：类型1  1：类型2... */
    double position;            /*!< 转台位置(度_0-360) */
    double speed;               /*!< 转台速度(度/秒) */
    short directionRotation;    /*!< 转台旋转方向 0顺时针  1逆时针 */
    short zeroPosSetting;       /*!< 转台零位设置  0不设置  1设置 */

    friend QDataStream & operator<<(QDataStream & stream,const TurntableControl & info);
    friend QDataStream & operator>>(QDataStream & stream,TurntableControl & info);

    void dispatch(){}
};

/*!
 *  @brief 回放控制
 */
struct PlayBackControl  : public TaskInfo
{
    PlayBackControl():TaskInfo()
    {
        taskType = PlayBack;
    }
    PlayBackControl(const PlayBackControl & info):TaskInfo(info){
        memcpy(this->path,info.path,256);
        this->type = info.type;
        this->originPos = info.originPos;
        this->speed = info.speed;
    }
    ~PlayBackControl(){}

    char path[256];             /*!< 回放文件路径 */
    int type;                   /*!< 回放类型  0辐射源 1全脉冲 2中频采集 3频谱 4设备状态 */
    double originPos;           /*!< 回放起始位置 0~1，表示相对整个文件的比例 */
    int speed;                  /*!< 回放速度(帧/分) */

    friend QDataStream & operator<<(QDataStream & stream,const PlayBackControl & info);
    friend QDataStream & operator>>(QDataStream & stream,PlayBackControl & info);

    void dispatch(){}
};

/**
 * @brief 存储下发任务
 */
struct pendingTaskState
{
    bool ifExists;              /*!< 是否加入下发队列 */
    bool issued;                /*!< 是否已经下发 */
    Type type;                  /*!< 下发任务类型 */
    QString ExcutionTime;       /*!< 下发时间 */
};

/***********任务下发配置信息*********/
struct TaskConfigInfo{
    QString hostIp;
    ushort hostPort;
};


} // namespace TaskControlModel

#endif // TASK_CONTROL_HEAD_H
