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
#include "pluginmanager/rcomponent.h"
#include "widgets/datadisplay/Graphics/scatterdiagram.h"
#include "widgets/datadisplay/Graphics/histogram.h"
#include "widgets/datadisplay/Graphics/waveform.h"
using namespace Datastruct;
using namespace Diagram;
namespace DataView {

class AllPluseGraphicsPrivate;

class AllPluseGraphics: public Core::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AllPluseGraphics)
public:
    explicit AllPluseGraphics(QWidget *parent = 0);
    ~AllPluseGraphics();

    QWidget * initialize(QWidget * parent);
    void release();
    QString pluginName();
    Core::RComponent * clone();
    void onMessage(MessageType::MessType type);
    void onNetwork(int protocolType,Datastruct::FieldValues & data);

private:
    void retranslateUi();
    void drawTimeParaGraphis();
    void drawBarGraphis();

private slots:
   // void recvAllPulseInfoList(AllPluseStatisticInfoList* statisticInfoList,AllPulseOriginalInfoList* originalInfoList);

private:
    AllPluseGraphicsPrivate * d_ptr;
   // AllPluseStatisticInfoList allPluseStatisticInfoList;

    Diagram::Scatter  timeScatter;
    Diagram::Histogram freqHistogram;
    Diagram::WaveForm  timeAreaWaveForm;
};

} //namespace DataView

#endif // ALLPLUSEGRAPHICS_H
