#include <QApplication>

#include <QTextCodec>
#include <QDebug>
#include <QSplashScreen>
#include <QTimer>

#include <base/constants.h>
#include <core/widgets/mainwindow.h>

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

    //加载并显示启动画面
    QSplashScreen splash(QPixmap(":/tech/resource/technology/logo1.jpg"));
    splash.setDisabled(true); //禁用用户的输入事件响应
    splash.show();
    QFont font;
    font.setFamily("微软雅黑");
    font.setPixelSize(50);
    font.setBold(true);
    font.setPointSize(20);
    font.setWeight(QFont::Light);
    splash.setFont(QFont(font));
    splash.showMessage(QObject::tr("Wait..."),Qt::AlignLeft|Qt::AlignBottom,Qt::black);

    Core::MainWindow w;
    w.initAll();
    w.show();
    w.displayResize();

    splash.finish(&w);

    return a.exec();
}
