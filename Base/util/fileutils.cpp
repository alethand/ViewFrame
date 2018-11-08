#include "fileutils.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include <QDebug>

#include "rlog.h"

namespace Base{

RFile::RFile(const QString &fileName):QFile(fileName)
{

}

bool RFile::startParse(OpenMode openMode)
{
    QFileInfo info(fileName());
    if(!info.exists()|| info.isDir()){
        RLOG_INFO("File [%s] type is not correct!",fileName().toLocal8Bit().data());
        return false;
    }

    if(!open(openMode)){
        RLOG_INFO("File [%s] open error!",fileName().toLocal8Bit().data());
        return false;
    }
    return true;
}

bool RFile::startSave(OpenMode openMode)
{
    QFileInfo info(fileName());
    if(info.isDir()){
        RLOG_INFO("File [%s] type is not correct!",fileName().toLocal8Bit().data());
        return false;
    }

    if(!open(openMode)){
        RLOG_INFO("File [%s] open error!",fileName().toLocal8Bit().data());
        return false;
    }

    return true;
}


RXmlFile::RXmlFile(const QString &fileName):RFile(fileName),parseMethod(NULL)
{

}

RXmlFile::~RXmlFile()
{
    if(parseMethod && isAutoReleaseParseMethod)
        delete parseMethod;
}

bool RXmlFile::startParse(OpenMode  openMode)
{
    if(parseMethod  && RFile::startParse(openMode)){
        QDomDocument doc;
        QString errorMsg;
        int errorRow = 0,errorCol = 0;
        if(!doc.setContent(this, false, &errorMsg, &errorRow, &errorCol)){
            RLOG_INFO("Open xml file error [row:%d,col:%d,msg:%s]!",errorRow,errorCol,errorMsg.toLocal8Bit().data());
            close();
            return false;
        }
        close();
        QDomElement root = doc.documentElement();
        if(!root.isNull())
        {
             return parseMethod->startParse(root.toElement());
        }
    }
    RLOG_INFO("Not set xml parseMethod!");
    return false;
}

bool RXmlFile::startSave(OpenMode  openMode)
{
    if(fileName().lastIndexOf(".xml") < 0)
        setFileName(fileName()+".xml");

    if(parseMethod && RFile::startSave(openMode))
    {
        QTextStream stream(this);
        stream.setCodec(QTextCodec::codecForLocale());

        QDomDocument doc("");

        QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml","version='1.0' encoding='UTF-8'");
        doc.appendChild(instruction);

        bool result = parseMethod->startSave(doc);
        if(result)
            doc.save(stream,4);
        return result;
    }
    RLOG_INFO("Not set xml parseMethod!");
    return false;
}

RTextFile::RTextFile(const QString &fileName):RFile(fileName),parseMethod(NULL)
{

}

RTextFile::~RTextFile()
{
    if(parseMethod)
        delete parseMethod;
}

bool RTextFile::startParse(OpenMode  openMode)
{
    if(parseMethod  && RFile::startParse(openMode)){
        return parseMethod->startParse(this);
    }

    RLOG_INFO("Not set text file parseMethod!");
    return false;
}

bool RTextFile::startSave(OpenMode  openMode)
{
    if(parseMethod && RFile::startSave(openMode))
    {
        return parseMethod->startSave(this);
    }
    RLOG_INFO("Not set text file parseMethod!");
    return false;
}

}
