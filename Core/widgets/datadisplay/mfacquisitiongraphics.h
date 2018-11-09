/*!
 *  @details   中频数据图形
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.13
 *  @copyright NanJing RenGu.
 */
#ifndef MFACQUISITIONGRAPHICS_H
#define MFACQUISITIONGRAPHICS_H

#include "pluginmanager/rcomponent.h"

namespace DataView {

class MFAcquisitionGraphicsPrivate;

class MFAcquisitionGraphics : public Core::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MFAcquisitionGraphics)
public:
    explicit MFAcquisitionGraphics(QWidget *parent = 0);
    ~MFAcquisitionGraphics();

    bool initialize();
    void release();
    QString pluginName();
    RComponent * clone();
    void onMessage(MessageType::MessType type);
    void onNetwork(int protocolType,Datastruct::FieldValues & data);

private:
    void retranslateUi();

private:
    MFAcquisitionGraphicsPrivate * d_ptr;
};

} //namespace DataView

#endif // MFACQUISITIONGRAPHICS_H
