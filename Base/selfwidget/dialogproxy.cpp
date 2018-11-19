#include "dialogproxy.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QCoreApplication>
#include <QPushButton>
#include <QVariant>
#include <QHash>
#include <QLabel>
#include <QMouseEvent>

#include "rbutton.h"

DialogTitleBar::DialogTitleBar(QWidget *parent)
{
    setFixedHeight(22);

    titleContent = new QWidget(this);

    titleLabel = new QLabel(this);
    titleLabel->setStyleSheet("background-color:rgba(17,227,246,120);");
    titleLabel->installEventFilter(this);

    QHBoxLayout * contentLayout = new QHBoxLayout;
    contentLayout->addWidget(titleLabel);
    contentLayout->setContentsMargins(0,0,0,0);
    titleContent->setLayout(contentLayout);

    QHBoxLayout * mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(titleContent);
    this->setLayout(mainLayout);
}

DialogTitleBar::~DialogTitleBar()
{

}

void DialogTitleBar::setTitle(QString text)
{
    titleLabel->setText(text);
}

bool DialogTitleBar::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == titleLabel)
    {
        switch(event->type()){
            case QEvent::MouseButtonPress:
                {
                    QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
                    if(eve){
                        mouseStartPoint = eve->pos();
                        mouseMoveable = true;
                    }
                }
                break;
            case QEvent::MouseMove:
                {
                    QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
                    if(eve){
                        QPoint offsetPos =  eve->pos() - mouseStartPoint;
                        emit newOffsetPos(offsetPos);
                        setCursor(Qt::ClosedHandCursor);
                    }
                }
                    break;
            case QEvent::MouseButtonRelease:
                {
                    mouseMoveable = false;
                    setCursor(Qt::ArrowCursor);
                }
                    break;
            default:
                break;
        }
    }
    return QWidget::eventFilter(watched,event);
}

class DialogProxyPrivate : public QObject
{
    Q_DECLARE_PUBLIC(DialogProxy)
public:
    DialogProxyPrivate(DialogProxy * q):q_ptr(q){
        initView();
    }
    ~DialogProxyPrivate(){}

    void initView();
    void updateButtLayout();

private:
    DialogProxy * q_ptr;

    QWidget * mainWidget;
    QWidget * contentWidget;                /*!< 中心显示区 */
    QWidget * buttonContainWidget;          /*!< 按钮区 */

    QList<RButton *> buttList;
    QHash<RButton *,DialogProxy::StandardButton> buttHash;
};

void DialogProxyPrivate::initView()
{
    mainWidget = new QWidget(q_ptr);

    DialogTitleBar *  titleBar = new DialogTitleBar(q_ptr);
    connect(titleBar,SIGNAL(newOffsetPos(QPoint)),q_ptr,SLOT(udpatePos(QPoint)));

    contentWidget = new QWidget(mainWidget);
    contentWidget->setObjectName("mainWidget");
    contentWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    buttonContainWidget = new QWidget(contentWidget);

    QVBoxLayout * mainLayout = new QVBoxLayout;
    mainLayout->setContentsMargins(1,1,1,1);
    mainLayout->setSpacing(2);

    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(contentWidget);

    mainWidget->setLayout(mainLayout);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(mainWidget);
    q_ptr->setLayout(layout);
}

void DialogProxyPrivate::updateButtLayout()
{
    if(!buttonContainWidget->layout()){
        QHBoxLayout * layout = new QHBoxLayout;
        layout->setContentsMargins(6,3,3,3);
        buttonContainWidget->setLayout(layout);
    }

    QHBoxLayout * layout = dynamic_cast<QHBoxLayout *>(buttonContainWidget->layout());

    int i = layout->count() - 1;

    while(i >= 0)
    {
        QLayoutItem * item = layout->takeAt(i);
        if(QWidget *widget = item->widget())
        {
            widget->hide();
        }
        delete item;
        i--;
    }

    for(int j = 0; j < buttList.size(); j++)
    {
        if(j == 0)
            layout->addStretch(1);

        buttList.at(j)->show();
        layout->addWidget(buttList.at(j));
    }
}


DialogProxy::DialogProxy(QWidget *parent):QDialog(parent),
    d_ptr(new DialogProxyPrivate(this))
{
    Qt::WindowFlags  flags = Qt::Dialog;
    flags |= Qt::WindowCloseButtonHint;
    flags |=  Qt::FramelessWindowHint;
    setAttribute(Qt::WA_TranslucentBackground);
    setWindowFlags(flags);
}

DialogProxy::~DialogProxy()
{

}

/*!
 * @brief 在底部按钮区添加按钮
 * @param[in]  butts 待添加的按钮or集合
 */
void DialogProxy::setButton(int butts)
{
    if(butts <= 0)
        return;

    uint mask = DialogProxy::FirstButton;
    while (mask <= DialogProxy::LastButton)
    {
        uint sb = butts & mask;
        mask <<= 1;
        if (!sb)
            continue;
        addButton((DialogProxy::StandardButton)sb);
    }
}

/*!
 * @brief 设置中心widget
 */
void DialogProxy::setContentWidget(QWidget *widget)
{
    Q_D(DialogProxy);
    if(widget == nullptr)
        return;

    if(!d->contentWidget->layout()){
        QVBoxLayout * layout = new QVBoxLayout;
        layout->setContentsMargins(0,0,0,0);

        layout->addWidget(widget);
        layout->addWidget(d->buttonContainWidget);

        d->contentWidget->setLayout(layout);
    }
}

/*!
 * @brief 响应按钮点击
 * @details 子类应覆写此类，用于响应点击操作
 * @param[in]  butt 发生点击按钮的类型
 */
void DialogProxy::respButtClicked(DialogProxy::StandardButton /*butt*/)
{

}

void DialogProxy::addButton(DialogProxy::StandardButton buttType)
{
    Q_D(DialogProxy);
    RButton * button = new RButton(d->buttonContainWidget);
    button->setProperty("buttType",buttType);
    button->setText(standardButtText(buttType));
    connect(button,SIGNAL(pressed()),this,SLOT(respButtonClicked()));

    d->buttHash.insert(button,buttType);
    d->buttList.append(button);
    d->updateButtLayout();
}

/*!
 * @brief 处理按钮点击
 * @details 按钮发生点击时，存在两种处理当前按钮响应的操作。
 *          【1】覆写respButtClicked(StandardButton)方法；
 *          【2】响应buttClicked(StandardButton)信号
 */
void DialogProxy::respButtonClicked()
{
    RButton * butt = dynamic_cast<RButton *>(QObject::sender());
    if(butt){
        StandardButton but = static_cast<StandardButton>(butt->property("buttType").toInt());
        respButtClicked(but);
        emit buttClicked(but);
    }
}

void DialogProxy::udpatePos(QPoint offsetPos)
{
    this->move(pos() + offsetPos);
}

/*!
 * @brief 获取对应按钮的文字描述
 * @param[in]  butt 按钮的类型
 * @return 对应文字描述
 */
QString DialogProxy::standardButtText(DialogProxy::StandardButton butt)
{
    if(butt)
    {
        switch(butt)
        {
            case DialogProxy::Ok:
                return QCoreApplication::translate("QPlatformTheme", "OK");
            case DialogProxy::Save:
                return QCoreApplication::translate("QPlatformTheme", "Save");
            case DialogProxy::SaveAll:
                return QCoreApplication::translate("QPlatformTheme", "SaveAll");
            case DialogProxy::Open:
                return QCoreApplication::translate("QPlatformTheme", "Open");
            case DialogProxy::Yes:
                return QCoreApplication::translate("QPlatformTheme", "Yes");
            case DialogProxy::YesToAll:
                return QCoreApplication::translate("QPlatformTheme", "YesToAll");
            case DialogProxy::No:
                return QCoreApplication::translate("QPlatformTheme", "No");
            case DialogProxy::NoToAll:
                return QCoreApplication::translate("QPlatformTheme", "NoToAll");
            case DialogProxy::Abort:
                return QCoreApplication::translate("QPlatformTheme", "Abort");
            case DialogProxy::Retry:
                return QCoreApplication::translate("QPlatformTheme", "Retry");
            case DialogProxy::Ignore:
                return QCoreApplication::translate("QPlatformTheme", "Ignore");
            case DialogProxy::Close:
                return QCoreApplication::translate("QPlatformTheme", "Close");
            case DialogProxy::Cancel:
                return QCoreApplication::translate("QPlatformTheme", "Cancel");
            case DialogProxy::Discard:
                return QCoreApplication::translate("QPlatformTheme", "Discard");
            case DialogProxy::Help:
                return QCoreApplication::translate("QPlatformTheme", "Help");
            case DialogProxy::Apply:
                return QCoreApplication::translate("QPlatformTheme", "Apply");
            case DialogProxy::Reset:
                return QCoreApplication::translate("QPlatformTheme", "Reset");
            case DialogProxy::RestoreDefaults:
                return QCoreApplication::translate("QPlatformTheme", "RestoreDefaults");
            default:
                break;
        }
    }
    return QString();
}
