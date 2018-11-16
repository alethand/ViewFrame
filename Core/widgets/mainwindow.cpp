﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAction>
#include <QActionGroup>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

#include "Base/actionmanager/actioncontainer.h"
#include "Base/actionmanager/actionmanager.h"
#include "Base/actionmanager/action.h"
#include "pluginmanager/pluginmanager.h"
#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "Base/common/stylemanager.h"
#include "Base/common/languagemanager.h"
#include "pluginmanager/subject.h"
#include "Base/util/rlog.h"
#include "Base/util/fileutils.h"
#include "healthmanage/healthinfopannel.h"

#include "widgets/datadisplay/radiusSource/radiationsourcetable.h"
#include "datadisplay/allplusetable.h"
#include "datadisplay/mfacquistiontable.h"
#include "widgets/datadisplay/radiusSource/radiasourcemap.h"
#include "datadisplay/allplusegraphics.h"
#include "datadisplay/mfacquisitiongraphics.h"

#include "network/protocolparsethread.h"

#include "global.h"
#include "file/globalconfigfile.h"
#include "file/programfilepath.h"
#include "widgets/taskcontrol/taskcontrolpanel.h"
#include "shortcutsettings.h"
#include "network/tcpserver.h"
#include "network/taskmanager.h"

#include "selfwidget/modulesetting.h"
#include "selfwidget/titlebar.h"

#include "selfwidget/mydockwidget.h"
#include "taskcontrol/taskcontrol.h"
#include "Base/util/rutil.h"
#include "mapview.h"
#include "file/layoutparsemethod.h"

#include <QDateTime>
#include <QScreen>
#include <QFileDialog>
#include <QTimer>

#include "protocol/pluginloader.h"

using namespace Base;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    titleBar(NULL),settingBar(NULL),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("ViewFrame"));

    //TODO 20180915 待添加对配置路径下文件搜索
    RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Technology style"),":/resource/style/Technology.qss",true));
    RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Dark style"),":/resource/style/Black.qss",false));
    RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Light style"),":/resource/style/White.qss",false));
    RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Technology(Big) style"),":/resource/style/TechnologyBig.qss",false));
    RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Dark(Big) style"),":/resource/style/BlackBig.qss",false));
    RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Light(Big) style"),":/resource/style/WhiteBig.qss",false));

    RSingleton<Core::Subject>::instance()->attach(this);

    initMenu();
    initComponent();

    updateLanguage(curLanguageName);

    loadUserSetting();
    if(!RGlobal::G_GlobalConfigFile->systemConfigInfo.fullscreen)
        showMaximized();
}

MainWindow::~MainWindow()
{
    RSingleton<Core::ProtocolParseThread>::instance()->stopMe();

    delete ui;
}

void MainWindow::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void MainWindow::initMenu()
{
    ActionContainer *menubar = ActionManager::instance()->createMenuBar(Constant::MENU_BAR);

    setMenuBar(menubar->menuBar());
//    QMenu *file = menuBar()->addMenu(tr("&file"));
    menubar->appendGroup(Constant::MENU_PROGRAM);
    menubar->appendGroup(Constant::MENU_VIEW);
    menubar->appendGroup(Constant::MENU_SETTING);
    menubar->appendGroup(Constant::MENU_HELP);

    QAction * coreAction = new QAction();
    coreAction->setVisible(false);
    ActionManager::instance()->registAction(Constant::PLUGIN_CORE,coreAction);

    //程序菜单
    serverMenu = ActionManager::instance()->createMenu(Constant::MENU_PROGRAM);
    setMenuTranslucent(serverMenu->menu());
    serverMenu->appendGroup(Constant::MENU_PROGRAM);
    menubar->addMenu(serverMenu, Constant::MENU_PROGRAM);

    exitAction = new QAction(this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(programExit()));
    Action * serverDelete = ActionManager::instance()->registAction(Constant::PROGRAM_EXIT,exitAction);
    serverDelete->setDefaultKey(QKeySequence("Ctrl+Q"));

    serverMenu->addAction(serverDelete,Constant::MENU_PROGRAM);

    //视图菜单
    viewMenu = ActionManager::instance()->createMenu(Constant::MENU_VIEW);
    setMenuTranslucent(viewMenu->menu());
    viewMenu->appendGroup(Constant::MENU_VIEW);
    menubar->addMenu(viewMenu, Constant::MENU_VIEW);

    viewManagerMenu = ActionManager::instance()->createMenu(Constant::VIEW_MANAGER);
    setMenuTranslucent(viewManagerMenu->menu());
    viewManagerMenu->appendGroup(Constant::VIEW_MANAGER);
    viewMenu->addMenu(viewManagerMenu,Constant::VIEW_MANAGER);

    importViewAction = new QAction(this);
    connect(importViewAction,SIGNAL(triggered()),this,SLOT(importView()));
    Action * importView = ActionManager::instance()->registAction(Constant::IMPORT_VIEW,importViewAction);
    importView->setDefaultKey(QKeySequence("Ctrl+Shift+I"));
    viewManagerMenu->addAction(importView,Constant::VIEW_MANAGER);

    exportViewAction = new QAction(this);
    connect(exportViewAction,SIGNAL(triggered(bool)),this,SLOT(exportView(bool)));
    Action * exportView = ActionManager::instance()->registAction(Constant::EXPORT_VIEW,exportViewAction);
    exportView->setDefaultKey(QKeySequence("Ctrl+Shift+E"));
    viewManagerMenu->addAction(exportView,Constant::VIEW_MANAGER);

    viewMenu->addMenu(viewManagerMenu,Constant::MENU_VIEW);
    viewMenu->addSeparator(Constant::MENU_VIEW);

    //设置菜单
    settingsMenu = ActionManager::instance()->createMenu(Constant::MENU_SETTING);
    setMenuTranslucent(settingsMenu->menu());
    settingsMenu->appendGroup(Constant::MENU_SETTING);
    menubar->addMenu(settingsMenu, Constant::MENU_SETTING);

    topHintAction = new QAction(this);
    topHintAction->setCheckable(true);
    connect(topHintAction,SIGNAL(triggered(bool)),this,SLOT(windowTopHint(bool)));
    Action * topHint = ActionManager::instance()->registAction(Constant::TOP_HINT,topHintAction);
    topHint->setDefaultKey(QKeySequence("Ctrl+Shift+T"));
    settingsMenu->addAction(topHint,Constant::MENU_SETTING);

    fullScreenAction = new QAction(this);
    fullScreenAction->setCheckable(true);
    connect(fullScreenAction,SIGNAL(triggered(bool)),this,SLOT(windowFullScreen(bool)));
    Action * fullScreen = ActionManager::instance()->registAction(Constant::FULL_SCREEN,fullScreenAction);
    fullScreen->setDefaultKey(QKeySequence("Ctrl+Shift+f11"));
    settingsMenu->addAction(fullScreen,Constant::MENU_SETTING);

    settingsMenu->addSeparator(Constant::MENU_SETTING);

    //样式
    settingsMenu->appendGroup(Constant::CUSTOM_STYLE);
    styleMenu = ActionManager::instance()->createMenu(Constant::CUSTOM_STYLE);
    setMenuTranslucent(styleMenu->menu());
    styleMenu->appendGroup(Constant::CUSTOM_STYLE);
    settingsMenu->addMenu(styleMenu,Constant::CUSTOM_STYLE);

    StylePtrList styles = RSingleton<StyleManager>::instance()->styles();
    QActionGroup * styleGroup = new QActionGroup(this);
    for(int i = 0;i<styles.size();i++){
        QAction * action = new QAction(styles.at(i)->getStyleName());
        styleActionList.append(action);
        action->setCheckable(true);
        if(styles.at(i)->isSelected()){
            action->setChecked(styles.at(i)->isSelected());
            updateStyle(i);
        }
        styles.at(i)->setAction(action);
        styleGroup->addAction(action);
        connect(action,SIGNAL(triggered()),this,SLOT(switchStyle()));
        action->setProperty(Constant::CUSTOM_STYLE,i);
        Action * tmpStyle = ActionManager::instance()->registAction(QString("StyleAction%1").arg(i).toLocal8Bit().data(),action);
        styleMenu->addAction(tmpStyle,Constant::CUSTOM_STYLE);
    }

    //翻译文件
    settingsMenu->appendGroup(Constant::SYSTEM_LANGUAGE);
    lanMenu = ActionManager::instance()->createMenu(Constant::SYSTEM_LANGUAGE);
    setMenuTranslucent(lanMenu->menu());
    lanMenu->appendGroup(Constant::SYSTEM_LANGUAGE);
    settingsMenu->addMenu(lanMenu,Constant::SYSTEM_LANGUAGE);

    LanguagePtrList lans = RSingleton<LanguageManager>::instance()->languages();
    QActionGroup * lanGroup = new QActionGroup(this);
    for(int i = 0;i<lans.size();i++){
        QAction * action = new QAction(lans.at(i)->getName());
        lanActionList.append(action);
        action->setCheckable(true);
        if(lans.at(i)->isSelected()){
            action->setChecked(lans.at(i)->isSelected());
            curLanguageName = lans.at(i)->getFileName();
        }
        lans.at(i)->setAction(action);
        lanGroup->addAction(action);
        connect(action,SIGNAL(triggered()),this,SLOT(switchLanguage()));
        action->setProperty(Constant::SYSTEM_LANGUAGE,lans.at(i)->getFileName());
        Action * tmpStyle = ActionManager::instance()->registAction(QString("LanAction%1").arg(i).toLocal8Bit().data(),action);
        lanMenu->addAction(tmpStyle,Constant::SYSTEM_LANGUAGE);
    }

    //快捷键盘
    settingsMenu->appendGroup(Constant::SYSTEM_SHORTCUT);
    shortcutAction = new QAction(this);
    connect(shortcutAction,SIGNAL(triggered()),this,SLOT(showShortcutSettings()));
    Action * shortcut = ActionManager::instance()->registAction(Constant::SYSTEM_SHORTCUT,shortcutAction);
    shortcut->setDefaultKey(QKeySequence("Ctrl+Shift+K"));
    settingsMenu->addAction(shortcut,Constant::SYSTEM_SHORTCUT);

    //截图
    settingsMenu->appendGroup(Constant::SYSTEM_ScreenShot);
    screenshotAction = new QAction(this);
    connect(screenshotAction,SIGNAL(triggered()),this,SLOT(screenshotSettings()));
    Action * screenshot = ActionManager::instance()->registAction(Constant::SYSTEM_ScreenShot,screenshotAction);
//    shortcut->setDefaultKey(QKeySequence("Ctrl+Shift+K"));
    settingsMenu->addAction(screenshot,Constant::SYSTEM_ScreenShot);


    //帮助菜单
    helpMenu = ActionManager::instance()->createMenu(Constant::MENU_HELP);
    setMenuTranslucent(helpMenu->menu());
    helpMenu->appendGroup(Constant::MENU_HELP);
    menubar->addMenu(helpMenu, Constant::MENU_HELP);

    supportAction = new QAction(this);
    connect(supportAction,SIGNAL(triggered()),this,SLOT(technicalSupport()));
    Action * support = ActionManager::instance()->registAction(Constant::TEC_SUPPORT,supportAction);
    helpMenu->addAction(support,Constant::MENU_HELP);

    helpMenu->addSeparator(Constant::MENU_HELP);

    aboutPorgramAction = new QAction(this);
    connect(aboutPorgramAction,SIGNAL(triggered()),this,SLOT(aboutProgram()));
    Action * aboutProgram = ActionManager::instance()->registAction(Constant::ABOUT_PROGRAM,aboutPorgramAction);
    helpMenu->addAction(aboutProgram,Constant::MENU_HELP);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    exportView(true);
    event->accept();
    exit(0);
}

/*!
 * @brief   程序退出
 */
void MainWindow::programExit()
{
    emit close();
}

/*!
 * @brief   程序置顶
 */
void MainWindow::windowTopHint(bool flag)
{
    Qt::WindowFlags flags = windowFlags();
    if(flag)
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags = flags & ~Qt::WindowStaysOnTopHint;

    setWindowFlags(flags);
    show();
    RGlobal::G_GlobalConfigFile->systemConfigInfo.topHint = flag;
    RGlobal::G_GlobalConfigFile->saveFile();
}

void MainWindow::windowFullScreen(bool flag)
{
    Qt::WindowStates state = windowState();
    if(flag)
    {
        state |= Qt::WindowFullScreen;
    }
    else
    {
        state = state & ~Qt::WindowFullScreen;
        showMaximized();
    }

    setWindowState(state);

    RGlobal::G_GlobalConfigFile->systemConfigInfo.fullscreen = flag;
    RGlobal::G_GlobalConfigFile->saveFile();
}

/*!
 * @brief 切换显示样式
 */
void MainWindow::switchStyle()
{
    int switchIndex = QObject::sender()->property(Constant::CUSTOM_STYLE).toInt();
    StylePtrList list = RSingleton<StyleManager>::instance()->styles();
    if(switchIndex >= 0 && switchIndex < list.size()){
        updateStyle(switchIndex);
        RGlobal::G_GlobalConfigFile->systemConfigInfo.style = RSingleton<StyleManager>::instance()->currentStyle()->getStyleName();
        RGlobal::G_GlobalConfigFile->saveFile();
    }
}

void MainWindow::updateStyle(int index)
{
    RSingleton<StyleManager>::instance()->switchStyle(index);
}

/*!
 *  @brief 切换显示语言
 */
void MainWindow::switchLanguage()
{
    QString switchLan = QObject::sender()->property(Constant::SYSTEM_LANGUAGE).toString();
    updateLanguage(switchLan);
    RGlobal::G_GlobalConfigFile->systemConfigInfo.locale = switchLan;
    RGlobal::G_GlobalConfigFile->saveFile();
}

/*!
 * @brief 根据翻译文件名切换文件
 * @param[in]  lanFileName 要切换的文件名
 */
void MainWindow::updateLanguage(QString lanFileName)
{
    if(RSingleton<LanguageManager>::instance()->switchLanguage(lanFileName)){
        RSingleton<Core::Subject>::instance()->notify(MessageType::MESS_LAN_CHANGED);
    }
}

/*!
 * @brief 加载上一次的默认配置
 * @details 1.语言设置； @n
 *          2.样式表设置； @n
 *          3.从config/config.ini文件中查找是否有快捷键设置，若存在快捷键设置则使用 @n
 *          4.窗口设置(20181113) @n
 *          5.布局设置(待添加) @n
 */
void MainWindow::loadUserSetting()
{
    //[1] 语言设置
    if(RGlobal::G_GlobalConfigFile->systemConfigInfo.locale.isEmpty()){
        RGlobal::G_GlobalConfigFile->systemConfigInfo.locale = curLanguageName;
        RGlobal::G_GlobalConfigFile->saveFile();
    }else{
        Base::Language * currLan = RSingleton<LanguageManager>::instance()->findLan(RGlobal::G_GlobalConfigFile->systemConfigInfo.locale);
        if(currLan){
            currLan->getAction()->setChecked(true);
            updateLanguage(RGlobal::G_GlobalConfigFile->systemConfigInfo.locale);
        }
    }

    //[2] 样式表设置
    if(RGlobal::G_GlobalConfigFile->systemConfigInfo.style.isEmpty()){
        RGlobal::G_GlobalConfigFile->systemConfigInfo.style = RSingleton<StyleManager>::instance()->currentStyle()->getStyleName();
        RGlobal::G_GlobalConfigFile->saveFile();
    }else{
        Base::CustomStyle * currStyle = RSingleton<StyleManager>::instance()->findStyle(RGlobal::G_GlobalConfigFile->systemConfigInfo.style);
        if(currStyle){
            currStyle->getAction()->setChecked(true);
            updateStyle(currStyle->getAction()->property(Constant::CUSTOM_STYLE).toInt());
        }
    }

    //[3] 快捷键
    if(!RGlobal::G_GlobalConfigFile->systemConfigInfo.defaultKeySchemes){
        ProgramFilePath filePath;
        RXmlFile xmlfile(filePath.shortcutPath + QDir::separator() + RGlobal::G_GlobalConfigFile->systemConfigInfo.userKeySchemesName);
        ShortcutParseMethod * method = new ShortcutParseMethod();
        xmlfile.setParseMethod(method);
        if(xmlfile.startParse(QFile::ReadOnly)){
            ShortcutParseMethod::ShortcutMappingList list = method->getParsedList();
            ActionManager::ActionMap actMap = ActionManager::instance()->getAllActions();

            ActionManager::ActionMapIterator iter = actMap.constBegin();
            while(iter != actMap.constEnd()){
                QString id = const_cast<Id&>(iter.key()).data();
                if(id.indexOf(".") > 0 && id.split(".").size() == 2){
                    auto findIndex = std::find_if(list.constBegin(),list.constEnd(),[&id](const ShortcutMapping & item){
                        return item.id == id;
                    });
                    if(findIndex != list.constEnd()){
                        iter.value()->setDefaultKey(QKeySequence((*findIndex).keySequence));
                    }else{
                        iter.value()->setDefaultKey(QKeySequence());
                    }
                }
                iter++;
            }
        }else{
            QMessageBox::warning(this,tr("warning"),tr("User defined shortcut keys not available!"),QMessageBox::Yes);
        }
    }

    //[4]窗口设置
    windowTopHint(RGlobal::G_GlobalConfigFile->systemConfigInfo.topHint);
    ActionManager::instance()->action(Constant::TOP_HINT)->action()->setChecked(RGlobal::G_GlobalConfigFile->systemConfigInfo.topHint);

    windowFullScreen(RGlobal::G_GlobalConfigFile->systemConfigInfo.fullscreen);
    ActionManager::instance()->action(Constant::FULL_SCREEN)->action()->setChecked(RGlobal::G_GlobalConfigFile->systemConfigInfo.fullscreen);

    //[5]布局
}

void MainWindow::retranslateUi()
{
    serverMenu->menu()->setTitle(tr("Program(&P)"));
    exitAction->setText(tr("Exit(&X)"));

    viewMenu->menu()->setTitle(tr("View(&V)"));
    viewManagerMenu->menu()->setTitle(tr("View manager"));
    importViewAction->setText(tr("Import view"));
    exportViewAction->setText(tr("Export view"));

    settingsMenu->menu()->setTitle(tr("Settings(&S)"));
    topHintAction->setText(tr("Top hint"));
    fullScreenAction->setText(tr("Full screen"));

    styleMenu->menu()->setTitle(tr("Styles(&Y)"));
    lanMenu->menu()->setTitle(tr("Language(&L)"));
    shortcutAction->setText(tr("Shortcut settings(&T)"));
    screenshotAction->setText(tr("Screenshot(&S)"));

    helpMenu->menu()->setTitle(tr("Help(&H)"));
    supportAction->setText(tr("Technical support(&T)"));
    aboutPorgramAction->setText(tr("About program(&A)"));

    Core::PluginManager::ComponentMap maps = RSingleton<Core::PluginManager>::instance()->getAllActivePlugins();
    Core::PluginManager::ComponentMap::iterator iter = maps.begin();
    while(iter != maps.end()){
        Id id = iter.value()->id();
        Action * act = ActionManager::instance()->action(id);
        if(act){
            act->action()->setText(iter.value()->name());
        }

        QStringList slist = id.toString().split(".");
        QString menuId = slist.at(0)+"."+slist.at(1);
        char mid[128] = {0};
        memcpy(mid,menuId.toLocal8Bit().data(),menuId.toLocal8Bit().size());
        Action * module = ActionManager::instance()->action(mid);
        if(module){
            module->action()->setText(iter.value()->pluginName());
        }
        iter++;
    }
}

/*!
 * @brief 开启Menu透明背景
 * @param[in] menu 待修改的背景
 */
void MainWindow::setMenuTranslucent(QMenu *menu)
{
    menu->setWindowFlags(menu->windowFlags() | Qt::FramelessWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
}

void MainWindow::technicalSupport()
{

}

void MainWindow::aboutProgram()
{

}

/*!
 * @brief 显示快捷键设置窗口
 * @details 初始显示所有快捷键信息，按照插件分类显示；
 */
void MainWindow::showShortcutSettings()
{
    ShortcutSettings ss(this);
    ss.exec();
}

/*!
 *@brief 截屏功能
 */
void MainWindow::screenshotSettings()
{
    QString saveName = QFileDialog::getSaveFileName(this,tr("save screenshot"),QDir::homePath(),QString("*.jpg"));
    if(!saveName.isEmpty()){
        QEventLoop eventloop;
        QTimer::singleShot(50, &eventloop, SLOT(quit()));
        eventloop.exec();

        QScreen *screen = QGuiApplication::primaryScreen();
        QString filePathName = saveName;

        if(!screen->grabWindow(0).save(filePathName, "jpg"))
        {
            QMessageBox::warning(this,QCoreApplication::tr("warning"),tr("save screenshot failed!"));
        }else{
            QMessageBox::information(this,tr("information"),tr("save screenshot successfully!"));
        }
    }
}

/*!
 * @brief 导入布局
 * @details 根据ObjName查找widget，则从文件中将信息设置回
 */
void MainWindow::importView()
{
    Core::LayoutParseMethod method;
    ProgramFilePath filePath;
    Base::RTextFile file(filePath.layoutFile);
    file.setParseMethod(&method,false);
    if(file.startParse()){
        QList<Core::LayoutItem> items = method.getItems();
        Core::Widget::WidgetMap maps = Core::Widget::getAllWidgets();
        std::for_each(items.begin(),items.end(),[&](Core::LayoutItem & item){
            Core::Widget * tmpWidget = maps.value(item.objName);
            tmpWidget->setWidgetFeatures(static_cast<Core::Widget::WidgetFeature>(item.feature));
            tmpWidget->setGeometry(item.geometry);
        });
    }
}

/*!
 * @brief 保存布局
 * @details 保存内容：1.objname；2.WidgetFeatures; 3.Geometry @n
 *          保存路径：config/layout.bin
 *          保存范围：已激活使用的控件；上部菜单栏；右侧图层控制兰
 */
void MainWindow::exportView(bool flag)
{
    Core::LayoutParseMethod method;
    ProgramFilePath filePath;
    Base::RTextFile file(filePath.layoutFile);
    file.setParseMethod(&method,false);
    Core::LayoutItem item;

    Core::Widget::WidgetMap maps = Core::Widget::getAllWidgets();
    Core::Widget::WidgetMap::iterator iter = maps.begin();
    while(iter != maps.end()){
        item.objName = (*iter)->objectName();
        item.geometry = (*iter)->geometry();
        item.feature = static_cast<int>((*iter)->getWidgetFeatures());
        method.addItem(item);
        iter++;
    }

    //TODO 可增加状态显示栏，减少弹窗使用
    bool result = file.startSave(QFile::WriteOnly);
    if(!flag){
        if(result){
            QMessageBox::information(this,tr("information"),tr("export layout successfully!"));
        }else{
            QMessageBox::warning(this,tr("warning"),tr("export layout failed!"));
        }
    }
}

/*!
 * @brief 加载可用的所有模块
 * @warning 每个子插件需要在类中定义pluginId,确保每个插件的id不一样.
 */
void MainWindow::loadCmponent()
{
    //TODO 20180824 待从各个dll中读取
    TaskControlModel::TaskControlPanel * taskControl = new TaskControlModel::TaskControlPanel;
    HealthInfoDockPanel *healthControl = new HealthInfoDockPanel;

    DataView::RadiationSourceTable * radiationTable = new DataView::RadiationSourceTable;
    DataView::AllPluseDock * allPluseTable = new DataView::AllPluseDock;
//    DataView::MFAcquistionTable * acquistionTable = new DataView::MFAcquistionTable;
    DataView::RadiaSourceMap * radiaSourceMap = new DataView::RadiaSourceMap;
    DataView::AllPluseGraphics * allPluseGraphics = new DataView::AllPluseGraphics;
    DataView::MFAcquisitionGraphics * mfGraphics = new DataView::MFAcquisitionGraphics;

    RSingleton<Core::PluginManager>::instance()->addAvailblePlugin(taskControl);
    RSingleton<Core::PluginManager>::instance()->addAvailblePlugin(healthControl);

    RSingleton<Core::PluginManager>::instance()->addAvailblePlugin(radiationTable);
    RSingleton<Core::PluginManager>::instance()->addAvailblePlugin(allPluseTable);
//    RSingleton<Core::PluginManager>::instance()->addAvailblePlugin(acquistionTable);
    RSingleton<Core::PluginManager>::instance()->addAvailblePlugin(radiaSourceMap);

    RSingleton<Core::PluginManager>::instance()->addAvailblePlugin(allPluseGraphics);
    RSingleton<Core::PluginManager>::instance()->addAvailblePlugin(mfGraphics);
}

/*!
 * @brief   初始化各个组件
 * @details 1.加载可用的插件 @n
 *          2.解析plugins.xml文件 @n
 *          3.初始化网路接收、网络解析模块 @n
 *          4.初始化配置的模块
 */
void MainWindow::initComponent()
{
    setDockNestingEnabled(true);

    //【0】设置主窗口信息
    if(!RGlobal::G_GlobalConfigFile->systemConfigInfo.menubarVisible){
        menuBar()->hide();
    }

    mapView = new MapView(centralWidget());
    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(mapView);
    centralWidget()->setLayout(mainLayout);

    loadCmponent();

    //加载配置
    RSingleton<Core::PluginLoader>::instance()->initConfigFile();

    //【1】实例化网络
    Core::NetworkMap * networkMap = RSingleton<Core::PluginLoader>::instance()->getNetworks();
    Core::NetworkMap::iterator niter = networkMap->begin();
    while(niter != networkMap->end()){
        Datastruct::NetworkInfo ninfo = niter.value();
        if(ninfo.protocol == Datastruct::N_TCP && ninfo.baseInfo.connectionType == Datastruct::N_Server){
            std::shared_ptr<Core::TcpServer> server(new Core::TcpServer());
            server->init(ninfo);
            server->startMe();
            RSingleton<Core::TaskManager>::instance()->addTask(ninfo.id,server);
        }
        niter++;
    }

    //【1.1】网络解析
    RSingleton<Core::ProtocolParseThread>::instance();

    //【2】实例化模块
    Core::ModuleMap * moduleMap = RSingleton<Core::PluginLoader>::instance()->getModules();
    Core::ModuleMap::iterator miter = moduleMap->begin();
    while(miter != moduleMap->end()){
        Datastruct::ModuleInfo mm = miter.value();

        Core::RComponent * plugin = RSingleton<Core::PluginManager>::instance()->getAvailblePlugin(mm.pluginId);
        if(plugin){
            //2.1 向网络接收模块注册需要数据信息
            Core::TaskPtr tptr = RSingleton<Core::TaskManager>::instance()->getTask(mm.networkId);
            if(tptr){
                std::shared_ptr<Core::TcpServer> tcpPtr = std::dynamic_pointer_cast<Core::TcpServer>(tptr);
                if(tcpPtr){
                    tcpPtr->registNetworkObserver(mm.pluginId,mm.protocols);
                    //2.2 向网络解析模块注册需要解析的协议
                    RSingleton<Core::ProtocolParseThread>::instance()->registNetworkObserver(mm.pluginId,mm.protocols,Datastruct::N_TCP);
                }
            }

            plugin = plugin->clone();
            plugin->setName(mm.name);
            Core::MyDockWidget * dockContainer = new Core::MyDockWidget(this);
            dockContainer->setGeometry(mm.geometry);
            plugin->setDockContainer(dockContainer);

            RSingleton<Core::PluginManager>::instance()->addActivePlugin(plugin);
        }
        miter++;
    }

    RSingleton<Core::ProtocolParseThread>::instance()->startMe();

    RSingleton<Core::PluginManager>::instance()->load();
    Core::PluginManager::ComponentMap maps = RSingleton<Core::PluginManager>::instance()->getAllActivePlugins();

    Core::PluginManager::ComponentMap::iterator iter = maps.begin();
    while(iter != maps.end()){
        Core::RComponent * comp = iter.value();
        Core::MyDockWidget * dockContainer = comp->getDockContainer();
        dockContainer->setWidgetFeatures(Core::Widget::AllWidgetFeatures);
        dockContainer->setFloating(true);
        QWidget * widget = comp->initialize(dockContainer);
        if(widget){
            dockContainer->setWidget(widget);
            dockContainer->setObjectName(comp->objectName());
            dockContainer->setTitle(comp->name());
        }

        //将dock中控制显隐的action添加至菜单栏
        if(dockContainer->toggleViewAction()){
            QStringList slist = comp->id().toString().split(".");
            QString menuId = slist.at(0)+"."+slist.at(1);
            ActionContainer * container = ActionManager::instance()->actionContainer(Constant::MENU_VIEW);
            if(container){
                char mid[128] = {0};
                memcpy(mid,menuId.toLocal8Bit().data(),menuId.toLocal8Bit().size());
                ActionContainer * moduleMenu = ActionManager::instance()->actionContainer(mid);
                if(moduleMenu == NULL){
                    QAction * moduleAction = new QAction();
                    moduleAction->setText(comp->pluginName());
                    Action * module = ActionManager::instance()->registAction(mid,moduleAction);
                    container->appendGroup(mid);
                    container->addAction(module,mid);

                    moduleMenu = ActionManager::instance()->createMenu(mid);
                    setMenuTranslucent(moduleMenu->menu());
                    moduleMenu->appendGroup(mid);

                    moduleAction->setMenu(moduleMenu->menu());
                }
                Action * toggleAction = ActionManager::instance()->registAction(comp->id(),dockContainer->toggleViewAction());
                moduleMenu->addAction(toggleAction,mid);
            }
        }
        iter++;
    }

    //初始化控制面板、标题栏
    settingBar = new Core::ModuleSetting(this);
    QRect screenGeometry = RUtil::screenGeometry();
    int width = 300;
    int height = 620;
    settingBar->setGeometry(screenGeometry.width() - width,160,width,height);

    height = 80;
    if(!RGlobal::G_GlobalConfigFile->systemConfigInfo.menubarVisible){
        titleBar = new Core::TitleBar(this);
        titleBar->setGeometry(0,0,screenGeometry.width(),height);
        titleBar->addMenu(QStringLiteral("图层(&P)"),QString(Constant::MENU_VIEW),QPixmap(":/tech/resource/technology/system_setting.png"));
        titleBar->addMenu(QStringLiteral("工具(&T)"),QString(Constant::MENU_PROGRAM),QPixmap(":/tech/resource/technology/system_setting.png"));
        titleBar->addMenu(QStringLiteral("设置(&S)"),QString(Constant::MENU_SETTING),QPixmap(":/tech/resource/technology/system_setting.png"));
        titleBar->addMenu(QStringLiteral("帮助(&Ha)"),QString(Constant::MENU_HELP),QPixmap(":/tech/resource/technology/system_setting.png"));
        titleBar->setTitle(QStringLiteral("XX型数据显控系统"));
    }
}

void MainWindow::displayResize(){
    importView();
}
