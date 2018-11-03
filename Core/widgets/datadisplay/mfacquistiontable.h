﻿/*!
 *  @details   中频数据表格
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.13
 *  @copyright NanJing RenGu.
 */
#ifndef MFACQUISTIONTABLE_H
#define MFACQUISTIONTABLE_H

#include "Base/pluginmanager/rcomponent.h"
#include "protocol/datastruct.h"
#include "modelview/tableviewdata.h"
#include "Graphics/waveform.h"
#include <QAction>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QSortFilterProxyModel>
#include <QFileDialog>
#include <QDesktopServices>
#include <QPushButton>

using namespace Protocol::Data;
using namespace Datastruct;
using namespace Diagram;

namespace DataView {
class MFAcquistionTablePrivate;

class MFAcquistionTable : public Base::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(MFAcquistionTable)
public:
    explicit MFAcquistionTable(QWidget *parent = 0);
    ~MFAcquistionTable();

    bool initialize();
    void release();
    QString pluginName();
    void onMessage(MessageType::MessType type);

public slots:
    void recvMidFreqData(char *buff, int len);                                          /*! 接收网络数据 */
    void clearTable();                                                                  /*! 文件另存为 */
    void on_btn_load_clicked();

    void showTimeAreaFreq();                                                            /*! 时域频谱图 */
    void showFFT();                                                                     /*! FFT频谱图 */
private:
    void retranslateUi();
    QDateTime getCurrentDate();


private:
    MFAcquistionTablePrivate * d_ptr;
    QPushButton * btn_load;
    QWidget *     mainWidget;


};

} //namespace DataView

#endif // MFACQUISTIONTABLE_H
