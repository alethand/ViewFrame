#include "spectrumgraphics.h"

#include <QHBoxLayout>

#include "Base/constants.h"
#include "Base/util/rsingleton.h"

namespace DataView {

class SpectrumGraphicsPrivate
{
    Q_DECLARE_PUBLIC(SpectrumGraphics)
public:
    explicit SpectrumGraphicsPrivate(SpectrumGraphics * q):q_ptr(q)
    {
        initView();
    }
    void initView();

    QWidget * mainWidget;
    SpectrumGraphics * q_ptr;
};

void SpectrumGraphicsPrivate::initView()
{
    mainWidget = new QWidget();

    QVBoxLayout * vlayout = new QVBoxLayout;

    mainWidget->setLayout(vlayout);
}

SpectrumGraphics::SpectrumGraphics(QWidget *parent) :
    Base::RComponent(Constant::PLUGIN_SPECTRUM_GRAPHICS,parent),d_ptr(new SpectrumGraphicsPrivate(this))
{
    RSingleton<Base::Subject>::instance()->attach(this);
    retranslateUi();
}

bool SpectrumGraphics::initialize()
{
    Q_D(SpectrumGraphics);
    setWidget(d->mainWidget);
    return true;
}

void SpectrumGraphics::release()
{

}

QString SpectrumGraphics::pluginName()
{
    return tr("DataDisplay panel");
}

void SpectrumGraphics::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void SpectrumGraphics::retranslateUi()
{
    m_name = tr("Spectral data graph");
    setWindowTitle(m_name);
}

SpectrumGraphics::~SpectrumGraphics()
{

}

} //namespace DataView
