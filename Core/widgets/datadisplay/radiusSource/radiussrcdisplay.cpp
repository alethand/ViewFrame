#include "radiussrcdisplay.h"
#include "protocol/pluginloader.h"
#include "Base/util/rsingleton.h"
#include "protocol/commonprotocolparse.h"
#include "protocol/protocolmanager.h"
#include <QGroupBox>
#include <QHBoxLayout>


RadiusSrcDisplay::RadiusSrcDisplay(const Datastruct::BaseProtocol *protocol)
    :DataManager(protocol),
     model_ScrollRefresh(this),
     model_CoverRefresh(this,NULL)
    //,RSingleton<Core::ProtocolManager>::instance()->getProtocol(QStringLiteral("健康管理"))
{
    curMode = MapDraw;
    initView();
}

void RadiusSrcDisplay::initView()
{
    but_MapDraw = new QRadioButton();
    but_MapDraw->setText(tr("地图标绘"));
    but_ScrollFlush = new QRadioButton();
    but_ScrollFlush->setText(tr("滚动刷新"));
    but_CoverRefresh = new QRadioButton();
    but_CoverRefresh->setText(tr("覆盖刷新"));
    QGroupBox *groupBox = new QGroupBox();
    groupBox->setLayout(new QHBoxLayout());
    groupBox->layout()->addWidget(but_MapDraw);
    groupBox->layout()->addWidget(but_ScrollFlush);
    groupBox->layout()->addWidget(but_CoverRefresh);

    QWidget *container =new QWidget();
    container->setLayout(new QHBoxLayout());
    container->layout()->addWidget(groupBox);
    dynamic_cast<QHBoxLayout*>(container->layout())->insertStretch(0);


    stackWidget = new QStackedWidget();




    connect(but_MapDraw,SIGNAL(clicked(bool)),this,SLOT(switchMode()));
    connect(but_ScrollFlush,SIGNAL(clicked(bool)),this,SLOT(switchMode()));
    connect(but_CoverRefresh,SIGNAL(clicked(bool)),this,SLOT(switchMode()));

}

void RadiusSrcDisplay::switchMode()
{
    QObject *obj = QObject::sender();
    if(obj->objectName() == but_MapDraw->objectName()) {

    }
    else if(obj->objectName() == but_ScrollFlush->objectName()) {

    }
    else  if(obj->objectName() == but_CoverRefresh->objectName()) {

    }

    //RSingleton<Core::PluginLoader>::instance()->getModules()->find("0x0003").v  ;
}
