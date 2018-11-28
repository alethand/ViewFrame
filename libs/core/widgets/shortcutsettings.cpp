#include "shortcutsettings.h"

#include <QLineEdit>
#include <QGroupBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QGroupBox>
#include <QShortcut>
#include <QKeyEvent>
#include <QHeaderView>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QlistView>

#include "Base/selfwidget/rbutton.h"
#include "Base/actionmanager/actionmanager.h"
#include "Base/actionmanager/action.h"
#include "file/programfilepath.h"
#include "file/globalconfigfile.h"
#include <core/global.h>

#define LIST_ITEM_LEN 3

namespace Core{

class ShortcutKey{
public:
    ShortcutKey():root("mapping"),nodeName("shortcut"),nodeKey("key"),nodeValue("value"){}

    QString root;
    QString nodeName;
    QString nodeKey;
    QString nodeValue;
};

bool ShortcutParseMethod::startParse(QDomNode &rootNode)
{
    ShortcutKey shortcut;
    QDomNodeList nodeList = rootNode.toElement().elementsByTagName(shortcut.nodeName);
    for(int i = 0;i < nodeList.size() ; i++){
        QDomElement ele = nodeList.at(i).toElement();
        ShortcutMapping mapping{ele.attribute(shortcut.nodeKey),ele.attribute(shortcut.nodeValue)};
        mappings.push_back(mapping);
    }
    return true;
}

bool ShortcutParseMethod::startSave(QDomDocument &doc)
{
    ShortcutKey shortcut;
    ActionManager::ActionMap actMap = ActionManager::instance()->getAllActions();
    ActionManager::ActionMapIterator iter = actMap.constBegin();
    QDomElement root = doc.createElement(shortcut.root);
    doc.appendChild(root);
    while(iter != actMap.constEnd()){
        QString id = const_cast<Id &>(iter.key()).data();
        if(id.indexOf(".") > 0 && id.split(".").size() == LIST_ITEM_LEN){
            QDomElement element = doc.createElement(shortcut.nodeName);
            element.setAttribute(shortcut.nodeKey,id);
            element.setAttribute(shortcut.nodeValue,iter.value()->action()->shortcut().toString(QKeySequence::NativeText));
            root.appendChild(element);
        }
        iter++;
    }

    return true;
}

class ShortcutSettingsPrivate
{
    Q_DECLARE_PUBLIC(ShortcutSettings)
private:
    ShortcutSettingsPrivate(ShortcutSettings * q):q_ptr(q),keyPressCount(0),currSchemeModified(false){
        memset(key,0,sizeof(key));
        initView();
    }

    void initView();

    ShortcutSettings * q_ptr;

    QWidget * mainWidget;

    QTreeWidget * treeWidget;

    RButton * resetAllButt;
    RButton * importButt;
    RButton * exportButt;

    QComboBox * schemesBox;

    QGroupBox * keyGroupBox;
    QLabel * keySequenceLabel;
    QLineEdit * targetEdit;

    RButton * resetButt;

    int key[4];             /*!< 设置快捷键时储存已经按下的键 */
    ushort keyPressCount;   /*!< 一次设置快捷键时，按下键的数量，最多为4次 */
    bool currSchemeModified;        /*!< 当前快捷信息是否被更新 */
    QMap<QString,QTreeWidgetItem *> shortcutItems;
};

void ShortcutSettingsPrivate::initView()
{
    mainWidget = new QWidget();

    treeWidget = new QTreeWidget(mainWidget);
    QObject::connect(treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)),q_ptr,SLOT(respItemClicked(QTreeWidgetItem*,int)));

    QWidget * buttContainer = new QWidget();

    resetAllButt = new RButton(buttContainer);
    QObject::connect(resetAllButt,SIGNAL(pressed()),q_ptr,SLOT(resetAllShortcuts()));

    schemesBox = new QComboBox(buttContainer);
    schemesBox->setView(new QListView());
    schemesBox->addItem(QObject::tr("Default scheme"));

    ProgramFilePath filePath;
    QDir dir(filePath.shortcutPath);
    QStringList suffixFilter;
    suffixFilter<<"*.xml";
    QFileInfoList fileInfoList = dir.entryInfoList(suffixFilter);

    int selectedIndex = -1;
    for(int i = 0;i< fileInfoList.size();i++){
        QStringList fileNames = fileInfoList.at(i).fileName().split(".");
        if(fileNames.size() == 2){
            schemesBox->addItem(fileNames.at(0),fileInfoList.at(i).fileName());
            if(!RGlobal::G_GlobalConfigFile->systemConfigInfo.defaultKeySchemes && fileInfoList.at(i).fileName() ==
                    RGlobal::G_GlobalConfigFile->systemConfigInfo.userKeySchemesName){
                selectedIndex = i;
            }
        }
    }
    schemesBox->setCurrentIndex(selectedIndex + 1);
    QObject::connect(schemesBox,SIGNAL(activated(int)),q_ptr,SLOT(switchSchemes(int)));

    importButt = new RButton(buttContainer);
    QObject::connect(importButt,SIGNAL(pressed()),q_ptr,SLOT(importShortcuts()));

    exportButt = new RButton(buttContainer);
    QObject::connect(exportButt,SIGNAL(pressed()),q_ptr,SLOT(exportShortcuts()));

    QHBoxLayout * buttLayout = new QHBoxLayout;
    buttLayout->setContentsMargins(4,2,4,2);
    buttLayout->addWidget(resetAllButt);
    buttLayout->addWidget(schemesBox);
    buttLayout->addStretch(1);
    buttLayout->addWidget(importButt);
    buttLayout->addWidget(exportButt);

    buttContainer->setLayout(buttLayout);

    keyGroupBox = new QGroupBox(mainWidget);
    keySequenceLabel = new QLabel(keyGroupBox);

    targetEdit = new QLineEdit(keyGroupBox);
    targetEdit->installEventFilter(q_ptr);

    resetButt = new RButton(keyGroupBox);
    QObject::connect(resetButt,SIGNAL(pressed()),q_ptr,SLOT(resetKeySequence()));

    QHBoxLayout * boxLayout = new QHBoxLayout;
    boxLayout->setContentsMargins(4,2,4,2);
    boxLayout->addWidget(keySequenceLabel);
    boxLayout->addWidget(targetEdit);
    boxLayout->addWidget(resetButt);

    keyGroupBox->setLayout(boxLayout);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->addWidget(treeWidget);
    mainLayout->addWidget(buttContainer);
    mainLayout->addWidget(keyGroupBox);

    mainWidget->setLayout(mainLayout);
}

ShortcutSettings::ShortcutSettings(QWidget * parent):DialogProxy(parent),d_ptr(new ShortcutSettingsPrivate(this))
{
    setButton(DialogProxy::Ok|DialogProxy::Cancel);
    setContentWidget(d_ptr->mainWidget);
    setWindowTitle(tr("Shortcut settings"));
    retranslateUi();

    initShortcutTree();
}

ShortcutSettings::~ShortcutSettings()
{

}

QSize ShortcutSettings::sizeHint() const
{
    return QSize(660,500);
}

void ShortcutSettings::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void ShortcutSettings::retranslateUi()
{
    Q_D(ShortcutSettings);

    d->resetAllButt->setText(tr("Reset All"));
    d->importButt->setText(tr("Import..."));
    d->exportButt->setText(tr("Export..."));

    d->keyGroupBox->setTitle(tr("Shortcut"));
    d->keySequenceLabel->setText(tr("Key sequence:"));
    d->targetEdit->setPlaceholderText(tr("Type to set shortcut"));
    d->resetButt->setText(tr("Reset"));
}

void ShortcutSettings::respButtClicked(DialogProxy::StandardButton butt)
{
    switch(butt){
        case DialogProxy::Ok:
            respOk();
            break;
        case DialogProxy::Cancel:
            respCancel();
            break;
        default:
            break;
    }
}

bool ShortcutSettings::eventFilter(QObject *obj, QEvent *event)
{
    Q_D(ShortcutSettings);

    if(obj == d->targetEdit){
        if(event->type() == QEvent::KeyPress){
            QKeyEvent * key = dynamic_cast<QKeyEvent *>(event);
            if(key){
                int newKey = key->key();

                if(d->keyPressCount > 3 ||
                        newKey == Qt::Key_Control ||
                        newKey == Qt::Key_Shift ||
                        newKey == Qt::Key_Meta ||
                        newKey == Qt::Key_Alt )
                    return true;

                newKey |=  translateModifiers(key->modifiers(),key->text());

                switch(d->keyPressCount){
                    case 0:
                          d->key[0] =newKey;
                          break;
                    case 1:
                          d->key[1] =newKey;
                          break;
                    case 2:
                          d->key[2] =newKey;
                          break;
                    case 3:
                          d->key[3] =newKey;
                          break;
                    default:
                        break;
                }
                d->keyPressCount++;
                d->currSchemeModified = true;

                QKeySequence sequence(d->key[0],d->key[1],d->key[2],d->key[3]);
                QString seqText = sequence.toString(QKeySequence::NativeText);
                d->targetEdit->setText(seqText);
                if(d->treeWidget->currentItem() && d->treeWidget->currentItem()->parent()){
                    ActionManager::ActionMap actMap = ActionManager::instance()->getAllActions();
                    ActionManager::ActionMapIterator iter = actMap.constBegin();
                    bool conflict = false;
                    while(iter != actMap.constEnd()){
                        if(iter.value()->action()->shortcut().toString(QKeySequence::NativeText) == seqText){
                            conflict = true;
                            break;
                        }
                        iter++;
                    }
                    if(conflict){
                        d->treeWidget->currentItem()->setBackgroundColor(T_SHORTCUT,Qt::red);
                    }else{
                        d->treeWidget->currentItem()->setBackgroundColor(T_SHORTCUT,d->treeWidget->currentItem()->backgroundColor(T_TAG));
                    }
                    d->treeWidget->currentItem()->setText(T_SHORTCUT,seqText);
                }
            }
        }
    }

    return DialogProxy::eventFilter(obj,event);
}

/*!
 * @brief 转换修饰控制键位
 * @details 对于输入Ctrl+Shift+S组合键来看，这个组合属于一个快捷键。在按下Ctrl、Shift时本QKeySequence不需要对按键进行记录，只有最后的S键时，才对
 *          整体的快捷键进行转换记录。而对像Ctrl、Shift、Alt、Meta等键，其属于修饰符键。
 * @param[in] state 当前键盘的触发的修饰符键
 * @param[in] text 同时按下多个键时，最后一个键的输入信息
 * @return 经过转义后的键盘值
 */
int ShortcutSettings::translateModifiers(Qt::KeyboardModifiers state,
                                         const QString &text)
{
    int result = 0;
    if ((state & Qt::ShiftModifier) && (text.size() == 0
                                        || !text.at(0).isPrint()
                                        || text.at(0).isLetterOrNumber()
                                        || text.at(0).isSpace()))
        result |= Qt::SHIFT;
    if (state & Qt::ControlModifier)
        result |= Qt::CTRL;
    if (state & Qt::MetaModifier)
        result |= Qt::META;
    if (state & Qt::AltModifier)
        result |= Qt::ALT;
    return result;
}

/*!
 * @brief 初始快捷键树
 * @details 从ActionManager::instance()中获取所有注册的action信息，并按照插件所属插件Id的名称分类显示， \p
 *          Id的命名请遵循constant.h中相关规则
 */

/*!
 *  @brief 插件快捷键节点
 *  @details
 *      |   + MouleA      |
 *      |   - MouleB      |
 *      |     - Plugin1   |
 *      |       - Action1 |
 *      |       - Action2 |
 *      |     - Plugin2   |
 */
struct ShortcutItem{
    QTreeWidgetItem * rootItem;
    QMap<QString,QTreeWidgetItem *> childItems;
};

void ShortcutSettings::initShortcutTree()
{
    Q_D(ShortcutSettings);
    ActionManager::ActionMap actMap = ActionManager::instance()->getAllActions();

    d->treeWidget->setColumnCount(3);
    QStringList headItem;
    headItem<<tr("Command")<<tr("Tag")<<tr("Shortcut");
    d->treeWidget->setHeaderLabels(headItem);
    d->treeWidget->header()->setSectionResizeMode(QHeaderView::Stretch);

    QMap<QString,ShortcutItem *> categorys;
    QMap<QString,QTreeWidgetItem *> pluginMap;
    ActionManager::ActionMap::iterator iter = actMap.begin();
    while(iter != actMap.end()){
        QString id(const_cast<Id&>(iter.key()).data());
        if(id.indexOf(".") > 0 && id.split(".").size() == LIST_ITEM_LEN){
            QStringList idList = id.split(".");
            ShortcutItem  * categoryItem = NULL;
            if(idList.at(0) == QString("Plugin")){
                if(!categorys.contains(idList.at(1))){
                    categoryItem = new ShortcutItem();
                    //创建模块节点
                    QTreeWidgetItem * rootItem = new QTreeWidgetItem();
                    rootItem->setText(0,idList.at(1));
                    d->treeWidget->addTopLevelItem(rootItem);
                    rootItem->setExpanded(true);
                    categoryItem->rootItem = rootItem;
                    categorys.insert(idList.at(1),categoryItem);
                }else{
                    categoryItem =  categorys.value(idList.at(1));
                }

                QTreeWidgetItem * pluginItem = new QTreeWidgetItem();
                pluginItem->setText(T_COMMAND,idList.at(2));
                pluginItem->setText(T_TAG,iter.value()->action()->text());
                pluginItem->setText(T_SHORTCUT,iter.value()->action()->shortcut().toString(QKeySequence::NativeText));
                d->shortcutItems.insert(id,pluginItem);
                categoryItem->rootItem->addChild(pluginItem);

                categoryItem->childItems.insert(idList.at(2),pluginItem);
                pluginMap.insert(idList.at(2),pluginItem);
                iter++;
                continue;
            }
        }
        iter++;
    }

    iter = actMap.begin();
    while(iter != actMap.end()){
        QString id(const_cast<Id&>(iter.key()).data());
        if(id.indexOf(".") > 0 && id.split(".").size() == LIST_ITEM_LEN){
            QStringList idList = id.split(".");
            if(idList.at(0) == QString("Action")){
                if(pluginMap.contains(idList.at(1))){
                    QTreeWidgetItem * pluginItem = pluginMap.value(idList.at(1));
                    if(pluginItem){
                        QTreeWidgetItem * childItem = new QTreeWidgetItem();
                        childItem->setText(T_COMMAND,idList.at(2));
                        childItem->setText(T_TAG,iter.value()->action()->text());
                        childItem->setText(T_SHORTCUT,iter.value()->action()->shortcut().toString(QKeySequence::NativeText));
                        d->shortcutItems.insert(id,childItem);
                        pluginItem->addChild(childItem);
                        pluginItem->setExpanded(true);
                        childItem->setData(T_COMMAND,Qt::UserRole,id);
                    }
                }
         }
      }
      iter++;
    }

    QMap<QString,ShortcutItem *>::iterator riter = categorys.begin();
    while(riter != categorys.end()){
        riter = categorys.erase(riter);
    }
}

void ShortcutSettings::updateTargetEditInfo()
{
    Q_D(ShortcutSettings);
    if(d->treeWidget->currentItem() && d->treeWidget->currentItem()->parent()){
        d->targetEdit->setText(d->treeWidget->currentItem()->text(T_SHORTCUT));
        d->targetEdit->setEnabled(true);
    }else{
        d->targetEdit->setEnabled(false);
    }
}

/*!
 * @brief 将修改的信息写入本地磁盘
 * @details 1.若在默认的配置上修改，则提示另存为；
 *          2.若在自定义文件上修改，则直接覆盖。
 */
void ShortcutSettings::respOk()
{
    Q_D(ShortcutSettings);

    QString saveFileName;
    if(d->currSchemeModified){
        ProgramFilePath filePath;
        if(d->schemesBox->currentIndex() == 0){
            saveFileName = QFileDialog::getSaveFileName(this,tr("save schemes"),filePath.shortcutPath);
            QFileInfo tmpInfo(saveFileName);
            if(saveFileName.isEmpty() || tmpInfo.exists()){
                return;
            }
        }else{
            saveFileName = filePath.shortcutPath + QDir::separator() + d->schemesBox->currentData().toString();
        }

        //[1] 保存设置至当前软件
        QMap<QString,QTreeWidgetItem *>::const_iterator iter = d->shortcutItems.constBegin();
        while(iter != d->shortcutItems.constEnd()){
            if(Action * action = ActionManager::instance()->action(Id(iter.key().toLocal8Bit().data()))){
                action->setDefaultKey(QKeySequence(iter.value()->text(T_SHORTCUT)));
            }
            iter++;
        }

        //[2] 将设置写入磁盘
        Base::RXmlFile xmlfile(saveFileName);
        xmlfile.setParseMethod(new ShortcutParseMethod());
        if(xmlfile.startSave(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
            QMessageBox::warning(this,tr("warning"),tr("Save successfully!"),QMessageBox::Yes);
        }else{
            QMessageBox::warning(this,tr("warning"),tr("Save failed!"),QMessageBox::Yes);
        }

        //[3] 添加新条目
        if(d->schemesBox->currentIndex() == 0){
            addNewItem(saveFileName);
            qDebug()<<QFileInfo(xmlfile.fileName()).fileName();
            updateLocalConfig(false,QFileInfo(xmlfile.fileName()).fileName());
        }
    }
    respCancel();
}

void ShortcutSettings::respCancel()
{
    close();
}

/*!
 * @brief 重置快捷键输入
 * @details 若当前存在点击的行，则同步将当前行的快捷键信息清除
 */
void ShortcutSettings::resetKeySequence()
{
    Q_D(ShortcutSettings);
    d->keyPressCount = 0;
    memset(d->key,0,sizeof(d->key));
    d->targetEdit->clear();

    if(d->treeWidget->currentItem() && d->treeWidget->currentItem()->parent()){
        d->treeWidget->currentItem()->setText(T_SHORTCUT,"");
    }
}

/*!
 * @brief 响应点击表格单元格，显示对应项的快捷方式
 * @param[in] item 点击的单元格
 * @param[in] column 列号
 */
void ShortcutSettings::respItemClicked(QTreeWidgetItem *item, int /*column*/)
{
    Q_D(ShortcutSettings);
    if(item->parent()){
        d->targetEdit->setText(item->text(T_SHORTCUT));
        d->targetEdit->setEnabled(true);
    }else{
        resetKeySequence();
        d->targetEdit->setEnabled(false);
    }
}

/*!
 * @brief 重置全部的快捷方式
 * @details 将恢复系统内部各个组件的快捷方式设置,并且将配置信息内容进行修改
 */
void ShortcutSettings::resetAllShortcuts()
{
    Q_D(ShortcutSettings);
    int result = QMessageBox::information(this,tr("information"),tr("Reset default Settings?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if(result == QMessageBox::Yes){
        ActionManager::ActionMap actMap = ActionManager::instance()->getAllActions();
        ActionManager::ActionMapIterator iter = actMap.constBegin();
        while(iter != actMap.constEnd()){
            QString id = const_cast<Id&>(iter.key()).data();
            if(id.indexOf(".") > 0 && id.split(".").size() == 2){
                iter.value()->resetDefaultKey();
                if(d->shortcutItems.contains(id)){
                    d->shortcutItems.value(id)->setText(T_SHORTCUT,iter.value()->defaultKey().toString(QKeySequence::NativeText));
                }
            }
            iter++;
        }
        d->schemesBox->setCurrentIndex(0);
        updateTargetEditInfo();
        RGlobal::G_GlobalConfigFile->systemConfigInfo.defaultKeySchemes = true;
        RGlobal::G_GlobalConfigFile->saveFile();
    }
}

/*!
 * @brief 导入本地快捷方式
 * @details 导入后，将导入的快捷方式保存至本地磁盘，下次启动时读取并覆盖默认快捷设置
 */
void ShortcutSettings::importShortcuts()
{
    QString openFileName = QFileDialog::getOpenFileName(this,tr("choose file"));
    if(!openFileName.isEmpty()){
        if(parsedLocalFile(openFileName)){
            addNewItem(openFileName);
        }
    }
}

/*!
 * @brief 解析导入的快捷键文件
 * @param[in] fileName 待解析的文件全路径
 * @param[in] userSelected true 为导入新的文件，需要将文件拷贝至schemes目录下
 *                         false  导入schemes目录下的文件
 */
bool ShortcutSettings::parsedLocalFile(QString fileName, bool userSelected)
{
    Q_D(ShortcutSettings);

    Base::RXmlFile xmlfile(fileName);
    ShortcutParseMethod * method = new ShortcutParseMethod();
    xmlfile.setParseMethod(method);
    if(xmlfile.startParse(QFile::ReadOnly)){
        QFileInfo  fileInfo(fileName);
        if(userSelected){
            ProgramFilePath filePath;
            QFileInfo info(filePath.shortcutPath + QDir::separator() + fileInfo.fileName());
            if(info.exists()){
                QMessageBox::warning(this,tr("warning"),tr("File already exists, please rename!"),QMessageBox::Yes);
                return false;
            }

            if(!QFile::copy(fileName,filePath.shortcutPath + QDir::separator() + fileInfo.fileName())){
                QMessageBox::warning(this,tr("warning"),tr("File move faild!"),QMessageBox::Yes);
                return false;
            }
        }

        ShortcutParseMethod::ShortcutMappingList list = method->getParsedList();
        ActionManager::ActionMap actMap = ActionManager::instance()->getAllActions();

        ActionManager::ActionMapIterator iter = actMap.constBegin();
        while(iter != actMap.constEnd()){
            QString id = const_cast<Id&>(iter.key()).data();
            if(id.indexOf(".") > 0 && id.split(".").size() == LIST_ITEM_LEN){
                auto findIndex = std::find_if(list.constBegin(),list.constEnd(),[&id](const ShortcutMapping & item){
                    return item.id == id;
                });
                if(findIndex != list.constEnd()){
                    if(d->shortcutItems.contains(id)){
                        d->shortcutItems.value(id)->setText(T_SHORTCUT,(*findIndex).keySequence);
                    }
                    iter.value()->setDefaultKey(QKeySequence((*findIndex).keySequence));
                }else{
                    if(d->shortcutItems.contains(id)){
                        d->shortcutItems.value(id)->setText(T_SHORTCUT,"");
                    }
                    iter.value()->setDefaultKey(QKeySequence());
                }
            }
            iter++;
        }

        updateLocalConfig(false,fileInfo.fileName());

        updateTargetEditInfo();
        QMessageBox::information(this,tr("information"),tr("Import shortcut successfully!"),QMessageBox::Yes);
        return true;
    }
    QMessageBox::warning(this,tr("warning"),tr("Import shortcut failed!"),QMessageBox::Yes);
    return false;
}

void ShortcutSettings::addNewItem(QString filename)
{
    Q_D(ShortcutSettings);
    QFileInfo fileInfo(filename);
    QStringList list = fileInfo.fileName().split(".");
    if(list.size() == 2){
        d->schemesBox->addItem(list.at(0),fileInfo.fileName());
        d->schemesBox->setCurrentIndex(d->schemesBox->count() - 1);
    }
}

void ShortcutSettings::updateLocalConfig(bool flag, QString fileName)
{
    RGlobal::G_GlobalConfigFile->systemConfigInfo.defaultKeySchemes = flag;
    RGlobal::G_GlobalConfigFile->systemConfigInfo.userKeySchemesName = fileName;
    RGlobal::G_GlobalConfigFile->saveFile();
}

/*!
 * @brief 导出现有的快捷方式
 * @details 按照自定义的xml格式，将快捷方式导出
 */
void ShortcutSettings::exportShortcuts()
{
    QString saveFileName = QFileDialog::getSaveFileName(this,tr("choose path"));
    if(!saveFileName.isEmpty()){
        Base::RXmlFile xmlfile(saveFileName);
        xmlfile.setParseMethod(new ShortcutParseMethod());
        if(xmlfile.startSave(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
            QMessageBox::information(this,tr("information"),tr("Export shortcut successfully!"),QMessageBox::Yes);
        }else{
            QMessageBox::warning(this,tr("warning"),tr("Export shortcut failed!"),QMessageBox::Yes);
        }
    }
}

/*!
 * @brief 切换显示不同的快捷键配置文件
 * @details index为0表示系统内置的快捷键
 * @param[in]  index    待切换的文件序号
 */
void ShortcutSettings::switchSchemes(int index)
{
    Q_D(ShortcutSettings);
    d->currSchemeModified = false;
    if(index == 0){
        resetAllShortcuts();
    }else{
        ProgramFilePath filePath;
        parsedLocalFile(filePath.shortcutPath + QDir::separator() + d->schemesBox->itemData(index).toString(),false);
    }
}

} //namespace Core{
