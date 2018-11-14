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

#include "pluginmanager/rcomponent.h"
#include "healthstate_display.h"

class  HealthInfoDockPanel : public Core::RComponent
{
  Q_OBJECT
public:
     HealthInfoDockPanel(QWidget *parent = 0);
     virtual ~HealthInfoDockPanel(){}

     QWidget * initialize(QWidget * parent);
     void release(){}
     QString pluginName();
     QString MachineName();
     Core::RComponent * clone();
     QSize sizeHint()const;
     void onMessage(MessageType::MessType type);
     void onNetwork(int protocolType,Datastruct::FieldValues & data);

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
