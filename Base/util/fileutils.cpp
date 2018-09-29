#include "fileutils.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>
#include <QDebug>

#include "rlog.h"

RFile::RFile(const QString &fileName):QFile(fileName)
{

}


RXmlFile::RXmlFile(const QString &fileName):RFile(fileName),parseMethod(NULL)
{

}

RXmlFile::~RXmlFile()
{
    if(parseMethod)
        delete parseMethod;
}

bool RXmlFile::startParse()
{
    if(parseMethod){

        QFileInfo info(fileName());
        if(!info.exists()|| info.isDir()){
            RLOG_INFO("File [%s] type is not correct!",fileName().toLocal8Bit().data());
            return false;
        }

        if(!open(QFile::ReadOnly)){
            RLOG_INFO("File [%s] open error!",fileName().toLocal8Bit().data());
            return false;
        }

        QDomDocument doc;
        QString errorMsg;
        int errorRow = 0,errorCol = 0;
        if(!doc.setContent(this, false, &errorMsg, &errorRow, &errorCol)){
            RLOG_INFO("Open xml file error [row:%d,col:%d,msg:%s]!",errorRow,errorCol,errorMsg.toLocal8Bit().data());
            close();
            return false;
        }
        close();
        return parseMethod->startParse(doc.documentElement());
    }
    RLOG_INFO("Not set xml parseMethod!");
    return false;
}

bool RXmlFile::startSave()
{
    if(parseMethod)
    {
        QFileInfo info(fileName());
        if(info.isDir()){
            RLOG_INFO("File [%s] type is not correct!",fileName().toLocal8Bit().data());
            return false;
        }

        if(fileName().lastIndexOf(".xml") < 0)
            setFileName(fileName()+".xml");

        if(!open(QFile::WriteOnly | QFile::Truncate | QFile::Text)){
            RLOG_INFO("File [%s] open error!",fileName().toLocal8Bit().data());
            return false;
        }

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
