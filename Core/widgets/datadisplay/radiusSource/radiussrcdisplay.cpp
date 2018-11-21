#include "radiussrcdisplay.h"
#include "protocol/pluginloader.h"
#include "Base/util/rsingleton.h"
#include "protocol/commonprotocolparse.h"
#include "protocol/protocolmanager.h"
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>


RadiusSrcDisplay::RadiusSrcDisplay(const Datastruct::BaseProtocol *protocol, QWidget *parent)
    :QWidget(parent),DataManager(protocol),
     model_ScrollRefresh(this),
     model_CoverRefresh(this,NULL)
{
    curMode = MapDraw;
    initView();
}

void RadiusSrcDisplay::initView()
{
    QWidget * mainWidget = new QWidget(this);
    mainWidget->setObjectName("mainWidget");

    QGroupBox *groupBox = new QGroupBox();

    but_MapDraw = new QRadioButton(groupBox);
    but_MapDraw->setText(QStringLiteral("地图标绘"));

    but_ScrollFlush = new QRadioButton(groupBox);
    but_ScrollFlush->setText(QStringLiteral("滚动刷新"));

    but_CoverRefresh = new QRadioButton(groupBox);
    but_CoverRefresh->setText(QStringLiteral("覆盖刷新"));

    QHBoxLayout * groupLayout = new QHBoxLayout();
    groupLayout->setContentsMargins(1,1,1,1);
    groupLayout->addWidget(but_MapDraw);
    groupLayout->addWidget(but_ScrollFlush);
    groupLayout->addWidget(but_CoverRefresh);
    groupBox->setLayout(groupLayout);

    QWidget *container =new QWidget(mainWidget);
    QHBoxLayout * containerLayout = new QHBoxLayout();
    containerLayout->setContentsMargins(1,1,1,1);
    containerLayout->addStretch(0);
    containerLayout->addWidget(groupBox);

    container->setLayout(containerLayout);

    stackWidget = new QStackedWidget(mainWidget);
    update();
    stackWidget->insertWidget(0,&view_ScrollRefresh);
    stackWidget->insertWidget(1,&view_CoverRefresh);

    QVBoxLayout * mainLaout = new QVBoxLayout();
    mainLaout->setContentsMargins(1,1,1,1);
    mainLaout->addWidget(container);
    mainLaout->addWidget(stackWidget);

    mainWidget->setLayout(mainLaout);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(mainWidget);
    setLayout(layout);

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
         stackWidget->setCurrentIndex(0);
         update();
    }
    else  if(obj->objectName() == but_CoverRefresh->objectName()) {
         stackWidget->setCurrentIndex(1);
         update();
    }

    //RSingleton<Core::PluginLoader>::instance()->getModules()->find("0x0003").v  ;
}

void RadiusSrcDisplay::update()
{
    view_ScrollRefresh.setModel(&model_ScrollRefresh);
    view_CoverRefresh.setModel(&model_CoverRefresh);
}
