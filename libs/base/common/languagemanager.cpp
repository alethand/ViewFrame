#include "languagemanager.h"

#include <QTranslator>
#include <QDir>
#include <QApplication>
#include <QDebug>

namespace Base{

Language::Language(QString fileName, QString countryCode):fileName(fileName),selected(false)
{
    switchName(countryCode);
}

void Language::switchName(QString code)
{
    if(code.toLower() == QString("cn"))
        name = QStringLiteral("简体中文");
    else if(code.toLower() == QString("us"))
        name = QStringLiteral("English");

}

class LanguageManagerPrivate
{
    Q_DECLARE_PUBLIC(LanguageManager)

public:
    LanguageManagerPrivate(LanguageManager * q):q_ptr(q){

    }

    LanguageManager * q_ptr;
    LanguagePtrList lanlist;
    QString transPath;           /*!< 保存翻译文件路径 */
    QTranslator translator;
};

LanguageManager::LanguageManager():d_ptr(new LanguageManagerPrivate(this)),QObject()
{

}

/*!
 * @brief 从指定目录中加载翻译文件
 * @param[in] filePath 指定的存放路径
 */
void LanguageManager::loadTranslator(QString filePath)
{
    Q_D(LanguageManager);

    QStringList nameFilters;
    nameFilters<<QString("*.qm");
    QDir dir(filePath);
    QStringList qmfiles = dir.entryList(nameFilters);
    d->transPath = filePath;

    QLocale locale;

    std::for_each(qmfiles.begin(),qmfiles.end(),[&](const QString & fileName){
        QStringList result = fileName.left(fileName.indexOf(".")).split("_");
        if(result.size() == 3){
            Language * la = new Language(fileName,result.at(2));
            if(fileName.contains(locale.name())){
                la->setSelected(true);
            }
            d->lanlist.push_back(la);
        }
    });
}

/*!
 * @brief 切换至对应的翻译文件
 * @param[in] fileName 文件名
 * @return 是否切换成功
 */
bool LanguageManager::switchLanguage(QString fileName)
{
    Q_D(LanguageManager);
    if(d->translator.load(fileName,d->transPath))
    {
        qApp->installTranslator(&d->translator);
        return true;
    }
    return false;
}

LanguagePtrList LanguageManager::languages()
{
    Q_D(LanguageManager);
    return d->lanlist;
}

Language *LanguageManager::findLan(QString fileName)
{
    Q_D(LanguageManager);
    for(int i = 0; i < d->lanlist.size();i++){
        if(d->lanlist.at(i)->getFileName() == fileName)
            return d->lanlist.at(i);
    }
    return NULL;
}

} //namespace Base
