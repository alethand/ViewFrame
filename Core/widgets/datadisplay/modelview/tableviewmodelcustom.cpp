//#include "tableviewmodelcustom.h"
//#include <QDebug>
//#include "Base/util/rsingleton.h"

//namespace DataView {
//const int INIT_TABLE_CUSTOM_ROW=40;
//const int INIT_TABLE_ALLPULSE_ROW=18;  //全脉冲初始表格
//TableViewModelCustom::TableViewModelCustom(QObject *parent)
//    :QAbstractTableModel(parent),
//    tableCustomKind(STATISTICAL_INFO)
//{
//    tableTotalRows=INIT_TABLE_CUSTOM_ROW;
//    retranslateUi();
//    RSingleton<Base::Subject>::instance()->attach(this);
//}

//TableViewModelCustom::~TableViewModelCustom()
//{

//}


//int TableViewModelCustom::rowCount(const QModelIndex &index) const
//{
//    return tableTotalRows;
//}

//int TableViewModelCustom::columnCount(const QModelIndex &index) const
//{
//    return headInfo.size();
//}

//bool TableViewModelCustom::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//    if (!index.isValid())
//        return false;
//    return true;
//}

//QVariant TableViewModelCustom::data(const QModelIndex &index, int role) const
//{
//    if (!index.isValid())
//        return QVariant();

//    int row = index.row();
//    int col=index.column();
//    switch(role)
//    {
//    case Qt::TextAlignmentRole:
//        return int(Qt::AlignHCenter | Qt::AlignVCenter);
//        break;
//    case Qt::DisplayRole:
//    {
//        if(col==0)
//        {
//            return QString("%1").arg(row+1);
//            break;
//        }
//        if(tableCustomKind==MF_ACQUISITION_INFO)    //中频数据模式
//        {
//            bool blRowEff=false;                    //该行有效标识
//            MidFreqInfo::Core mfInfo;               //中频信息
//            if(mfAcquistionInfoList.size()>row)
//            {
//                blRowEff=true;
//                mfInfo=mfAcquistionInfoList.at(row);
//            }
//            if(blRowEff)
//            {
//                switch (static_cast<MidFreqInfo::MFAcquisitionHead>(col)) {
//                case MidFreqInfo::T_AcqTime:                 /*!< 采集时间*/
//                {
//                    return mfInfo.acqTime;
//                    break;
//                }
//                case MidFreqInfo::T_AcqModel:                 /*!< 采集模式 0xCF01:VP触发采,0xCF02:盲采,0xCF00:中频数据不采集*/
//                {
//                    QString strValue;
//                   switch(mfInfo.acqMode)
//                    {
//                   case MidFreqInfo::Collect::VP_Trigger:
//                       strValue=QStringLiteral("VP触发采");
//                       break;
//                    case MidFreqInfo::Collect::Blind:
//                         strValue=QStringLiteral("盲采");
//                       break;
//                    case MidFreqInfo::Collect::NoCollect:
//                        strValue=QStringLiteral("中频数据不采集");
//                       break;
//                    }

//                    return strValue;
//                    break;
//                }
//                case MidFreqInfo::T_AcqNum:                  /*!< 脉冲采集个数*/
//                {
//                    return mfInfo.iAcqNum;
//                    break;
//                }
//                case MidFreqInfo::T_AcqDotNum:                 /*!< 采集点数*/
//                {
//                    return mfInfo.iAcqDotNum;
//                    break;
//                }
//                default:
//                    break;
//                }
//            }
//        }
//        else if(tableCustomKind==STATISTICAL_INFO)  //统计信息模式
//        {
//            bool blRowEff=false;                    //该行有效标识
////            AllPluseStatisticInfo statisticInfo;    //统计信息
////            if(allPluseStatisticInfoList.size()>row)
////            {
////                blRowEff=true;
////                statisticInfo=allPluseStatisticInfoList.at(row);
////            }

//           /* if(blRowEff)
//            {
//                switch (static_cast<AllPluseStatisticInfoHead>(col)) {
//                case T_StaInfoName:                 /*!< 统计参数名称*/
//            /*    {
//                    QString strValue=QString(QLatin1String(statisticInfo.allPluseStatisticInfoBase.arrStaInfoName));
//                    return strValue;
//                    break;
//                }
//                case T_Min:
//                {
//                    return statisticInfo.allPluseStatisticInfoBase.dMin;
//                    break;
//                }
//                case T_Max:
//                {
//                    return statisticInfo.allPluseStatisticInfoBase.dMax;
//                    break;
//                }
//                case T_Ave:
//                {
//                    return statisticInfo.allPluseStatisticInfoBase.dAve;
//                    break;
//                }
//                case T_Std:
//                {
//                    return statisticInfo.allPluseStatisticInfoBase.dStd;
//                    break;
//                }
//                default:
//                    break;
//                }
//            }*/
//        }
//        else if(tableCustomKind==ORIGINAL_INFO)     //原始数据模式
//        {
//            bool blRowEff=false;        //该行有效标识
//            AllPluseOriginalInfoAttributeBase originalInfoBase; //原始信息

//            if(allPulseOriginalInfoList.size()>row)
//            {
//                blRowEff=true;
//                originalInfoBase=allPulseOriginalInfoList.at(row).allPluseOriginalInfoBase;
//            }

//            if(blRowEff)
//            {
//                switch (static_cast<AllPluseOriginalInfoAttributeHead>(col)) {
//                case T_OrgInfoAttributeName:                 /*!< 原始参数名称*/
//                {
//                    QString strValue=QString(QLatin1String(originalInfoBase.arrOrgInfoAttributeName));
//                    return strValue;
//                    break;
//                }
//                case T_DrawFlag:                            /*!< 属性是否绘图*/
//                {
//                    return originalInfoBase.iDrawFlag;
//                    break;
//                }
//                case T_Value:                               /*!< 属性值*/
//                {
//                    return originalInfoBase.dValue;
//                    break;
//                }
//                default:
//                    break;
//                }
//            }
//        }
//    }
//        break;
//        case Qt::BackgroundColorRole:
//        {

//        }
//        break;
//        default:
//            break;
//    }

//    return QVariant();
//}

//QVariant TableViewModelCustom::headerData(int section, Qt::Orientation orientation,int role) const
//{
//    if (role != Qt::DisplayRole)
//        return QVariant();

//    if (Qt::Horizontal == orientation && section < headInfo.size()){
//        return headInfo.at(section);
//    }

//    return QVariant();
//}

//Qt::ItemFlags TableViewModelCustom::flags(const QModelIndex &index) const
//{
//    if (!index.isValid())
//        return 0;

//    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
//}

//void TableViewModelCustom::onMessage(MessageType::MessType type)
//{
//    switch(type){
//        case MessageType::MESS_LAN_CHANGED:
//                retranslateUi();
//                resetData();
//        break;
//        default:
//            break;
//    }
//}

//void TableViewModelCustom::resetData()
//{
//    beginResetModel();
//    endResetModel();
//}

//void TableViewModelCustom::retranslateUi()
//{
//    QStringList headInfoTemp=headInfo;
//    headInfo.clear();
//    headInfo=headInfoTemp;
//}

///*!
// * \brief  更新表头信息
// * \param head
// */
//void TableViewModelCustom::resetHeadInfo(const QStringList& head)
//{
//    headInfo=head;
//    resetData();
//    retranslateUi();
//}

///*!
// * \brief 设置表格显示类型,一个model要显示多张不同表,设置默认行数
// * \param kind
// */
//void TableViewModelCustom::setTableCustomKind(TableCustomKind kind)
//{
//    tableCustomKind=kind;

//    if(tableCustomKind==MF_ACQUISITION_INFO)    //中频数据
//    {

//    }
//    else if(tableCustomKind==STATISTICAL_INFO)  //统计信息
//    {
//        tableTotalRows=INIT_TABLE_ALLPULSE_ROW;
//    }
//    else if(tableCustomKind==ORIGINAL_INFO)     //原始数据
//    {
//        tableTotalRows=INIT_TABLE_ALLPULSE_ROW;
//    }
//}

/////*!
//// * \brief 设置全脉冲统计信息
//// * \param list 统计信息
//// */
////void TableViewModelCustom::updateAllPluseStatisticInfoList(const AllPluseStatisticInfoList& list)
////{
////    if(tableCustomKind==STATISTICAL_INFO)  //统计信息模式有效
////    {
////        allPluseStatisticInfoList=list;
////        if(allPluseStatisticInfoList.size()>INIT_TABLE_ALLPULSE_ROW)
////        {
////            tableTotalRows=allPluseStatisticInfoList.size();
////        }
////        resetData();
////        retranslateUi();
////    }
////}

///*!
// * \brief 设置全脉冲原始属性信息
// * \param list 属性
// */
//void TableViewModelCustom::updateAllPulseOriginalInfoList(const AllPulseOriginalInfoList& list)
//{
//    if(tableCustomKind==ORIGINAL_INFO)     //原始数据模式有效
//    {
//        allPulseOriginalInfoList=list;
//        if(allPulseOriginalInfoList.size()>INIT_TABLE_ALLPULSE_ROW)
//        {
//            tableTotalRows=allPulseOriginalInfoList.size();
//        }
//        resetData();
//        retranslateUi();
//    }
//}

///*!
// * \brief 设置中频数据表格信息
// * \param list 中频数据
// */
//void TableViewModelCustom::updateMFAcquistionInfoList(const MFAcquistionInfoList& list)
//{
//    if(tableCustomKind==MF_ACQUISITION_INFO)     //中频数据模式
//    {
//        mfAcquistionInfoList=list;
//        if(mfAcquistionInfoList.size()>INIT_TABLE_CUSTOM_ROW)
//        {
//            tableTotalRows=mfAcquistionInfoList.size();
//        }
//        resetData();
//        retranslateUi();
//    }
//}

//} //namespace DataView
