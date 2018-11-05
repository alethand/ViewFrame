#include <QAction>

#include "datadisplay.h"

#include "base/actionmanager/action.h"
#include "base/actionmanager/actionmanager.h"
#include "base/actionmanager/actioncontainer.h"
#include "Base/constants.h"
#include "Base/util/rsingleton.h"

const char ACTION_TYPE_PROPERTY [] = "DataView.ActionType";

class DataDisplayPrivate
{
    Q_DECLARE_PUBLIC(DataDisplay)
private:
    DataDisplayPrivate(DataDisplay * q):q_ptr(q){
        initView();
    }

    void initView();
    void initAction();

    DataDisplay * q_ptr;

    ActionContainer * dataViewMenu;
    QAction * radiaSourceAction;
    QAction * allPluseAction;
    QAction * mfAcquisitionAction;
    QAction * radiaSourceMapAction;
    QAction * allPluseGraphicsAction;
    QAction * mfAcquisitionGraphicsAction;
    QAction * spectrumGraphicsAction;
};

void DataDisplayPrivate::initView()
{

//    spectrumGraphics=new SpectrumGraphics(q_ptr);  //频谱数据图形
//    spectrumGraphics->hide();
}

/*!
 * @brief 初始化按钮
 */
void DataDisplayPrivate::initAction()
{
//    ActionContainer *menubar = ActionManager::instance()->createMenuBar(Constant::MENU_BAR);
//    menubar->insertGroup(Constant::G_HELP,Constant::DATA_VIEW);

//    dataViewMenu = ActionManager::instance()->createMenu(Constant::DATA_VIEW);
//    dataViewMenu->appendGroup(Constant::DATA_VIEW);
//    menubar->addMenu(dataViewMenu, Constant::DATA_VIEW);

//    radiaSourceAction = new QAction();
//    radiaSourceAction->setProperty(ACTION_TYPE_PROPERTY,Datastruct::TABLE_RADIASOURCE);
//    QObject::connect(radiaSourceAction,SIGNAL(triggered(bool)),q_ptr,SLOT(switchDockVisible(bool)));
//    Action * radiaSourceTable = ActionManager::instance()->registAction(Constant::RADIASOURCETABLE_SETTING,radiaSourceAction);
//    dataViewMenu->addAction(radiaSourceTable,Constant::DATA_VIEW);

//    allPluseAction = new QAction();
//    allPluseAction->setProperty(ACTION_TYPE_PROPERTY,Datastruct::TABLE_ALLPLUSE);
//    QObject::connect(allPluseAction,SIGNAL(triggered(bool)),q_ptr,SLOT(switchDockVisible(bool)));
//    Action * allPluseTable = ActionManager::instance()->registAction(Constant::ALLPLUSETABLE_SETTING,allPluseAction);
//    dataViewMenu->addAction(allPluseTable,Constant::DATA_VIEW);

//    mfAcquisitionAction = new QAction();
//    mfAcquisitionAction->setProperty(ACTION_TYPE_PROPERTY,Datastruct::TABLE_MFACQUISITION);
//    QObject::connect(mfAcquisitionAction,SIGNAL(triggered(bool)),q_ptr,SLOT(switchDockVisible(bool)));
//    Action * mfAcquisitionTable = ActionManager::instance()->registAction(Constant::MFACQUISITIONTABLE_SETTING,mfAcquisitionAction);
//    dataViewMenu->addAction(mfAcquisitionTable,Constant::DATA_VIEW);

//    radiaSourceMapAction = new QAction();
//    radiaSourceMapAction->setProperty(ACTION_TYPE_PROPERTY,Datastruct::MAP_RADIASOURCE);
//    QObject::connect(radiaSourceMapAction,SIGNAL(triggered(bool)),q_ptr,SLOT(switchDockVisible(bool)));
//    Action * radiaSourceMapActionTable = ActionManager::instance()->registAction(Constant::RADIASOURCEMAP_SETTING,radiaSourceMapAction);
//    dataViewMenu->addAction(radiaSourceMapActionTable,Constant::DATA_VIEW);

//    allPluseGraphicsAction = new QAction();
//    allPluseGraphicsAction->setProperty(ACTION_TYPE_PROPERTY,Datastruct::GRAPHICSE_ALLPLUSE);
//    QObject::connect(allPluseGraphicsAction,SIGNAL(triggered(bool)),q_ptr,SLOT(switchDockVisible(bool)));
//    Action * allPluseGraphicsActionTable = ActionManager::instance()->registAction(Constant::ALLPLUSEGRAPHICS_SETTING,allPluseGraphicsAction);
//    dataViewMenu->addAction(allPluseGraphicsActionTable,Constant::DATA_VIEW);

//    mfAcquisitionGraphicsAction = new QAction();
//    mfAcquisitionGraphicsAction->setProperty(ACTION_TYPE_PROPERTY,Datastruct::GRAPHICSE_MFACQUISITION);
//    QObject::connect(mfAcquisitionGraphicsAction,SIGNAL(triggered(bool)),q_ptr,SLOT(switchDockVisible(bool)));
//    Action * mfAcquisitionGraphicsActionTable = ActionManager::instance()->registAction(Constant::MFACQUISITIONGRAPHICS_SETTING,mfAcquisitionGraphicsAction);
//    dataViewMenu->addAction(mfAcquisitionGraphicsActionTable,Constant::DATA_VIEW);

//    spectrumGraphicsAction = new QAction();
//    spectrumGraphicsAction->setProperty(ACTION_TYPE_PROPERTY,Datastruct::GRAPHICSE_SPECTRUM);
//    QObject::connect(spectrumGraphicsAction,SIGNAL(triggered(bool)),q_ptr,SLOT(switchDockVisible(bool)));
//    Action * spectrumGraphicsActionTable = ActionManager::instance()->registAction(Constant::SPECTRUMGRAPHICS_SETTING,spectrumGraphicsAction);
//    dataViewMenu->addAction(spectrumGraphicsActionTable,Constant::DATA_VIEW);
}

DataDisplay::DataDisplay(QWidget *parent) :
    QWidget(parent),d_ptr(new DataDisplayPrivate(this))
{
    RSingleton<Base::Subject>::instance()->attach(this);

    retranslateUi();
}

DataDisplay::~DataDisplay()
{
}

void DataDisplay::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

/*!
 * @brief 切换显示dock
 * @param[in] flag 显示或隐藏指定dock
 */
void DataDisplay::switchDockVisible(bool flag)
{
//    DataDisplayTableType type = static_cast<DataDisplayTableType>(QObject::sender()->property(ACTION_TYPE_PROPERTY).toInt());
//    switch(type){
//        default:
//            break;
//    }
}

void DataDisplay::retranslateUi()
{
    Q_D(DataDisplay);
    d->dataViewMenu->menu()->setTitle(tr("DataDisplay panel"));

    d->radiaSourceAction->setText(tr("List of data sources"));
    d->allPluseAction->setText(tr("Full Pulse List"));
    d->mfAcquisitionAction->setText(tr("Intermediate Frequency Data List"));
    d->radiaSourceMapAction->setText(tr("Data source Map"));
    d->allPluseGraphicsAction->setText(tr("Full Pulse graphics information"));
    d->mfAcquisitionGraphicsAction->setText(tr("Intermediate Frequency Data Graphics"));
    d->spectrumGraphicsAction->setText(tr("Spectral data graph"));
}
