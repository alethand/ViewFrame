#include "dynamiclayoutbynet.h"

DynamicLayoutByNet::DynamicLayoutByNet(QWidget *parent):LayoutAndDisplay(parent)
{

}

DynamicLayoutByNet::~DynamicLayoutByNet()
{

}

bool DynamicLayoutByNet::startLayout(HealthData &data, bool ifUse)
{
    Q_UNUSED(ifUse)
    if(!isHasLayout)
    {
        isHasLayout = true;

        StateLamp::initColorMap();
        LayoutAndDisplay::initColorMap();

        mUpdatePara.reset(&data);
        ensureLayoutData(data);         //解析data数据,确定数据量

        generateWSLayout();             //生成工作状态布局
        generateSubMCpLayout();         //生成分机-组件布局
    }
    return true;
}

QPair<int, int> DynamicLayoutByNet::findMatchSubMIndexPair(int subMIndex, int compIndex)
{
    return QPair<int,int>(subMIndex,compIndex);
}
