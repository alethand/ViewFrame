#ifndef PANEL_H
#define PANEL_H

#include "pluginmanager/rcomponent.h"

/*!
 * \brief The Panel class 辐射源插件面板
 */
class RadiusPanel: public Core::RComponent
{
    Q_OBJECT
public:
    explicit RadiusPanel(QWidget *parent=0);

    QWidget *initialize(QWidget * parent);
    void release(){}
    void onNetwork(int protocolType,Datastruct::FieldValues & data);

protected:
    void retranslateUi();
};

#endif // PANEL_H
