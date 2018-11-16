﻿/*!
 *  @brief     布局解析与保存
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.16
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef LAYOUTPARSEMETHOD_H
#define LAYOUTPARSEMETHOD_H

#include "Base/util/fileutils.h"

namespace Base {
class RTextFile;
}

#include <QList>
#include <QRect>

namespace Core{

struct LayoutItem{
    QString objName;
    QRect geometry;
    int feature;

    friend QDataStream & operator << (QDataStream & stream,LayoutItem & item);
    friend QDataStream & operator >> (QDataStream & stream,LayoutItem & item);

};

class LayoutParseMethod : public Base::RTextParseMethod
{
public:
    explicit LayoutParseMethod();
    ~LayoutParseMethod();

    void addItem(Core::LayoutItem & item);
    QList<LayoutItem> getItems(){return this->items;}

    bool  startParse(Base::RTextFile * file);
    bool  startSave(Base::RTextFile * file);

private:
    int magicNum;
    QList<LayoutItem> items;
};

}

#endif // LAYOUTPARSEMETHOD_H
