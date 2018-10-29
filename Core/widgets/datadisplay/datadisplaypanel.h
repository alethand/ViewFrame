/*!
 *  @brief     数据显示面板
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef DATADISPLAYPANEL_H
#define DATADISPLAYPANEL_H

class DataDisplay;

#include "Base/pluginmanager/rcomponent.h"

namespace DataView {

class DataDisplayPanel: public Base::RComponent
{
    Q_OBJECT
public:
    DataDisplayPanel(QWidget *parent = 0);
    ~DataDisplayPanel();

    bool initialize();
    void release();
    void onMessage(MessageType::MessType type);

private:
    void retranslateUi();

private:
    QAction * spectrumGraphicsAction;
    DataDisplay* dataDisplay;
};

}

#endif // DATADISPLAYPANEL_H
