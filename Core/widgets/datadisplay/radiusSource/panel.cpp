#include "panel.h"
#include "Base/constants.h"
#include "file/programfilepath.h"
#include "protocol/protocolmanager.h"
#include "Base/util/rsingleton.h"

#include <QDebug>

RadiusPanel::RadiusPanel(QWidget *parent):
    RComponent(Constant::PLUGIN_RADIATION_SOURCE,parent)
{
    pluginId = "0x0003";
    m_name = tr("RadiusSrc panel");
    widget = NULL;
}

QWidget *RadiusPanel::initialize(QWidget *parent)
{
    ProgramFilePath filePath;
    RSingleton<Core::ProtocolManager>::instance()->parseLocalProtocol(filePath.dataViewPath+QStringLiteral("/辐射源.xml"));
    bool existed = false;
    const Datastruct::BaseProtocol *bprotocol = RSingleton<Core::ProtocolManager>::instance()->getProtocol(QStringLiteral("辐射源"),&existed);

    if(existed)
        widget = new RadiusSrcDisplay(bprotocol,parent);

    return widget;
}

QString RadiusPanel::pluginName()
{
    return QObject::tr("Dataview panel");
}

Core::RComponent *RadiusPanel::clone()
{
    return new RadiusPanel();
}

void RadiusPanel::onNetwork(int protocolType, Datastruct::FieldValues &data)
{

}

void RadiusPanel::onNetWork(int protocolType, NetParse::Protocol *data)
{
    widget->handleNetInfo(data);
}

void RadiusPanel::retranslateUi()
{

}
