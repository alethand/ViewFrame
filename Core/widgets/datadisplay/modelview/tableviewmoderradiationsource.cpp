#include "tableviewmoderradiationsource.h"
#include <QDebug>
#include "Base/util/rsingleton.h"
#include <QColor>

namespace DataView {
const int INIT_TABLE_RADIASOURCE_ROW=40;

TableViewModelRadiationSource::TableViewModelRadiationSource(QObject *parent):
    QAbstractTableModel(parent),
    refreshModel(SCROLL_RENOVATE),
    hightLightRow(-1)
{
    tableTotalRows=INIT_TABLE_RADIASOURCE_ROW;
    rsDataList.clear();
    retranslateUi();
    RSingleton<Base::Subject>::instance()->attach(this);
}

TableViewModelRadiationSource::~TableViewModelRadiationSource()
{

}

int TableViewModelRadiationSource::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return tableTotalRows;
}

int TableViewModelRadiationSource::columnCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return headInfo.size();
}

bool TableViewModelRadiationSource::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(role)
    Q_UNUSED(value)
    if (!index.isValid())
        return false;
    return true;
}

QVariant TableViewModelRadiationSource::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col=index.column();
    switch(role)
    {
        case Qt::TextAlignmentRole:
            return int(Qt::AlignHCenter | Qt::AlignVCenter);
        break;

        case Qt::DisplayRole:
            {
                bool blRowEff=false;        //该行有效标识
                RadiationSource::RadiationSourceBase rsInfo; //数据源数据
                RadiationSourceRenovate rsRe;
                if(refreshModel==SCROLL_RENOVATE) //滚动刷新
                {
                    if(rsDataList.size()>row)
                    {
                        blRowEff=true;
                        rsInfo=rsDataList.at(row).rsInfo;
                        rsRe=rsDataList.at(row);
                    }
                }
                else if(refreshModel==COVER_RENOVATE)
                {
                    if(rsDataMap.size()>row)
                    {
                        QMap<int,RadiationSourceRenovate>::const_iterator iter;
                        for(iter=rsDataMap.begin();iter!=rsDataMap.end();iter++)
                        {
                            rsRe=iter.value();
                            if(rsRe.iInsertRow==row)
                            {
                                blRowEff=true;
                                rsInfo=rsRe.rsInfo;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    return QVariant();
                }

                switch (static_cast<RadiationSourceHead>(col)) {
                case T_No:
                {
                    return QString("%1").arg(row+1);
                    break;
                }
                case T_SourceNo:    /*!< 辐射源序号,递增*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.usSourceNo;
                    }
                    break;
                }
                case T_PulseType:   /*!< 载频,脉间类型,0 固定,1 脉间捷变,2 脉组捷变,3 分时分集,4 连续波,5 双频点,0xff:未知*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.ucPulseType;
                    }
                    break;
                }
                case T_IntraPulseType:  /*!< 载频,脉内类型,0 单载频 1 多载频 2 调频 0xff 未知*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.ucIntraPulseType;
                    }
                    break;
                }
                case T_CarrierCount:    /*!< 载频,个数*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.ucCarrierCount;
                    }
                    break;
                }
                case T_ContinuousWaveLabeling:     /*!< 载频,连续波标记,0 无效 1 有效*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.ucContinuousWaveLabeling;
                    }
                    break;
                }
                case T_PulseGroupInCount:         /*!< 载频,脉组内脉冲数*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.usPulseGroupInCount;
                    }
                    break;
                }
                case T_FrequencyBandCode:          /*!< 载频,频段码 T_FrequencyBandCode */
                {
                    if(blRowEff)
                    {
                        return rsInfo.usFrequencyBandCode;
                    }
                    break;
                }
                case T_RF1:                        /*!< 载频,1*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dRF1;
                    }
                    break;
                }
                case T_RF2:                        /*!< 载频,2*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dRF2;
                    }
                    break;
                }
                case T_RF3:                        /*!< 载频,3*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dRF3;
                    }
                    break;
                }
                case T_RF4:                        /*!< 载频,4*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dRF4;
                    }
                    break;
                }
                case T_RF5:                        /*!< 载频,5*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dRF5;
                    }
                    break;
                }
                case T_RF6:                        /*!< 载频,6*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dRF6;
                    }
                    break;
                }
                case T_RF7:                        /*!< 载频,7*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dRF7;
                    }
                    break;
                }
                case T_RF8:                        /*!< 载频,8,单位,MHz*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dRF8;
                    }
                    break;
                }
                case T_MultiPluseType:            /*!< 重频,类型,0 固定,1 抖动,2 滑变,3 成组参差,4 固定参差*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.usMultiPluseType;
                    }
                    break;
                }
                case T_MultiPluseCount:           /*!< 重频,个数*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.usMultiPluseCount;
                    }
                    break;
                }
                case T_MultiPulseGroupInCount:      /*!< 重频,脉组内脉冲数*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.ulMultiPulseGroupInCount;
                    }
                    break;
                }
                case T_PRI1:                       /*!< 重频,周期1*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPRI1;
                    }
                    break;
                }
                case T_PRI2:                       /*!< 重频,周期2*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPRI2;
                    }
                    break;
                }
                case T_PRI3:                       /*!< 重频,周期3*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPRI3;
                    }
                    break;
                }
                case T_PRI4:                       /*!< 重频,周期4*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPRI4;
                    }
                    break;
                }
                case T_PRI5:                       /*!< 重频,周期5*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPRI5;
                    }
                    break;
                }
                case T_PRI6:                       /*!< 重频,周期6*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPRI6;
                    }
                    break;
                }
                case T_PRI7:                       /*!< 重频,周期7*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPRI7;
                    }
                    break;
                }
                case T_PRI8:                       /*!< 重频,周期8,单位:ns*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPRI8;
                    }
                    break;
                }
                case T_PluseWidthType:            /*!< 脉宽,类型,0 固定,1 变化,0xff 未知*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.usPluseWidthType;
                    }
                    break;
                }
                case T_PluseWidthCount:           /*!< 脉宽,个数,≤8*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.usPluseWidthCount;
                    }
                    break;
                }
                case T_PluseWidthGroupInCount:      /*!< 脉宽,脉组内脉冲数*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.ulPluseWidthGroupInCount;
                    }
                    break;
                }
                case T_PW1:                        /*!< 脉宽,1*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPW1;
                    }
                    break;
                }
                case T_PW2:                        /*!< 脉宽,2*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPW2;
                    }
                    break;
                }
                case T_PW3:                        /*!< 脉宽,3*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPW3;
                    }
                    break;
                }
                case T_PW4:                        /*!< 脉宽,4*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPW4;
                    }
                    break;
                }
                case T_PW5:                        /*!< 脉宽,5*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPW5;
                    }
                    break;
                }
                case T_PW6:                        /*!< 脉宽,6*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPW6;
                    }
                    break;
                }
                case T_PW7:                        /*!< 脉宽,7 T_PW7*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPW7;
                    }
                    break;
                }
                case T_PW8:                        /*!< 脉宽,8,单位:ns T_PW8*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dPW8;
                    }
                    break;
                }
                case T_DigitalPA:                  /*!< 数字幅度,0-255*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.ucDigitalPA;
                    }
                    break;
                }
                case T_AnalogPA:                   /*!< 模拟幅度,0-255*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.ucAnalogPA;
                    }
                    break;
                }
                case T_DigitalPower:               /*!< 数字功率,单位:dBm,大于9999无效*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dDigitalPower;
                    }
                    break;
                }
                case T_AnalogPower:                /*!< 模拟功率,单位:dBm,大于9999无效*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dAnalogPower;
                    }
                    break;
                }
                case T_AziAngle:                   /*!< 测量信息,方位角 T_AziAngle*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dAziAngle;
                    }
                    break;
                }
                case T_EleAngle:                   /*!< 测量信息,俯仰角 T_EleAngle*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dEleAngle;
                    }
                    break;
                }
                case T_Lon:                        /*!< 定位结果,经度*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dLon;
                    }
                    break;
                }
                case T_Lat:                        /*!< 定位结果,纬度*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dLat;
                    }
                    break;
                }
                case T_Hight:                      /*!< 定位结果，高度 T_Hight*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.dHight;
                    }
                    break;
                }
                case T_ntraPulseEffFlag:           /*!< 脉内调制信息,脉内有效标识,1 有效,0 无效 T_ntraPulseEffFlag*/
                {
                    if(blRowEff)
                    {
                        return rsInfo.ulIntraPulseEffFlag;
                    }
                    break;
                }
                case T_IntraPulseInfo:              /*!< 脉内调制信息,脉内特征信息 T_IntraPulseInfo*/
                {
                    if(blRowEff)
                    {
                        char *pBuff=new char[32];
                        memcpy(pBuff,rsInfo.ucIntraPulseInfo,32);
                        QString strValue=QString(QLatin1String(pBuff));
                        return strValue;
                    }
                    break;
                }
                case T_CRC:                       /*!< CRC校验 T_CRC */
                {
                    if(blRowEff)
                    {
                        return rsInfo.usCRC;
                    }
                    break;
                }
                case T_Counter:                       /*!< 次数 T_Counter */
                {
                    if(blRowEff)
                    {
                        return rsRe.iHoldCount;
                    }
                    break;
                }
                case T_Timer:                       /*!< 时间 T_Timer */
                {
                    if(blRowEff)
                    {
                        return rsRe.strHoldTime;
                    }
                    break;
                }
                default:
                    break;
                }
            }
        break;
        case Qt::BackgroundColorRole:
        {
            if(refreshModel==COVER_RENOVATE)        //覆盖刷新模式下高亮最新数据
            {
                if(hightLightRow==index.row())
                {
                    return QColor(Qt::green);
                }
            }
        }
        break;
        default:
            break;
    }

    return QVariant();
}

QVariant TableViewModelRadiationSource::headerData(int section, Qt::Orientation orientation,int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (Qt::Horizontal == orientation && section < headInfo.size()){
        return headInfo.at(section);
    }

    return QVariant();
}

Qt::ItemFlags TableViewModelRadiationSource::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

//    if(index.column()>=0) {

//       return Qt::ItemIsEditable | QAbstractTableModel::flags(index);
//    }
    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

void TableViewModelRadiationSource::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
                retranslateUi();
                resetData();
        break;
        default:
            break;
    }
}

void TableViewModelRadiationSource::resetData()
{
    beginResetModel();
    endResetModel();
}

void TableViewModelRadiationSource::retranslateUi()
{
    QStringList headInfoTemp=headInfo;
    headInfo.clear();
    headInfo=headInfoTemp;
}

/*!
 * \brief TableViewModelRadiationSource::resetHeadInfo 更新表头信息
 * \param head
 */
void TableViewModelRadiationSource::resetHeadInfo(const QStringList& head)
{
    headInfo=head;
    resetData();
    retranslateUi();
}

/*!
 * \brief TableViewModelRadiationSource::resetRefreshModel 设置刷新模式
 * \param model 刷新模式
 */
void TableViewModelRadiationSource::resetRefreshModel(const DataRefreshModel &model)
{
    refreshModel=model;
}

/*!
 * \brief TableViewModelRadiationSource::resetHigetLightRow 设置高亮行
 * \param row 高亮行
 */
void TableViewModelRadiationSource::resetHigetLightRow(int row)
{
   hightLightRow=row;
}

/*!
 * \brief TableViewModelRadiationSource::updateTableData 更新数据,滚动刷新下使用
 * \param rsList 数据列表
 * \param isDialog 是否弹框模式
 */
void TableViewModelRadiationSource::updateTableData(const RSDataList &rsList,const bool isDialog)
{
    rsDataList=rsList;

    if(!isDialog)
    {
        if(refreshModel==SCROLL_RENOVATE) //滚动刷新
        {
            if(rsDataList.size()>=tableTotalRows)
            {
                tableTotalRows=rsDataList.size();
            }
            else
            {
                if(rsDataList.size()<INIT_TABLE_RADIASOURCE_ROW)
                {
                    tableTotalRows=INIT_TABLE_RADIASOURCE_ROW;
                }
            }
        }
        else if(refreshModel==COVER_RENOVATE)
        {
            if(rsDataMap.size()>tableTotalRows)
            {
                tableTotalRows=rsDataMap.size();
            }
            else
            {
                if(rsDataMap.size()<INIT_TABLE_RADIASOURCE_ROW)
                {
                    tableTotalRows=INIT_TABLE_RADIASOURCE_ROW;
                }
            }
        }
    }
    else
    {
        tableTotalRows=rsDataList.size();
    }

    resetData();
    retranslateUi();
}

/*!
 * \brief TableViewModelRadiationSource::updateTableData 更新数据,覆盖刷新下使用
 * \param rsMap
 */
void TableViewModelRadiationSource::updateTableData(const RSDataMap& rsMap)
{
    rsDataMap=rsMap;

    if(refreshModel==SCROLL_RENOVATE) //滚动刷新
    {
        if(rsDataList.size()>=tableTotalRows)
        {
            tableTotalRows=rsDataList.size();
        }
        else
        {
            if(rsDataList.size()<INIT_TABLE_RADIASOURCE_ROW)
            {
                tableTotalRows=INIT_TABLE_RADIASOURCE_ROW;
            }
        }
    }
    else if(refreshModel==COVER_RENOVATE)
    {
        if(rsDataMap.size()>tableTotalRows)
        {
            tableTotalRows=rsDataMap.size();
        }
        else
        {
            if(rsDataMap.size()<INIT_TABLE_RADIASOURCE_ROW)
            {
                tableTotalRows=INIT_TABLE_RADIASOURCE_ROW;
            }
        }
    }
    resetData();
    retranslateUi();
}

}
