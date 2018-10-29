#include "widgets/mainwindow.h"

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QSplashScreen>
#include <QTimer>

#include "Base/util/rutil.h"
#include "Base/util/rlog.h"
#include "Base/util/rsingleton.h"
#include "Base/constants.h"
#include "Base/common/languagemanager.h"
#include "global.h"
#include "file/globalconfigfile.h"
#include "file/programfilepath.h"
#include "Base/util/fileutils.h"
#include "Core/protocol/layout_byxml.h"
#include <QThread>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

using namespace ProtocolHandle;

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

    //加载并显示启动画面
    QSplashScreen splash(QPixmap(":/tech/resource/technology/logo1.jpg"));
    splash.setDisabled(true); //禁用用户的输入事件响应
    splash.show();
    QFont font;
    //设置文字字体
    font.setFamily("微软雅黑");
    //设置文字大小为50像素
    font.setPixelSize(50);
    //设置文字为粗体
    font.setBold(true);             //封装的setWeight函数
    //设置文字大小
    font.setPointSize(20);
    //设置文字粗细//enum Weight 存在5个值
    font.setWeight(QFont::Light);
    splash.setFont(QFont(font));
    splash.showMessage(QObject::tr("Wait..."),
        Qt::AlignLeft|Qt::AlignBottom,Qt::black);

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
//    QFont font;
//    font.setPointSize(20);
//    w.setFont(font);
    QEventLoop eventloop;
    QTimer::singleShot(550, &eventloop, SLOT(quit()));
    eventloop.exec();
    w.show();
    splash.finish(&w);

//    XMLLayout_Static *limitDisplay;
//    QThread *thread = new QThread();
//    limitDisplay = new XMLLayout_Static();
//    RXmlFile file("../test.xml");
//    file.moveToThread(thread);
//    file.setParseMethod(limitDisplay);
//    file.startParse();


    return a.exec();
}
