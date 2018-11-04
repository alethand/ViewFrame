#include "radiasourcemap.h"

#include <QVBoxLayout>

#include "Base/constants.h"
#include "Base/util/rsingleton.h"

namespace DataView {

class RadiaSourceMapPrivate
{
    Q_DECLARE_PUBLIC(RadiaSourceMap)
public:
    explicit RadiaSourceMapPrivate(RadiaSourceMap * q):q_ptr(q)
    {
        initView();
    }
    void initView();

    QWidget * mainWidget;
    RadiaSourceMap * q_ptr;
};

void RadiaSourceMapPrivate::initView()
{
    mainWidget = new QWidget();
    QVBoxLayout * vlayout = new QVBoxLayout;
    mainWidget->setLayout(vlayout);
}

RadiaSourceMap::RadiaSourceMap(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_RADIA_SOURCE_MAP,parent),d_ptr(new RadiaSourceMapPrivate(this))
{
    RSingleton<Base::Subject>::instance()->attach(this);
    retranslateUi();
}

RadiaSourceMap::~RadiaSourceMap()
{
}

bool RadiaSourceMap::initialize()
{
    Q_D(RadiaSourceMap);
    setWidget(d->mainWidget);
    return true;
}

void RadiaSourceMap::release()
{

}

QString RadiaSourceMap::pluginName()
{
    return tr("DataDisplay panel");
}

void RadiaSourceMap::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void RadiaSourceMap::retranslateUi()
{
    m_name = tr("Data source Map");
    setWindowTitle(m_name);
}

} //namespace DataView
