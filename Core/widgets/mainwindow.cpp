#include "mainwindow.h"
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
#include "Base/pluginmanager/pluginmanager.h"
#include "Base/constants.h"
#include "Base/util/rsingleton.h"
#include "Base/common/stylemanager.h"
#include "Base/common/languagemanager.h"
#include "Base/pluginmanager/subject.h"
#include "Base/util/rlog.h"
#include "Base/util/fileutils.h"
#include "healthmanage/healthinfopannel.h"

#include "datadisplay/radiationsourcetable.h"
#include "datadisplay/allplusetable.h"
#include "datadisplay/mfacquistiontable.h"
#include "datadisplay/radiasourcemap.h"
#include "datadisplay/allplusegraphics.h"
#include "datadisplay/mfacquisitiongraphics.h"
#include "datadisplay/spectrumgraphics.h"

#include "global.h"
#include "file/globalconfigfile.h"
#include "file/programfilepath.h"
#include "widgets/taskcontrol/taskcontrolpanel.h"
#include "shortcutsettings.h"

using namespace Base;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle(tr("ViewFrame"));

    //TODO 20180915 待添加对配置路径下文件搜索
    RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Technology style"),":/resource/style/Technology.qss",true));
    RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Dark style"),":/resource/style/Black.qss",false));
    RSingleton<StyleManager>::instance()->addStyle(new CustomStyle(tr("Light style"),":/resource/style/White.qss",false));

    RSingleton<Subject>::instance()->attach(this);

    initMenu();
    initComponent();

    updateLanguage(curLanguageName);

    loadUserSetting();
    showMaximized();
}

MainWindow::~MainWindow()
{
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
    menubar->appendGroup(Constant::MENU_PROGRAM);
    menubar->appendGroup(Constant::MENU_VIEW);
    menubar->appendGroup(Constant::MENU_SETTING);
    menubar->appendGroup(Constant::MENU_HELP);

    QAction * coreAction = new QAction();
    coreAction->setVisible(false);
    ActionManager::instance()->registAction(Constant::PLUGIN_CORE,coreAction);

    //程序菜单
    serverMenu = ActionManager::instance()->createMenu(Constant::MENU_PROGRAM);
    serverMenu->appendGroup(Constant::MENU_PROGRAM);
    menubar->addMenu(serverMenu, Constant::MENU_PROGRAM);

    exitAction = new QAction(this);
    connect(exitAction,SIGNAL(triggered()),this,SLOT(programExit()));
    Action * serverDelete = ActionManager::instance()->registAction(Constant::PROGRAM_EXIT,exitAction);
    serverDelete->setDefaultKey(QKeySequence("Ctrl+Q"));

    serverMenu->addAction(serverDelete,Constant::MENU_PROGRAM);

    //视图菜单
    viewMenu = ActionManager::instance()->createMenu(Constant::MENU_VIEW);
    viewMenu->appendGroup(Constant::MENU_VIEW);
    menubar->addMenu(viewMenu, Constant::MENU_VIEW);

    viewManagerMenu = ActionManager::instance()->createMenu(Constant::VIEW_MANAGER);
    viewManagerMenu->appendGroup(Constant::VIEW_MANAGER);
    viewMenu->addMenu(viewManagerMenu,Constant::VIEW_MANAGER);

    importViewAction = new QAction(this);
    connect(importViewAction,SIGNAL(triggered()),this,SLOT(importView()));
    Action * importView = ActionManager::instance()->registAction(Constant::IMPORT_VIEW,importViewAction);
    importView->setDefaultKey(QKeySequence("Ctrl+Shift+I"));
    viewManagerMenu->addAction(importView,Constant::VIEW_MANAGER);

    exportViewAction = new QAction(this);
    connect(exportViewAction,SIGNAL(triggered()),this,SLOT(exportView()));
    Action * exportView = ActionManager::instance()->registAction(Constant::EXPORT_VIEW,exportViewAction);
    exportView->setDefaultKey(QKeySequence("Ctrl+Shift+E"));
    viewManagerMenu->addAction(exportView,Constant::VIEW_MANAGER);

    viewMenu->addMenu(viewManagerMenu,Constant::MENU_VIEW);
    viewMenu->addSeparator(Constant::MENU_VIEW);

    //设置菜单
    settingsMenu = ActionManager::instance()->createMenu(Constant::MENU_SETTING);
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


    //帮助菜单
    helpMenu = ActionManager::instance()->createMenu(Constant::MENU_HELP);
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

/*!
 * @brief   程序退出
 */
void MainWindow::programExit()
{
    exit(0);
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
}

void MainWindow::windowFullScreen(bool)
{
    Qt::WindowStates state = windowState();
    if(!isFullScreen())
        state |= Qt::WindowFullScreen;
    else{
        state = state & ~Qt::WindowFullScreen;
        showMaximized();
    }

    setWindowState(state);
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
        RSingleton<Subject>::instance()->notify(MessageType::MESS_LAN_CHANGED);
    }
}

/*!
 * @brief 加载上一次的默认配置
 * @details 1.语言设置； @n
 *          2.样式表设置； @n
 *          3.从config/config.ini文件中查找是否有快捷键设置，若存在快捷键设置则使用 @n
 *          4.布局设置(待添加) @n
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
        if(xmlfile.startParse()){
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

    helpMenu->menu()->setTitle(tr("Help(&H)"));
    supportAction->setText(tr("Technical support(&T)"));
    aboutPorgramAction->setText(tr("About program(&A)"));

    PluginManager::ComponentMap maps = RSingleton<PluginManager>::instance()->plugins();
    PluginManager::ComponentMap::iterator iter = maps.begin();
    while(iter != maps.end()){
        Id id = iter.value()->id();
        Action * act = ActionManager::instance()->action(id);
        if(act)
            act->action()->setText(iter.value()->name());

        QStringList slist = id.toString().split(".");
        QString menuId = slist.at(0)+"."+slist.at(1);
        char mid[128] = {0};
        memcpy(mid,menuId.toLocal8Bit().data(),menuId.toLocal8Bit().size());
        Action * module = ActionManager::instance()->action(mid);
        if(module)
            module->action()->setText(iter.value()->pluginName());
        iter++;
    }
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

void MainWindow::importView()
{

}

void MainWindow::exportView()
{

}

/*!
 * @brief   初始化各个组件
 */
void MainWindow::initComponent()
{
    setDockNestingEnabled(true);
    QWidget * centeral = takeCentralWidget();
    if (centeral)
        delete centeral;

    //TODO 20180824 待从各个dll中读取
    TaskControlModel::TaskControlPanel * taskControl = new TaskControlModel::TaskControlPanel;
    HealthInfoDockPanel *healthControl = new HealthInfoDockPanel;

    //数据显示模块，每个页面都是dock
    DataView::RadiationSourceTable * radiationTable = new DataView::RadiationSourceTable;
    DataView::AllPluseTable * allPluseTable = new DataView::AllPluseTable;
    DataView::MFAcquistionTable * acquistionTable = new DataView::MFAcquistionTable;
    DataView::RadiaSourceMap * radiaSourceMap = new DataView::RadiaSourceMap;
    DataView::AllPluseGraphics * allPluseGraphics = new DataView::AllPluseGraphics;
    DataView::MFAcquisitionGraphics * mfGraphics = new DataView::MFAcquisitionGraphics;
    DataView::SpectrumGraphics * spectrumGraphics = new DataView::SpectrumGraphics;

    addDockWidget(Qt::LeftDockWidgetArea,taskControl);
    splitDockWidget(taskControl,healthControl,Qt::Vertical);

    addDockWidget(Qt::RightDockWidgetArea,radiationTable);
    tabifyDockWidget(radiationTable,allPluseTable);
    tabifyDockWidget(allPluseTable,acquistionTable);
    tabifyDockWidget(acquistionTable,radiaSourceMap);
    tabifyDockWidget(radiaSourceMap,allPluseGraphics);
    tabifyDockWidget(allPluseGraphics,mfGraphics);
    tabifyDockWidget(mfGraphics,spectrumGraphics);

    radiationTable->raise();

    RSingleton<PluginManager>::instance()->addPlugin(taskControl);
    RSingleton<PluginManager>::instance()->addPlugin(healthControl);

    RSingleton<PluginManager>::instance()->addPlugin(radiationTable);
    RSingleton<PluginManager>::instance()->addPlugin(allPluseTable);
    RSingleton<PluginManager>::instance()->addPlugin(acquistionTable);
    RSingleton<PluginManager>::instance()->addPlugin(radiaSourceMap);
    RSingleton<PluginManager>::instance()->addPlugin(allPluseGraphics);
    RSingleton<PluginManager>::instance()->addPlugin(mfGraphics);
    RSingleton<PluginManager>::instance()->addPlugin(spectrumGraphics);

    RSingleton<PluginManager>::instance()->load();
    PluginManager::ComponentMap maps = RSingleton<PluginManager>::instance()->plugins();

    PluginManager::ComponentMap::iterator iter = maps.begin();
    while(iter != maps.end()){
        RComponent * comp = iter.value();
        comp->setFeatures(QDockWidget::AllDockWidgetFeatures);
//        comp->setFloating(true);
        comp->initialize();

        if(comp->toggleViewAction()){
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
                    moduleMenu->appendGroup(mid);

                    moduleAction->setMenu(moduleMenu->menu());
                }
                Action * toggleAction = ActionManager::instance()->registAction(comp->id(),comp->toggleViewAction());
                moduleMenu->addAction(toggleAction,mid);
            }
        }

        iter++;
    }
}
