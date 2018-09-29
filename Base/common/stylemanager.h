/*!
 *  @brief     样式管理器
 *  @details   提供对内置样式的管理，支持从指定配置目录下读取扩展的样式文件
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include <QObject>
#include <QList>

#include "../base_global.h"

class QAction;

namespace Base{

/*!
 *  @brief 自定义样式信息描述
 *  @details
 */
class BASESHARED_EXPORT CustomStyle{

public:
    /*!
     *  @brief 自定义样式类枚举
     *  @details 每添加一种style，即添加一个字段
     */
    enum CustomStyleClass{

    };

    CustomStyle(){}

    CustomStyle(QString name,QString path,bool selected = false):name(name),path(path),selected(selected),stype(STYLE_SHEET){

    }

    CustomStyle(QString name,CustomStyleClass clazz,bool selected = false):name(name),clazz(clazz),selected(selected),stype(STYLE_CUSTOMSTYLE){

    }

    enum StyleType{
        STYLE_SHEET,            /*!< 样式表文件 */
        STYLE_CUSTOMSTYLE       /*!< 自定义样式类 */
    };

    QString getStyleName()const{return this->name;}
    StyleType getStyleType(){return this->stype;}
    CustomStyleClass getClazz(){return this->clazz;}
    QString getStylePath()const{return this->path;}
    bool isSelected()const{return this->selected;}

    void setAction( QAction * action){this->action = action;}
     QAction * getAction(){return this->action;}

private:
    QString name;           /*!< 样式名 */
    StyleType stype;        /*!< 样式类型 */
    CustomStyleClass clazz; /*!< 自定义样式类型 */
    QString path;           /*!< 样式保存路径，可为本地路径，也可为资源文件中路径 */
    bool selected;          /*!< 是否被选中显示 */
    QAction * action;       /*!< 工具栏对应按钮 */
};

typedef QList<CustomStyle *> StylePtrList;
class  StyleManagerPrivate;

class BASESHARED_EXPORT StyleManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(StyleManager)
public:
    explicit StyleManager();

    void addStyle(CustomStyle * style);
    int size();
    StylePtrList styles();
    void switchStyle(int index);
    CustomStyle *currentStyle();
    CustomStyle * findStyle(QString styleName);

private:
    StyleManagerPrivate * d_ptr;
};

}

#endif // STYLEMANAGER_H
