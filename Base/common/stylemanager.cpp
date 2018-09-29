#include "stylemanager.h"

#include <QFile>
#include <QApplication>

#include "../util/rlog.h"

namespace Base{

class StyleManagerPrivate
{
    Q_DECLARE_PUBLIC(StyleManager)

public:
    StyleManagerPrivate(StyleManager * q):q_ptr(q){

    }

    StyleManager * q_ptr;
    StylePtrList slist;
    CustomStyle * currStyle;
};

StyleManager::StyleManager():d_ptr(new StyleManagerPrivate(this)),QObject()
{

}

void StyleManager::addStyle(CustomStyle *style)
{
    Q_D(StyleManager);
    d->slist.push_back(style);
}

int StyleManager::size()
{
    Q_D(StyleManager);
    return d->slist.size();
}

StylePtrList StyleManager::styles()
{
    Q_D(StyleManager);
    return d->slist;
}

void StyleManager::switchStyle(int index)
{
    Q_D(StyleManager);
    Q_ASSERT(index < d->slist.size());
    d->currStyle = d->slist.at(index);
    switch(d->currStyle->getStyleType()){
        case CustomStyle::STYLE_SHEET:
            {
                QFile styleFile(d->currStyle->getStylePath());
                if(!styleFile.open(QFile::ReadOnly)){
                    RLOG_ERROR("style file %s read error!",d->currStyle->getStylePath().toLocal8Bit().data());
                    return;
                }
                qApp->setStyleSheet(styleFile.readAll());
            }
            break;
        case CustomStyle::STYLE_CUSTOMSTYLE:
            switch(d->currStyle->getClazz()){
                //qApp->setStyle();
                default:
                    break;
            }
            break;
        default:
            break;
    }
}

CustomStyle * StyleManager::currentStyle()
{
    Q_D(StyleManager);
    return d->currStyle;
}

CustomStyle *StyleManager::findStyle(QString styleName)
{
    Q_D(StyleManager);
    for(int i = 0; i < d->slist.size();i++){
        if(d->slist.at(i)->getStyleName() == styleName)
            return d->slist.at(i);
    }
    return NULL;
}

}
