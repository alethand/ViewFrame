#include "modulesetting.h"

#include <QTreeView>
#include <QHBoxLayout>
#include <QAction>
#include <QHeaderView>

#include "Base/util/rsingleton.h"
#include "pluginmanager/pluginmanager.h"
#include "pluginmanager/rcomponent.h"
#include "Base/selfwidget/rbutton.h"

namespace Core{

MSettingModel::MSettingModel(QObject *parent):QAbstractItemModel(parent)
{
    rootNode = new TreeNode;

    Core::PluginManager::ComponentMap maps = RSingleton<Core::PluginManager>::instance()->getAllActivePlugins();
    Core::PluginManager::ComponentMap::iterator iter = maps.begin();
    while(iter != maps.end()){
        Core::RComponent * comp = iter.value();
        QStringList slist = comp->id().toString().split(".");
        QString menuId = slist.at(0)+"."+slist.at(1);

        TreeNode * existedNode = NULL;
        auto findex = std::find_if(rootNode->nodes.begin(),rootNode->nodes.end(),[&](TreeNode * node){
            return node->nodeId == menuId;
        });

        if(findex == rootNode->nodes.end()){
            existedNode = new TreeNode;
            existedNode->index = rootNode->nodes.size();
            existedNode->nodeId = menuId;
            existedNode->nodeName = comp->pluginName();
            existedNode->parentNode = NULL;
            rootNode->nodes.push_back(existedNode);
        }else{
            existedNode = *findex;
        }

        TreeNode * childNode = new TreeNode;
        childNode->nodeId = comp->id().toString();
        childNode->nodeName = comp->getDockContainer()->toggleViewAction()->text();
        childNode->parentNode = existedNode;
        childNode->index = existedNode->nodes.size();
        existedNode->nodes.push_back(childNode);
        iter++;
    }

    headList<<(QObject::tr("Plugin Name"));
}

MSettingModel::~MSettingModel()
{

}

QVariant MSettingModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeNode *item = static_cast<TreeNode*>(index.internalPointer());

    switch(role){
        case Qt::DisplayRole:
            return item->nodeName;
            break;
        case Qt::CheckStateRole:
            return item->visible;
            break;
        default:
            break;
    }
    return QVariant();
}

Qt::ItemFlags MSettingModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    if(index.column() == 0)
         return flags | Qt::ItemIsUserCheckable;

    return flags;
}

QVariant MSettingModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation == Qt::Horizontal && section == 0){
        switch (role){
            case  Qt::DisplayRole:
                return headList.at(section);
                break;
            case  Qt::BackgroundColorRole:
                return QColor(0,255,0,50);
                break;
            default:
                break;
        }
    }
    return QVariant();
}

QModelIndex MSettingModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TreeNode * parentItem;

    if (!parent.isValid())
        parentItem = rootNode;
    else
        parentItem = static_cast<TreeNode*>(parent.internalPointer());

    TreeNode * childItem = parentItem->nodes.at(row);

    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex MSettingModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeNode *childItem = static_cast<TreeNode*>(index.internalPointer());
    TreeNode *parentItem = childItem->parentNode;

    if (parentItem == rootNode || parentItem == NULL)
        return QModelIndex();

    return createIndex(parentItem->index, 0, parentItem);
}

int MSettingModel::rowCount(const QModelIndex &parent) const
{
    TreeNode *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootNode;
    else
        parentItem = static_cast<TreeNode*>(parent.internalPointer());

    return parentItem->nodes.size();
}

int MSettingModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

class ModuleSettingPrivate
{
    Q_DECLARE_PUBLIC(ModuleSetting)
public:
    ModuleSettingPrivate(ModuleSetting * q):q_ptr(q){
        init();
    }

    void init();
    void retranslateUi();

    QWidget * mainWidget;
    ModuleSetting* q_ptr;

    QTreeView * treeView;

    RButton * sysSettingButt;
    RButton * networkButt;

    MSettingModel * model;
};

void ModuleSettingPrivate::init()
{
    mainWidget = new QWidget(q_ptr);
    mainWidget->setObjectName("mainWidget");

    model = new MSettingModel;

    treeView = new QTreeView(mainWidget);
    treeView->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    treeView->setFocusPolicy(Qt::TabFocus);
    treeView->header()->hide();
    treeView->setModel(model);
    treeView->expandAll();

    QWidget * toolWidget = new QWidget(mainWidget);
    toolWidget->setFixedHeight(32);

    sysSettingButt = new RButton(toolWidget);
    networkButt = new RButton(toolWidget);

    QHBoxLayout * toolLayout = new QHBoxLayout;
    toolLayout->setContentsMargins(1,1,1,1);
    toolLayout->addStretch(1);
    toolLayout->addWidget(sysSettingButt);
    toolLayout->addWidget(networkButt);
    toolLayout->addStretch(1);
    toolWidget->setLayout(toolLayout);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(2,2,2,2);
    mainLayout->addWidget(treeView);
    mainLayout->addWidget(toolWidget);

    mainWidget->setLayout(mainLayout);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(1,1,1,1);
    layout->addWidget(mainWidget);
    q_ptr->setLayout(layout);
}

void ModuleSettingPrivate::retranslateUi()
{
    sysSettingButt->setText(QObject::tr("System setting"));
    networkButt->setText(QObject::tr("Network setting"));

}

ModuleSetting::ModuleSetting(QWidget *parent):Widget(parent),d_ptr(new ModuleSettingPrivate(this))
{
    RSingleton<Core::Subject>::instance()->attach(this);
}

ModuleSetting::~ModuleSetting()
{

}

QSize ModuleSetting::sizeHint() const
{
    return QSize(310,450);
}

void ModuleSetting::onMessage(MessageType::MessType type)
{
    Q_D(ModuleSetting);
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            d->retranslateUi();
            break;
        default:
            break;
    }
}

}
