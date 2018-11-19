#ifndef RADIUSSRCDISPLAY_H
#define RADIUSSRCDISPLAY_H

#include <QWidget>
#include <QRadioButton>
#include <QStackedWidget>
#include "widgets/datadisplay/modelview/tableviewdata.h"
#include "widgets/datadisplay/DisplayModel/model_details.h"
#include "widgets/datadisplay/DisplayModel/model_cover.h"
#include "widgets/datadisplay/DisplayModel/model_history.h"

using namespace DataView;
class RadiusSrcDisplay:public QWidget,public DataManager
{
    Q_OBJECT
public:
    enum Mode{
       MapDraw,                 //地图标烩模式
       ListRefresh_Roll,        //滚动刷新模式
       ListRefresh_Cover,       //覆盖刷新模式
    };
    explicit RadiusSrcDisplay(const Datastruct::BaseProtocol *protocol);
    void initView();


private slots:
    void switchMode();
    void update();

private:
    Mode curMode;                               /*!< 当前模式 */


    TableView    view_ScrollRefresh;
    TableView    view_CoverRefresh;
    Model_History model_ScrollRefresh;
    Model_Cover   model_CoverRefresh;



    /*** 界面按钮控件 ***/
    QRadioButton *but_MapDraw;
    QRadioButton *but_ScrollFlush;
    QRadioButton *but_CoverRefresh;
    QStackedWidget *stackWidget;

};

#endif // RADIUSSRCDISPLAY_H
