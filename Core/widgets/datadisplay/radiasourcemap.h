/*!
 *  @details   数据源地图
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.14
 *  @copyright NanJing RenGu.
 */
#ifndef RADIASOURCEMAP_H
#define RADIASOURCEMAP_H

#include "Base/pluginmanager/rcomponent.h"

namespace DataView {
class RadiaSourceMapPrivate;

class RadiaSourceMap : public Base::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(RadiaSourceMap)
public:
    explicit RadiaSourceMap(QWidget *parent = 0);
    ~RadiaSourceMap();

    bool initialize();
    void release();
    QString pluginName();
    void onMessage(MessageType::MessType type);
private:
    void retranslateUi();

private:
    RadiaSourceMapPrivate * d_ptr;
};

} //namespace DataView

#endif // RADIASOURCEMAP_H
