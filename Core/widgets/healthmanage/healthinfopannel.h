/*!
 *  @brief     健康管理显示面板
 *  @details
 *  @author    yww
 *  @version   1.0
 *  @date      2018.09.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef HEALTHINFOPANNEL_H
#define HEALTHINFOPANNEL_H

#include "Base/pluginmanager/rcomponent.h"
#include "healthstate_display.h"

class  HealthInfoDockPanel : public Base::RComponent
{
  Q_OBJECT
public:
     HealthInfoDockPanel(QWidget *parent = 0);
     virtual ~HealthInfoDockPanel(){}

     bool initialize();
     void release(){}
     QString pluginName();
     QString MachineName();
     void onMessage(MessageType::MessType type);
     QSize sizeHint()const;

private slots:
     void retranslateUi();
signals:
     void sendForHealthPanelResize();
public slots:
     void recForHealthPanelResize();

private:
     HealthState_Display *infoWidget;
};

#endif // HEALTHINFOPANNEL_H
