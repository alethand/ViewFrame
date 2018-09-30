/*!
 *  @details   全脉冲图形
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.14
 *  @copyright NanJing RenGu.
 */

#ifndef ALLPLUSEGRAPHICS_H
#define ALLPLUSEGRAPHICS_H

#include "Base/pluginmanager/rcomponent.h"

namespace DataView {

class AllPluseGraphicsPrivate;

class AllPluseGraphics: public Base::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AllPluseGraphics)
public:
    explicit AllPluseGraphics(QWidget *parent = 0);
    ~AllPluseGraphics();

    bool initialize();
    void release();
    QString pluginName();
    void onMessage(MessageType::MessType type);

private:
    void retranslateUi();

private:
    AllPluseGraphicsPrivate * d_ptr;
};

} //namespace DataView

#endif // ALLPLUSEGRAPHICS_H
