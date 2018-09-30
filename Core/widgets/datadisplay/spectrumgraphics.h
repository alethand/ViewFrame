/*!
 *  @details   频谱数据图形
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.15
 *  @copyright NanJing RenGu.
 */

#ifndef SPECTRUMGRAPHICS_H
#define SPECTRUMGRAPHICS_H

#include "Base/pluginmanager/rcomponent.h"

namespace DataView {
class SpectrumGraphicsPrivate;

class SpectrumGraphics : public Base::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SpectrumGraphics)
public:
    explicit SpectrumGraphics(QWidget *parent = 0);
    ~SpectrumGraphics();

    bool initialize();
    void release();
    QString pluginName();
    void onMessage(MessageType::MessType type);
    void retranslateUi();

private:
    SpectrumGraphicsPrivate * d_ptr;

};

} //namespace DataView

#endif // SPECTRUMGRAPHICS_H
