/*!
 *  @brief     文本帮助工具类
 *  @details   提供统一的文本创建、保存等操作
 *  @file
 *  @author    wey
 *  @version   1.0
 *  @date      2017.12.XX
 *  @warning
 *  @copyright NanJing RenGu.
 */

#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QIODevice>
#include <QCoreApplication>

#include <QtXml/qdom.h>
#include <QtXml/QDomProcessingInstruction>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QtXml/QDomNodeList>

#include "../base_global.h"

class QFile;

namespace FileUtils
{

class BASESHARED_EXPORT FileSaverBase
{
    Q_DECLARE_TR_FUNCTIONS(FileSaverBase)
public:
    FileSaverBase();
    ~FileSaverBase();

    QString fileName() const { return m_fileName; }
    bool hasError() const { return m_hasError; }
    QString errorString() const { return m_errorString; }

    virtual bool finalize();

    bool write(const char *data, int len);
    bool write(const QByteArray &bytes);
    bool setResult(bool ok);

protected:
    QFile * m_file;
    QString m_fileName;
    QString m_errorString;
    bool m_hasError;

};

class BASESHARED_EXPORT FileSaver : public FileSaverBase
{
    Q_DECLARE_TR_FUNCTIONS(FileSaver)
public:
    explicit FileSaver(const QString &filename, QIODevice::OpenMode mode = QIODevice::NotOpen);
    QFile *file() { return m_file; }

    virtual bool finalize();

private:
};

///--------XMl文件操作----------///

class ParseXMLMethod;
class BASESHARED_EXPORT XmlFileOpt
{
public:
   static   XmlFileOpt* getOne();

   ///写入xml文件
   void writeFileHead();                        //函数无作用

   ///解析xml文件
   void setParseMethod(ParseXMLMethod *method);       //设置解析方式
   bool readXmLsFile(QString filePath);


   ParseXMLMethod *mParseMethod;

private:
    XmlFileOpt();

    static XmlFileOpt *singleton;
};


/*!
 * @brief   xml文件的解析方式
 * @warning 每个解析中的所解释出来的节构体声明为public
 * @note 装饰者模式
 */
class BASESHARED_EXPORT ParseXMLMethod
{
public:
    ParseXMLMethod(ParseXMLMethod *parent=0){mParent = parent;}
    virtual ~ParseXMLMethod(){}

    bool  startParse(QDomNode &node);
protected:
    ParseXMLMethod *mParent;
    virtual bool concreteParse(QDomNode &node) {Q_UNUSED(node) return true;}
};

}

#endif // FILEUTILS_H
