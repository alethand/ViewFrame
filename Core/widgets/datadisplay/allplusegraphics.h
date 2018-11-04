/*!
 *  @details   全脉冲图形
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.14
 *  @copyright NanJing RenGu.
 */

#ifndef ALLPLUSEGRAPHICS_H
#define ALLPLUSEGRAPHICS_H
#include "protocol/datastruct.h"
#include "Base/pluginmanager/rcomponent.h"
#include "widgets/datadisplay/Graphics/scatterdiagram.h"
using namespace Datastruct;
using namespace Diagram;
namespace DataView {

class AllPluseGraphicsPrivate;

class AllPluseGraphics: public Base::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AllPluseGraphics)
public:
    explicit AllPluseGraphics(QWidget *parent = 0);
    ~AllPluseGraphics();

    bool initialize();
    void release();
    QString pluginName();
    void onMessage(MessageType::MessType type);

private:
    void retranslateUi();
    void drawTimeParaGraphis();
    void drawBarGraphis();

private slots:
   // void recvAllPulseInfoList(AllPluseStatisticInfoList* statisticInfoList,AllPulseOriginalInfoList* originalInfoList);

private:
    AllPluseGraphicsPrivate * d_ptr;
   // AllPluseStatisticInfoList allPluseStatisticInfoList;
    AllPulseOriginalInfoList allPluseOriginalInfoList;
    Diagram::Scatter  timeScatter;
    Diagram::Histogram freqHistogram;
};




} //namespace DataView

#endif // ALLPLUSEGRAPHICS_H
