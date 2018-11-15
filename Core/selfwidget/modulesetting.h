/*!
 *  @brief     模块设置面板
 *  @details   1.可对组件的状态进行设置;
 *             2.可对网络信息设置;
 *  @author    wey
 *  @version   1.0
 *  @date      2018.11.14
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef MODULESETTING_H
#define MODULESETTING_H

#include "widget.h"
#include "pluginmanager/observer.h"

#include <QAbstractItemModel>
#include <QMap>

namespace Core{

class MSettingModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit MSettingModel(QObject *parent = 0);
    ~MSettingModel();

    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
    Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

private:
    struct TreeNode{
        TreeNode():index(0),parentNode(NULL),visible(true){}
        QString nodeName;
        QString nodeId;
        int index;
        bool visible;
        TreeNode * parentNode;
        QList<TreeNode *> nodes;
    };

private:
    TreeNode * rootNode;
    QStringList headList;
};

class ModuleSettingPrivate;

class ModuleSetting : public Widget,public Observer
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ModuleSetting)
public:
    explicit ModuleSetting(QWidget * parent = 0);
    ~ModuleSetting();

    QSize sizeHint()const;

    void onMessage(MessageType::MessType type);

private:
    ModuleSettingPrivate * d_ptr;
};

}

#endif // MODULESETTING_H
