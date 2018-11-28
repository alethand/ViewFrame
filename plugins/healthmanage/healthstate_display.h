/*!
 *  @brief    健康管理协议
 *  @details  帧起始标记：2字节 - 0x1ACF
     *        数据类型：4字节 - 0x2345
     *        数据长度：4字节 - 有效标记到包尾
     *        工作状态个数N：4字节
     *        【工作状态名称：char[64]
     *          工作状态内容：char[64]
     *          工作状态内容文字颜色：2字节    //具体解析方式或许会变化
     *         】N组
     *
     *         分机状态个数M:4字节
     *         【
     *              分机m名称：char[64]
     *              分机m组件个数L：4字节
     *              【
     *                  分机m组件l名称： char[64]
     *                  分机m组件l状态：2字节    //解析方式或许会变化
     *               】L组
     *          】M组
 *  @author    yww
 *  @version   1.0
 *  @date      2018.09.17
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef HEALTHSTATE_DISPLAY_H
#define HEALTHSTATE_DISPLAY_H

#include <QWidget>
#include "layoutset.h"
#include "fileOpt.h"
#include "head.h"

#define  HEALTHSTATE_NAMEORCONTENTLEN 64            //健康状态名字或内容字节长度

class LayoutAndDisplay;
using namespace HealthState;
using HealthState::HealthData;
using HealthState::XlsHealthState;

/*!
 * \brief The HealthState_Display class   健康管理界面总的管理类
 */
class HealthState_Display : public QWidget
{
    Q_OBJECT

public:
    explicit HealthState_Display(QWidget *parent = 0);
    ~HealthState_Display();

    void openDynamicLayout(bool ifOpen);
    void openRecordData(bool ifRecord);
    void handleNetData(char *data,int size);

public://测试用函数
     void handleNetData(HealthData &result);

protected:
     void resizeEvent(QResizeEvent *event);

signals:
     void sendForHealthPanelResize();
public slots:
     void recForHealthPanelResize();

private:
    void  analysisNetData(char* data,int size,HealthData &result);

private:
    //动态布局实现
    bool ifDynamicLayout;                                          //是否为动态布局，网络布局需要接受网络数据，静态布局开机就可以
    LayoutAndDisplay *mDisplayElem;

    //数据记录
    bool ifRecordData;
    HealthState::XlsHealthState *xlsPrinter;

    //解析的网络数据
    HealthData  mNetData;
};

#endif // HEALTHSTATE_DISPLAY_H
