#include "layoutparsemethod.h"

#include <QDataStream>
#include <QDebug>

namespace Core{

QDataStream & operator << (QDataStream &stream, LayoutItem &item)
{
    stream<<item.objName<<item.geometry<<item.feature;
    return stream;
}

QDataStream & operator >> (QDataStream &stream, LayoutItem &item)
{
    stream>>item.objName>>item.geometry>>item.feature;
    return stream;
}

LayoutParseMethod::LayoutParseMethod()
{
    magicNum = 0xABCDDCBA;
}

LayoutParseMethod::~LayoutParseMethod()
{

}

void LayoutParseMethod::addItem(LayoutItem &item)
{
    items.push_back(item);
}

bool LayoutParseMethod::startParse(Base::RTextFile *file)
{
    QDataStream stream(file);
    int tmpMagic;
    stream>>tmpMagic;
    if(tmpMagic != magicNum)
        return false;

    int size = 0;
    stream>>size;

    for(int i = 0;i < size; i++){
        LayoutItem item;
        stream>>item;
        items.push_back(item);
    }

    return true;
}

bool LayoutParseMethod::startSave(Base::RTextFile *file)
{
    QDataStream stream(file);
    stream<<magicNum;

    stream<<items.size();

    std::for_each(items.begin(),items.end(),[&](LayoutItem item){
        stream<<item;
    });
    return true;
}

}
