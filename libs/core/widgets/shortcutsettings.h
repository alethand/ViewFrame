/*!
 *  @brief     快捷键设置窗口
 *  @details   1.支持从配置目录下读取快捷键信息；
 *             2.支持手动的修改某个快捷键信息；
 *             3.支持快捷键的导出和导入；
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.26
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef SHORTCUTSETTINGS_H
#define SHORTCUTSETTINGS_H

#include "Base/selfwidget/dialogproxy.h"
#include "pluginmanager/observer.h"

#include "Base/util/fileutils.h"

#include <QSharedPointer>
class QTreeWidgetItem;

namespace Core{

struct ShortcutMapping
{
    QString id;
    QString keySequence;
};

class ShortcutParseMethod : public Base::RXmlParseMethod
{
public:
    ShortcutParseMethod(){}
    ~ShortcutParseMethod(){}

    typedef QList<ShortcutMapping> ShortcutMappingList;

    bool  startParse(QDomNode & rootNode);
    bool  startSave(QDomDocument & doc);

    ShortcutMappingList getParsedList(){return mappings;}

private:
    ShortcutMappingList mappings;

};

class ShortcutSettingsPrivate;

class ShortcutSettings : public DialogProxy,public Core::Observer
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ShortcutSettings)
public:
    explicit ShortcutSettings(QWidget * parent = 0);
    ~ShortcutSettings();

    enum TreeColumn{
        T_COMMAND = 0,
        T_TAG,
        T_SHORTCUT
    };

    QSize sizeHint()const;

    void onMessage(MessageType::MessType type);
    void retranslateUi();

protected:
    void respButtClicked(StandardButton butt);
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void respOk();
    void respCancel();
    void resetKeySequence();
    void respItemClicked(QTreeWidgetItem* item, int);

    void resetAllShortcuts();
    void importShortcuts();
    void exportShortcuts();
    void switchSchemes(int index);

private:
    int translateModifiers(Qt::KeyboardModifiers state, const QString &text);
    void initShortcutTree();
    void updateTargetEditInfo();
    bool parsedLocalFile(QString fileName,bool userSelected = true);
    void addNewItem(QString filename);
    void updateLocalConfig(bool flag,QString fileName);

private:
    QSharedPointer<ShortcutSettingsPrivate> d_ptr;
};

} //namespace Core

#endif // SHORTCUTSETTINGS_H
