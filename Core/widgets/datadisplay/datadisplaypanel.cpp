#include "datadisplaypanel.h"

#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "datadisplay.h"

namespace DataView {

DataDisplayPanel::DataDisplayPanel(QWidget *parent):
    Base::RComponent(Constant::PLUGIN_DATA_DISPLAY,parent)
{
    retranslateUi();
    RSingleton<Base::Subject>::instance()->attach(this);
}

DataDisplayPanel::~DataDisplayPanel()
{

}

bool DataDisplayPanel::initialize()
{
    dataDisplay = new DataDisplay(this);
    setWidget(dataDisplay);

    return true;
}

void DataDisplayPanel::release()
{

}

void DataDisplayPanel::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void DataDisplayPanel::retranslateUi()
{
    m_name = tr("DataDisplay panel");
    setWindowTitle(m_name);
}

} //namespace DataView
