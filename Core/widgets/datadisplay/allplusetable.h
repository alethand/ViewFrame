/*!
 *  @details   全脉冲表格
 *  @author    庄雷
 *  @version   1.0
 *  @date      2018.09.13
 *  @copyright NanJing RenGu.
 */
#ifndef ALLPLUSETABLE_H
#define ALLPLUSETABLE_H

#include "Base/pluginmanager/rcomponent.h"

namespace DataView {

class AllPluseTablePrivate;

class AllPluseTable : public Base::RComponent
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AllPluseTable)
public:
    explicit AllPluseTable(QWidget *parent = 0);
    ~AllPluseTable();

    enum WorkModel{
        RealTimeDisplay,        /*!< 实时显示模式 */
        HistoricalDisplay       /*!< 历史显示模式 */
    };

    bool initialize();
    void release();
    QString pluginName();
    void onMessage(MessageType::MessType type);

public slots:
    void recvAllPlusePara(char *buff, int len);

private slots:
    void on_radioButton_RealityShow_clicked();
    void on_radioButton_HistoryShow_clicked();

private:
    void initAllPulseTable();
    void changeTableHeaderInfo(bool blRealityTypeFlag);
    void retranslateUi();

private:
    AllPluseTablePrivate * d_ptr;
};

} //namespace DataView

#endif // ALLPLUSETABLE_H
