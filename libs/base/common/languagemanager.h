/*!
 *  @brief     语言管理
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.15
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef LANGUAGEMANAGER_H
#define LANGUAGEMANAGER_H

#include <QObject>
#include <QList>

#include "../base_global.h"

class QAction;

namespace Base{

class BASESHARED_EXPORT Language{
public:
    Language(QString fileName,QString countryCode);

    QString getName()const{return this->name;}
    QString getFileName()const{return this->fileName;}

    void setSelected(bool flag){this->selected = flag;}
    bool isSelected()const{return this->selected;}

    void setAction(QAction * act){this->action = act;}
    QAction * getAction(){return action;}

private:
    void switchName(QString code);
private:
    QString name;           /*!< Action显示语言名称 */
    QString fileName;       /*!< 翻译文件名(带后缀) */
    bool selected;          /*!< 是否被选中显示 */
    QAction * action;
};

typedef QList<Language *> LanguagePtrList;

class LanguageManagerPrivate;

class BASESHARED_EXPORT LanguageManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(LanguageManager)
public:
    explicit LanguageManager();

    void loadTranslator(QString filePath);

    bool switchLanguage(QString fileName);
    LanguagePtrList languages();
    Language * findLan(QString fileName);

private:
    LanguageManagerPrivate * d_ptr;

};

} //namespace Base

#endif // LANGUAGEMANAGER_H
