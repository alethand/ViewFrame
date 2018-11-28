#include "titlebar.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QDebug>
#include <QEvent>
#include <QMouseEvent>
#include <QLabel>
#include <QLinearGradient>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QMap>

#include "Base/actionmanager/actionmanager.h"
#include "Base/actionmanager/actioncontainer.h"
#include "Base/util/rutil.h"

namespace Core{

IconButton::IconButton(QWidget *parent):QPushButton(parent),mouseHover(false),mousePressed(false)
{
     textFont = QFont(font().family(),10,75);
     connect(this,SIGNAL(pressed()),this,SLOT(respMenuTriggle()));
}

IconButton::~IconButton()
{

}

void IconButton::setText(QString text)
{
    m_content = text;
    QPushButton::setText(text);
}

void IconButton::setPixmap(QPixmap pix)
{
    m_pix = pix;
}

void IconButton::paintEvent(QPaintEvent *)
{
    QPainter * painter = new QPainter(this);

    painter->save();
    QLinearGradient gradient(0,0,0,height());
    gradient.setColorAt(1,QColor(17,227,246,80));
    gradient.setColorAt(0,QColor(17,227,246,110));
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(gradient));
    painter->drawRoundedRect(rect(),5,5);
    painter->restore();


    if(mouseHover){
        painter->save();
        QLinearGradient gradient(0,0,0,height());
        gradient.setColorAt(1,QColor(255,195,0,245));
        gradient.setColorAt(0,QColor(252,240,0,250));
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(gradient));
        painter->drawRoundedRect(rect(),5,5);
        painter->restore();
    }

    painter->save();
    QFontMetrics metrics(textFont);
    QSize fontSize = metrics.size(Qt::TextSingleLine,m_content);
    painter->setFont(textFont);
    int contentX = fontSize.width() + fontSize.height();
    int posX = (width() - contentX) / 2;
    int posY = (height() - fontSize.height()) / 2;
    if(mousePressed)
        posY+= 1;
    if(!mouseHover)
        painter->setPen(Qt::white);
    else
        painter->setPen(QColor(68,68,68,240));
    painter->drawText(QRect(posX + fontSize.height(),posY,fontSize.width(),fontSize.height()),Qt::AlignVCenter,m_content);
    painter->restore();

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->drawPixmap(QRect(posX,posY,fontSize.height(),fontSize.height()),m_pix);
    painter->restore();
}

void IconButton::mousePressEvent(QMouseEvent *)
{
    mousePressed = true;
    update();
}

void IconButton::mouseReleaseEvent(QMouseEvent * event)
{
    mousePressed = false;
    update();
    QPushButton::mousePressEvent(event);
}

void IconButton::enterEvent(QEvent *event)
{
    mouseHover = true;
    update();
}

void IconButton::leaveEvent(QEvent *event)
{
    mouseHover = false;
    update();
}

void IconButton::respMenuTriggle()
{
    if(menu())
        menu()->setFixedWidth(width());
}

class TitleBarPrivate{
    Q_DECLARE_PUBLIC(TitleBar)
public:
    TitleBarPrivate(TitleBar * q):q_ptr(q),barVisible(true){
        init();
    }
    void init();

    TitleBar * q_ptr;
    QWidget * mainWidget;
    QWidget * iconWidget;       /*!< 工具按钮显示区 */

    QLabel * titleLabel;        /*!< 文字显示区 */
    QMap<QString,IconButton *>    availableButts;      /*!< 添加功能按钮列表 */

    QString title;
    QPainterPath controlPath;       /*!< 控制显示/隐藏区域 */
    bool barVisible;                /*!< 控制面板是否显示 */
};


void TitleBarPrivate::init()
{
    mainWidget = new QWidget(q_ptr);
//    mainWidget->setObjectName("mainWidget");

    titleLabel = new QLabel(mainWidget);
    titleLabel->setObjectName("titleBar_titlelabel");

    iconWidget = new QWidget(mainWidget);
    iconWidget->setFixedSize(800,25);

    QHBoxLayout * layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(mainWidget);
    q_ptr->setLayout(layout);

    mainWidget->installEventFilter(q_ptr);
}

TitleBar::TitleBar(QWidget *parent):Widget(parent),d_ptr(new TitleBarPrivate(this))
{
    setObjectName("dataview_titlebar");
    setWidgetFeatures(getWidgetFeatures() & (~Widget::WidgetResizeable));
}

TitleBar::~TitleBar()
{

}

/*!
 * @brief 添加菜单按钮，并设置下拉菜单按钮
 * @details 传入的Id，与菜单栏中menubar中menu的id一致。 @n
 * @param[in] name 按钮名
 * @param[in] id 按钮Id
 * @param[in] buttPix 图片
 */
void TitleBar::addMenu(QString name, QString id, QPixmap buttPix)
{
    Q_D(TitleBar);
    if(d->availableButts.contains(id))
        return;

    QHBoxLayout * hlayout = NULL;
    if(!d->iconWidget->layout()){
        hlayout = new QHBoxLayout;
        hlayout->setContentsMargins(2,2,2,2);
        d->iconWidget->setLayout(hlayout);
    }else{
        hlayout = dynamic_cast<QHBoxLayout *>(d->iconWidget->layout());
    }

    IconButton * butt = new IconButton(d->iconWidget);
    butt->setText(name);
    butt->setPixmap(name);
    ActionContainer * container = ActionManager::instance()->actionContainer(id.toLocal8Bit().data());
    if(container){
        butt->setMenu(container->menu());
    }

    hlayout->addWidget(butt);

    d->availableButts.insert(id,butt);
}

void TitleBar::setTitle(QString title)
{
    Q_D(TitleBar);
    d->title = title;
    d->titleLabel->setText(title);
}

QString TitleBar::title() const
{
    Q_D(const TitleBar);
    return d->title;
}

void TitleBar::setIcon(QPixmap pixmap)
{

}

bool TitleBar::eventFilter(QObject *watched, QEvent *event)
{
    Q_D(TitleBar);
    if(watched == d->mainWidget){
        switch(event->type()){
            case QEvent::Paint:
                {
                    QSize windowSize = d->mainWidget->size();
                    QPainter painter(d->mainWidget);
                    painter.setRenderHint(QPainter::Antialiasing, true);

                    //背景
                    int breakPos = windowSize.width() * 0.25;
                    painter.save();
                    QPainterPath backgroundPath;
                    backgroundPath.moveTo(0,0);
                    backgroundPath.lineTo(0,d->iconWidget->y());
                    backgroundPath.lineTo(breakPos,d->iconWidget->y());
                    backgroundPath.lineTo(breakPos + 20,windowSize.height());
                    backgroundPath.lineTo(windowSize.width() ,windowSize.height());
                    backgroundPath.lineTo(windowSize.width() ,0);

                    d->controlPath = QPainterPath();
                    d->controlPath.moveTo(breakPos - 10,d->iconWidget->y());
                    d->controlPath.lineTo(breakPos + 10,windowSize.height());
                    d->controlPath.lineTo(breakPos - 50,windowSize.height());
                    d->controlPath.lineTo(breakPos - 70,d->iconWidget->y());

                    painter.fillPath(backgroundPath,QBrush(QColor(13,26,42,150)));

                    painter.setPen(QColor(0,204,255,150));
                    painter.drawPath(backgroundPath);
                    painter.drawPath(d->controlPath);
                    painter.restore();

                    //控制区
                    QColor yellowColor(255,195,0,245);
                    painter.save();
                    int radius = 5;
                    QPoint cpoint = QPoint(breakPos - 30,d->iconWidget->y() + d->iconWidget->height() / 2);
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(yellowColor);
                    QSize rectSize(2 * radius,2);
                    if(d->barVisible){
                        painter.drawRect(QRect(cpoint.x()- radius,cpoint.y() - rectSize.height() / 2,rectSize.width(),rectSize.height()));
                    }else{
                        painter.drawRect(QRect(cpoint.x()- radius,cpoint.y() - rectSize.height()/2,rectSize.width(),rectSize.height()));
                        painter.drawRect(QRect(cpoint.x()-rectSize.height()/2,cpoint.y() - radius,rectSize.height(),rectSize.width()));
                    }
                    painter.restore();

                    //渐变条
                    QBrush yellowBrush(yellowColor);
                    for(int i = 0;i<10;i++){
                        int marginLeft = 15 * i;
                        QPainterPath polygonPath;
                        QColor tmpYellowBrush = yellowColor;
                        tmpYellowBrush.setAlpha(tmpYellowBrush.alpha() - 13 * i);
                        QBrush gradientBrsuh(tmpYellowBrush);
                        painter.save();
                        polygonPath.moveTo(breakPos + marginLeft,d->iconWidget->y());
                        polygonPath.lineTo(breakPos + marginLeft + 20,windowSize.height());
                        polygonPath.lineTo(breakPos + marginLeft + 30,windowSize.height());
                        polygonPath.lineTo(breakPos + marginLeft + 10,d->iconWidget->y());
                        painter.fillPath(polygonPath,gradientBrsuh);
                        painter.restore();
                    }

                    //线条
                    painter.save();
                    int pathHeight = 5;
                    QPainterPath topRightPath;
                    topRightPath.moveTo(windowSize.width(),0);
                    topRightPath.lineTo(windowSize.width() - 10,pathHeight);
                    topRightPath.lineTo(windowSize.width() * 0.7,pathHeight);
                    topRightPath.lineTo(windowSize.width() * 0.7 - 10,0);
                    topRightPath.lineTo(0,0);
                    painter.fillPath(topRightPath,yellowBrush);

                    QPainterPath bottomLeftPath;
                    bottomLeftPath.moveTo(0,windowSize.height() - pathHeight);
                    bottomLeftPath.lineTo(windowSize.width() * 0.1, windowSize.height() - pathHeight);
                    bottomLeftPath.lineTo(windowSize.width() * 0.1 + 10,windowSize.height());
                    bottomLeftPath.lineTo(0,windowSize.height());
                    bottomLeftPath.lineTo(0,0);
//                    painter.fillPath(bottomLeftPath,yellowBrush);
                    painter.restore();
                }
                break;

            case QEvent::MouseButtonPress:
                {
                    QMouseEvent * eve = dynamic_cast<QMouseEvent *>(event);
                    if(eve && d->controlPath.contains(eve->pos())){
                        animationView(d->barVisible);
                        d->barVisible = !d->barVisible;
                    }
                }
            break;

            default:
                break;
        }
    }
    return Widget::eventFilter(watched,event);
}

void TitleBar::resizeEvent(QResizeEvent *)
{
    Q_D(TitleBar);
    int posX = size().width() - d->iconWidget->width();
    int posY = size().height() - d->iconWidget->height();
    d->iconWidget->move(posX,posY);

    QSize textMinSize = d->titleLabel->fontMetrics().size(Qt::TextSingleLine,d->titleLabel->text());
    posX = 20;
    posY = (size().height() - textMinSize.height() )/2;
    d->titleLabel->setGeometry(posX,posY,textMinSize.width(),textMinSize.height());
}

void TitleBar::updateFeatures()
{
    Q_D(TitleBar );
    if(d->barVisible != testFeatures(WidgetExpanable)){
        animationView(d->barVisible);
        d->barVisible = !d->barVisible;
    }
}

void TitleBar::animationView(bool isVisible)
{
    Q_D(TitleBar);
    QPropertyAnimation * animation = new QPropertyAnimation(this,"pos");
    QPoint source(0,0);
    QPoint dest(0,d->iconWidget->height() - height());
    if(isVisible){
        animation->setKeyValueAt(0,source);
        animation->setKeyValueAt(1,dest);
        currentFeatures &= ~Widget::WidgetExpanable;
    }else{
        animation->setKeyValueAt(0,dest);
        animation->setKeyValueAt(1,source);
        currentFeatures |= Widget::WidgetExpanable;
    }
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::OutQuad);
    animation->start(QPropertyAnimation::DeleteWhenStopped);
}

}
