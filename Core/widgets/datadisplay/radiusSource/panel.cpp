#include "panel.h"
#include "Base/constants.h"
#include "file/programfilepath.h"
#include "protocol/protocolmanager.h"
#include "Base/util/rsingleton.h"

RadiusPanel::RadiusPanel(QWidget *parent):
    RComponent(Constant::PLUGIN_RADIATION_SOURCE)
{
    pluginId = "0x0003";
    m_name = tr("RadiusSrc panel");
}

QWidget *RadiusPanel::initialize(QWidget *parent)
{
    ProgramFilePath filePath;
    RSingleton<Core::ProtocolManager>::instance()->parseLocalProtocol(filePath.dataViewPath+"/RadiusSrc.xml");

    bool existed = false;
    const Datastruct::BaseProtocol *bprotocol = RSingleton<Core::ProtocolManager>::instance()->getProtocol(QStringLiteral("健康管理"),&existed);

    return NULL;
}

void RadiusPanel::onNetwork(int protocolType, Datastruct::FieldValues &data)
{

}

void RadiusPanel::retranslateUi()
{

}
