#include "staticlayoutbyxml.h"

StaticLayoutByXml::StaticLayoutByXml(QWidget *parent):LayoutAndDisplay(parent)
{
    mNetXmlResult = XNCRCompleteSame;
    mxmlData =  new ParseXML_HealthStateLayout();
}

StaticLayoutByXml::~StaticLayoutByXml()
{
    delete mxmlData;
    mMapWorkState.clear();
    mMapSubMachine.clear();
    mMapComPonet.clear();
}

bool StaticLayoutByXml::startLayout(HealthData &data,bool ifUse)
{
    Q_UNUSED(data)
    Q_UNUSED(ifUse)

    if(!isHasLayout)
    {
        isHasLayout = true;
        if(!readXMLFile())
            return false;

        ensureLayoutData(*(mxmlData->mHealthData));         //解析data数据,确定数据量
        mUpdatePara.reset(mxmlData->mHealthData);

        generateWSLayout();         //生成工作状态布局
        generateSubMCpLayout();     //生成分机-组件布局
    }
    return true;
}

void StaticLayoutByXml::flushScreen(HealthData &data)
{
    static bool firstCompare = true;
    if(firstCompare)
    {
        compareStringOrder(data);//比较网络传来的数据布局与xml文件中的布局内容
    }

    if(mNetXmlResult != XNCRDataContentError)
    {
        LayoutAndDisplay::flushScreen(data);
    }
}



bool StaticLayoutByXml::readXMLFile()
{
    XmlFileOpt::getOne()->setParseMethod(mxmlData);
    return XmlFileOpt::getOne()->readXmLsFile("test.xml");
}

void StaticLayoutByXml::compareStringOrder(HealthData &data)
{
     //工作状态的比较
     {
       if(data.workStateNums != data.wsInfo.count()||data.workStateNums !=  mxmlData->mHealthData->workStateNums)
       {
           mNetXmlResult = XNCRDataContentError;
           return;
       }
       for(unsigned long i=0;i < mxmlData->mHealthData->workStateNums;i++)
       {
           if(data.wsInfo.at(i).name != mxmlData->mHealthData->wsInfo.at(i).name)
           {
               unsigned long j=0;
               for(;j < mxmlData->mHealthData->workStateNums;j++)
               {
                   if(data.wsInfo.at(j).name == mxmlData->mHealthData->wsInfo.at(i).name)
                   {
                       mMapWorkState.insert(j,i);
                       mNetXmlResult = XNCROnlyWorkStateDif;
                       break;
                   }
               }
               if(j == mxmlData->mHealthData->workStateNums)
               {
                    mNetXmlResult = XNCRDataContentError;
                   return;
               }
           }
       }

    }

    //分机的比较
    {
        if(data.subMachineNums!= data.smInfo.count()||data.subMachineNums !=  mxmlData->mHealthData->subMachineNums)
        {
            mNetXmlResult = XNCRDataContentError;
            return;
        }
        for(unsigned long i=0;i < mxmlData->mHealthData->subMachineNums;i++)
        {
            if(data.smInfo.at(i).name != mxmlData->mHealthData->smInfo.at(i).name)
            {
                unsigned long j=0;
                for(;j < mxmlData->mHealthData->subMachineNums;j++)
                {
                    if(data.smInfo.at(j).name == mxmlData->mHealthData->smInfo.at(i).name)
                    {
                        mMapSubMachine.insert(j,i);
                        if(XNCRCompleteSame == mNetXmlResult)
                            mNetXmlResult = XNCROnlyWorkStateDif;
                        else
                            mNetXmlResult = XNCRNeedMAp;
                        break;
                    }
                }
                if(j == mxmlData->mHealthData->subMachineNums)
                {
                     mNetXmlResult = XNCRDataContentError;
                    return;
                }
            }
        }
    }

    //组件的比较
    {
        QMap<int,int>::iterator iteSubM;
        for(unsigned long i=0;i < mxmlData->mHealthData->subMachineNums;i++)
        {
            int trueSubMIndex=i;                                ///索引到对应的组件信息
             if(mMapSubMachine.count())
            {////需要使用分机的映射
                 iteSubM = mMapSubMachine.find(i);
                if(mMapSubMachine.end() != iteSubM) //该分机的顺序是乱的
                {
                    trueSubMIndex = iteSubM.value();
                }
            }

             if(data.smInfo.at(i).componetNum!= data.smInfo.at(i).compInfo.count()          ///确定组件的个数信息无误
                     ||data.smInfo.at(i).componetNum !=  mxmlData->mHealthData->smInfo.at(trueSubMIndex).componetNum)
             {
                 mNetXmlResult = XNCRDataContentError;
                 return;
             }

             for(unsigned long j=0;j < mxmlData->mHealthData->smInfo.at(trueSubMIndex).componetNum;j++)
             {
                 if(data.smInfo.at(i).compInfo.at(j).name !=
                         mxmlData->mHealthData->smInfo.at(trueSubMIndex).compInfo.at(j).name)
                 {
                     unsigned long k=0;
                     for(;k < mxmlData->mHealthData->smInfo.at(trueSubMIndex).componetNum;k++)
                     {
                         if(data.smInfo.at(i).compInfo.at(j).name ==
                                 mxmlData->mHealthData->smInfo.at(trueSubMIndex).compInfo.at(j).name)
                         {
                             mMapComPonet.operator [](trueSubMIndex).insert(k,j);
                             if(XNCRCompleteSame == mNetXmlResult)
                                 mNetXmlResult = XNCNOnlyComponentDif;
                             else
                                 mNetXmlResult = XNCRNeedMAp;
                             break;
                         }
                     }
                     if(k == mxmlData->mHealthData->smInfo.at(trueSubMIndex).componetNum)
                     {
                          mNetXmlResult = XNCRDataContentError;
                         return;
                     }
                 }
             }
        }
    }
}

int StaticLayoutByXml::findMatchWKIndex(int index)
{
    int trueIndex=index;
     QMap<int,int>::iterator ite;
    if(mMapWorkState.count())
    {
        ite = mMapWorkState.find(index);
        if(ite != mMapWorkState.end())
        {
            trueIndex = ite.value();
        }
    }
    return trueIndex;
}

QPair<int, int> StaticLayoutByXml::findMatchSubMIndexPair(int subMIndex, int compIndex)
{
    QMap<int,int>::iterator iteSubM;
    QMap<int,int>::const_iterator iteComp;
    int trueSubMIndex = subMIndex;
    int trueCompIndex = compIndex;

         if(mMapSubMachine.count())
        {   ////需要使用分机的映射
             iteSubM = mMapSubMachine.find(compIndex);
            if(mMapSubMachine.end() != iteSubM) //该分机的顺序是乱的
            {
                trueSubMIndex = iteSubM.value();
            }
        }

      if(mMapComPonet.at(trueSubMIndex).count())
      {
          iteComp = mMapComPonet.at(trueSubMIndex).find(compIndex);
          if(mMapComPonet.at(trueSubMIndex).end() != iteComp)
          {
              trueCompIndex = iteComp.value();
          }
      }
      return QPair<int,int>(trueSubMIndex,trueCompIndex);
}

