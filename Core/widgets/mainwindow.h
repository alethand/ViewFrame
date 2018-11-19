/*!
 *  @brief     通用显示框架
 *  @details   以Dock方式运行子程序页面
 *  @author    wey
 *  @version   1.0
 *  @date      2018.08.22
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QList>
class QAction;

class MapView;

#include "pluginmanager/observer.h"

namespace Ui {
class MainWindow;
}

namespace Core{
class TitleBar;
class ModuleSetting;
}

class ActionContainer;
class BaseInfoDockPanel;

class MainWindow : public QMainWindow , public Core::Observer
{
    Q_OBJECT

public:
    QFont font;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void onMessage(MessageType::MessType type);

    void displayResize();

private:
    void initMenu();

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void sendForHealthPanelResize();/*！导入布局信号*/

private slots:
    void programExit();

    void windowTopHint(bool flag);
    void windowFullScreen(bool flag);

    void switchStyle();
    void switchLanguage();

    void technicalSupport();
    void aboutProgram();

    void showShortcutSettings();
    void screenshotSettings();

    void importView();
    void exportView(bool showOperateResult);

    void rasieWidget(QString wid);

private:
    void loadCmponent();
    void initComponent();
    void updateStyle(int index);
    void updateLanguage(QString lanFileName);
    void loadUserSetting();
    void retranslateUi();

    void setMenuTranslucent(QMenu * menu);

private:
    Ui::MainWindow *ui;

    QList<QAction *> styleActionList;
    QList<QAction *> lanActionList;

    QString curLanguageName;

    ActionContainer * serverMenu;
    ActionContainer * viewMenu;
    ActionContainer * viewManagerMenu;
    ActionContainer * settingsMenu;
    ActionContainer * styleMenu;
    ActionContainer * lanMenu;
    ActionContainer * helpMenu;
    QAction * exitAction;
    QAction * importViewAction;
    QAction * exportViewAction;
    QAction * topHintAction;
    QAction * fullScreenAction;
    QAction * shortcutAction;
    QAction * screenshotAction;
    QAction * supportAction;
    QAction * aboutPorgramAction;

    MapView * mapView;

    Core::TitleBar * titleBar;
    Core::ModuleSetting * settingBar;
};

#endif // MAINWINDOW_H
