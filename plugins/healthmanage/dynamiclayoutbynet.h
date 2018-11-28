/*!
 *  @brief     通过网络产生动态布局
 *  @details
 *  @author    yww
 *  @version   1.0
 *  @date      2018.09.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DYNAMICLAYOUTBYNET_H
#define DYNAMICLAYOUTBYNET_H

#include "layoutset.h"

class DynamicLayoutByNet : public LayoutAndDisplay
{
    Q_OBJECT
public:
    DynamicLayoutByNet(QWidget* parent);
    ~DynamicLayoutByNet();

    bool startLayout(HealthData &data,bool ifUse);
    int findMatchWKIndex(int index){return index;}
    QPair<int,int> findMatchSubMIndexPair(int subMIndex,int compIndex);
};

#endif // DYNAMICLAYOUTBYNET_H
