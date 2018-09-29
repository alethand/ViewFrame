#include "widgets/mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QMessageBox>
#include <QDebug>

#include "Base/util/rutil.h"
#include "Base/util/rlog.h"
#include "Base/util/rsingleton.h"
#include "Base/constants.h"
#include "Base/common/languagemanager.h"
#include "global.h"
#include "file/globalconfigfile.h"
#include "file/programfilepath.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setApplicationName(Constant::ApplicationName);
    QApplication::setOrganizationDomain(QObject::tr("rengu.com"));
    QApplication::setOrganizationName(QObject::tr("NanJing RenGu"));
    QApplication::setApplicationVersion(Constant::Version);

#if QT_VERSION > 0x050000
    QTextCodec * codec = QTextCodec::codecForName("utf-8");
    QTextCodec::setCodecForLocale(codec);
#endif

    ProgramFilePath programPath;

    //解析INI文件
    QSettings * settings = new QSettings(programPath.configFile,QSettings::IniFormat);
    RUtil::setGlobalSettings(settings);

    RGlobal::G_GlobalConfigFile = new GlobalConfigFile;
    RGlobal::G_GlobalConfigFile->setSettings(settings);
    if(!RGlobal::G_GlobalConfigFile->parseFile()){
        QMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("System INI file parsed false!"),QMessageBox::Yes,QMessageBox::Yes);
        return -1;
    }

    if(!RSingleton<RLog>::instance()->init(RGlobal::G_GlobalConfigFile->logConfig))
    {
        QMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("Log module initialization failure!"),QMessageBox::Yes,QMessageBox::Yes);
    }

    if(RUtil::createDir(programPath.translationPath)){
        RSingleton<Base::LanguageManager>::instance()->loadTranslator(programPath.translationPath);
    }

    if(!RUtil::createDir(programPath.shortcutPath)){
        QMessageBox::warning(NULL,QObject::tr("Warning"),QObject::tr("Create schemes path failed!"),QMessageBox::Yes,QMessageBox::Yes);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
