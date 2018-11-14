#include "mydockwidget.h"

#include "mydockwidgettitlebutton.h"

#include <QStyle>
#include <QStyleOptionDockWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QAction>

#include <QDebug>

namespace Core{

#define MOUSE_TRIGGER_SPACE 1        //窗口各边对鼠标事件触发间距

class MyDockWidgetPrivate
{
    Q_DECLARE_PUBLIC(MyDockWidget)
public:
    MyDockWidgetPrivate(MyDockWidget * q):q_ptr(q),contentExpanded(true),mousePressArea(0),leftButtonPressed(false),
        features(MyDockWidget::DockWidgetClosable| MyDockWidget::DockWidgetMovable| MyDockWidget::DockWidgetFloatable),content(NULL){
        dockLayout = new DockLayout(q);
        dockLayout->setContentsMargins(1,1,1,1);
    }

    void init();
    void updateButtons();

    bool mousePress(QMouseEvent * event);
    bool mouseMove(QMouseEvent * event);
    bool mouseRelease(QMouseEvent * event);
    bool mouseDoubleClickEvent(QMouseEvent * event);

    //边框四周拖拽计算
    int countMouseArea(QPoint p, int row);
    int countHorizonalArea(QPoint p);
    void setCursorType(int areaCode);

    QString fixedWindowTitle;
    QPoint mouseStartPoint;     /*!< 鼠标按下位置 */
    QWidget * titleBar;         /*!< 标题栏,可只显示图标 */
    QWidget * content;          /*!< 内容区,可折叠 */
    bool mouseMoveable;         /*!< 鼠标点下的位置是否支持移动 */
    bool contentExpanded;       /*!< 内容面板是否折叠 */

    QAction *toggleViewAction;

    bool leftButtonPressed;
    QPoint globalMouseStartPoint;
    int mousePressArea;

    MyDockWidget * q_ptr;
    DockLayout * dockLayout;
    MyDockWidget::DockWidgetFeatures features;
};

static inline bool hasFeature(const MyDockWidgetPrivate *priv, MyDockWidget::DockWidgetFeature feature)
{ return (priv->features & feature) == feature; }

static inline bool hasFeature(const MyDockWidget *dockwidget, MyDockWidget::DockWidgetFeature feature)
{ return (dockwidget->features() & feature) == feature; }

DockLayout::DockLayout(MyDockWidget *parent):QLayout(parent),items(MyDockWidget::RoleCount,NULL),verticalTitleBar(false)
{
    setObjectName("Hello!");
}

void DockLayout::addWidget(MyDockWidget::ButtonRole role, QWidget *w)
{
    QWidget * tmpWidget = getWidget(role);
    if(tmpWidget){
        tmpWidget->hide();
        removeWidget(w);
    }

    if(w){
        QWidgetItem * item = new QWidgetItem(w);
        items[role] = item;
        w->show();
    }else{
        items[role] = NULL;
    }
}

QWidget *DockLayout::getWidget(MyDockWidget::ButtonRole role) const
{
    QLayoutItem * item = items.at(role);
    return item == 0 ? 0 : item->widget();
}

void DockLayout::addItem(QLayoutItem *item)
{
    qWarning() << "please use DockLayout::addWidget()";
    return;
}

/*!
 * @brief 设置Layout中每个控件的位置信息
 * @details 布局中每一项的位置、尺寸是也是计算出来的 O(∩_∩)O
 * @warning 在计算button的位置时，并未参考原来QDockWidget中内置的使用QStyle::subElementRect()方式获得内置的高度。
 * @param[in] geometry 当前窗口尺寸
 */
void DockLayout::setGeometry(const QRect &geometry)
{
    MyDockWidget *q = qobject_cast<MyDockWidget*>(parentWidget());

    QMargins margins = contentsMargins();

    bool nativeDeco = nativeWindowDeco();

    int fw = q->isFloating() && !nativeDeco
            ? q->style()->pixelMetric(QStyle::PM_DockWidgetFrameWidth, 0, q)
            : 0;

    if (nativeDeco) {
        if (QLayoutItem *item = items[MyDockWidget::Content])
            item->setGeometry(geometry);
    } else {
        int titleHeight = this->titleHeight();

        if (verticalTitleBar) {
            _titleArea = QRect(QPoint(fw, fw),
                                QSize(titleHeight, geometry.height() - (fw * 2)));
        } else {
            _titleArea = QRect(QPoint(fw, fw),
                                QSize(geometry.width() - (fw * 2), titleHeight));
        }

        if (QLayoutItem *item = items[MyDockWidget::TitleBar]) {
            item->setGeometry(_titleArea);
        } else {
            QStyleOptionDockWidgetV2 opt;
            q->initStyleOption(&opt);

            int leftPos = _titleArea.width();
            int itemSpace = 1;

            if (QLayoutItem *item = items[MyDockWidget::CloseButton]) {
                if (!item->isEmpty()) {
                        leftPos -= item->widget()->width();
                        leftPos -= itemSpace;
                        item->setGeometry(QRect(leftPos,0,item->widget()->width(),item->widget()->height()));
                }
            }

            if (QLayoutItem *item = items[MyDockWidget::FloatButton]) {
                if (!item->isEmpty()) {
                    leftPos -= item->widget()->width();
                    leftPos -= itemSpace;
                    item->setGeometry(QRect(leftPos,0,item->widget()->width(),item->widget()->height()));
                }
            }

            if(QLayoutItem *item = items[MyDockWidget::TitleLabel]){
                if (!item->isEmpty()) {
                    leftPos -= itemSpace;
                     item->setGeometry(QRect(0,0,leftPos,_titleArea.height()));
                }
            }
        }

        if (QLayoutItem *item = items[MyDockWidget::Content]) {
            QRect r = geometry;
            if (verticalTitleBar) {
                r.setLeft(_titleArea.right() + 1);
                r.adjust(0, fw, -fw, -fw);
            } else {
                r.setTop(_titleArea.bottom() + 1);
                r.adjust(margins.left(), 0, - margins.right(), -margins.bottom());
            }
            item->setGeometry(r);
        }
    }
}

static inline int pick(bool vertical, const QSize &size)
{
    return vertical ? size.height() : size.width();
}

static inline int perp(bool vertical, const QSize &size)
{
    return vertical ? size.width() : size.height();
}

bool DockLayout::nativeWindowDeco() const
{
    return nativeWindowDeco(parentWidget()->isWindow());
}

bool DockLayout::nativeWindowDeco(bool floating) const
{
    return floating && items[MyDockWidget::TitleBar] == 0;
}


int DockLayout::minimumTitleWidth() const
{
    MyDockWidget *q = qobject_cast<MyDockWidget*>(parentWidget());

    if (QWidget *title = getWidget(MyDockWidget::TitleBar))
        return pick(verticalTitleBar, title->minimumSizeHint());

    QSize closeSize(0, 0);
    QSize floatSize(0, 0);
    if (hasFeature(q, MyDockWidget::DockWidgetClosable)) {
        if (QLayoutItem *item = items[MyDockWidget::CloseButton])
            closeSize = item->widget()->sizeHint();
    }
    if (hasFeature(q, MyDockWidget::DockWidgetFloatable)) {
        if (QLayoutItem *item = items[MyDockWidget::FloatButton])
            floatSize = item->widget()->sizeHint();
    }

    int titleHeight = this->titleHeight();

    int mw = q->style()->pixelMetric(QStyle::PM_DockWidgetTitleMargin, 0, q);
    int fw = q->style()->pixelMetric(QStyle::PM_DockWidgetFrameWidth, 0, q);

    return pick(verticalTitleBar, closeSize)
            + pick(verticalTitleBar, floatSize)
            + titleHeight + 2*fw + 3*mw;
}

/*!
 * @brief 计算TitleBar区域占用高度
 * @return 实际高度
 */
int DockLayout::titleHeight() const
{
    MyDockWidget *q = qobject_cast<MyDockWidget*>(parentWidget());

    if (QWidget *title = getWidget(MyDockWidget::TitleBar))
        return perp(verticalTitleBar, title->sizeHint());

    QSize closeSize(0, 0);
    QSize floatSize(0, 0);
    if (QLayoutItem *item = items[MyDockWidget::CloseButton])
        closeSize = item->widget()->sizeHint();

    if (QLayoutItem *item = items[MyDockWidget::FloatButton])
        floatSize = item->widget()->sizeHint();

    int buttonHeight = qMax(perp(verticalTitleBar, closeSize),
                            perp(verticalTitleBar, floatSize));

    QFontMetrics titleFontMetrics = q->fontMetrics();
    int mw = q->style()->pixelMetric(QStyle::PM_DockWidgetTitleMargin, 0, q);
    return qMax(buttonHeight + 2, titleFontMetrics.height() + 2*mw);
}

QSize DockLayout::sizeFromContent(const QSize &content, bool floating) const
{
    QSize result = content;

    if (verticalTitleBar) {
        result.setHeight(qMax(result.height(), minimumTitleWidth()));
        result.setWidth(qMax(content.width(), 0));
    } else {
        result.setHeight(qMax(result.height(), 0));
        result.setWidth(qMax(content.width(), minimumTitleWidth()));
    }

    MyDockWidget *w = qobject_cast<MyDockWidget*>(parentWidget());
    const bool nativeDeco = nativeWindowDeco(floating);

    int fw = floating && !nativeDeco
            ? w->style()->pixelMetric(QStyle::PM_DockWidgetFrameWidth, 0, w)
            : 0;

    const int th = titleHeight();
    if (!nativeDeco) {
        if (verticalTitleBar)
            result += QSize(th + 2*fw, 2*fw);
        else
            result += QSize(2*fw, th + 2*fw);
    }

    result.setHeight(qMin(result.height(), (int) QWIDGETSIZE_MAX));
    result.setWidth(qMin(result.width(), (int) QWIDGETSIZE_MAX));

    if (content.width() < 0)
        result.setWidth(-1);
    if (content.height() < 0)
        result.setHeight(-1);

    int left, top, right, bottom;
    w->getContentsMargins(&left, &top, &right, &bottom);
    //we need to substract the contents margin (it will be added by the caller)
    QSize min = w->minimumSize() - QSize(left + right, top + bottom);
    QSize max = w->maximumSize() - QSize(left + right, top + bottom);

    /* A floating dockwidget will automatically get its minimumSize set to the layout's
       minimum size + deco. We're *not* interested in this, we only take minimumSize()
       into account if the user set it herself. Otherwise we end up expanding the result
       of a calculation for a non-floating dock widget to a floating dock widget's
       minimum size + window decorations. */

    uint explicitMin = 0;
    uint explicitMax = 0;
//    if (w->d_func()->extra != 0) {
//        explicitMin = w->d_func()->extra->explicitMinSize;
//        explicitMax = w->d_func()->extra->explicitMaxSize;
//    }

    if (!(explicitMin & Qt::Horizontal) || min.width() == 0)
        min.setWidth(-1);
    if (!(explicitMin & Qt::Vertical) || min.height() == 0)
        min.setHeight(-1);

    if (!(explicitMax & Qt::Horizontal))
        max.setWidth(QWIDGETSIZE_MAX);
    if (!(explicitMax & Qt::Vertical))
        max.setHeight(QWIDGETSIZE_MAX);

    return result.boundedTo(max).expandedTo(min);
}

QSize DockLayout::sizeHint() const
{
    MyDockWidget *w = qobject_cast<MyDockWidget*>(parentWidget());

    QSize content(-1, -1);
    if (items[MyDockWidget::Content] != 0)
        content = items[MyDockWidget::Content]->sizeHint();
qDebug()<<"++++"<<sizeFromContent(content, w->isFloating());
    return sizeFromContent(content, w->isFloating());
}

QLayoutItem *DockLayout::itemAt(int index) const
{
    int cnt = 0;
    for (int i = 0; i < items.count(); ++i) {
        QLayoutItem *item = items.at(i);
        if (item == 0)
            continue;
        if (index == cnt++)
            return item;
    }
    return 0;
}

QLayoutItem *DockLayout::takeAt(int index)
{
    int j = 0;
    for (int i = 0; i < items.count(); ++i) {
        QLayoutItem *item = items.at(i);
        if (item == 0)
            continue;

        if (index == j)
        {
            items[i] = 0;
            invalidate();
            return item;
        }
        ++j;
    }
    return 0;
}

/*!
 * @brief 获取不为空的item数量
 */
int DockLayout::count() const
{
    int result = 0;
    for (int i = 0; i < items.count(); ++i) {
        if (items.at(i))
            ++result;
    }
    return result;
}

QSize DockLayout::minimumSize()
{
    if (items[MyDockWidget::Content] != 0) {
        QSize content;
        if(items[MyDockWidget::Content]->widget()->isVisible()){
            content = items[MyDockWidget::Content]->maximumSize();
        }
        content = sizeFromContent(content, parentWidget()->isWindow());
        qDebug()<<content<<"==new";
        return content;
    } else {
        return parentWidget()->maximumSize();
    }
}

QSize DockLayout::maximumSize()
{
    MyDockWidget *w = qobject_cast<MyDockWidget*>(parentWidget());

    QSize content(0, 0);
    if (items[MyDockWidget::Content] != 0)
        content = items[MyDockWidget::Content]->minimumSize();

    return sizeFromContent(content, w->isFloating());
}

void DockLayout::removeWidget(QWidget *widget)
{
    QVector<QWidgetItem *>::iterator iter = items.begin();
    while(iter != items.end()){
        if((*iter)->widget() == widget){
            delete (*iter)->widget();
        }else{
            iter++;
        }
    }
}

void MyDockWidgetPrivate::init()
{
    QAbstractButton *floatButt = new MyDockWidgetTitleButton(q_ptr);
    floatButt->setFixedSize(22,22);
    floatButt->setObjectName(QLatin1String("dockwidget_floatbutton"));
    QObject::connect(floatButt, SIGNAL(clicked()), q_ptr, SLOT(toggleTopLevel()));
    dockLayout->addWidget(MyDockWidget::FloatButton, floatButt);

    QAbstractButton *closeButt = new MyDockWidgetTitleButton(q_ptr);
    closeButt->setFixedSize(22,22);
    closeButt->setObjectName(QLatin1String("dockwidget_closebutton"));
    QObject::connect(closeButt, SIGNAL(clicked()), q_ptr, SLOT(close()));
    dockLayout->addWidget(MyDockWidget::CloseButton, closeButt);

    QLabel * titleLabel = new QLabel(q_ptr);
    titleLabel->setObjectName(QLatin1String("dockwidget_titlelabel"));
    dockLayout->addWidget(MyDockWidget::TitleLabel, titleLabel);

    toggleViewAction = new QAction(q_ptr);
    toggleViewAction->setCheckable(true);
    toggleViewAction->setChecked(true);
    toggleViewAction->setText(fixedWindowTitle);
    QObject::connect(toggleViewAction, SIGNAL(triggered(bool)),q_ptr, SLOT(toggleView(bool)));

    updateButtons();
}

void MyDockWidgetPrivate::updateButtons()
{
    QStyleOptionDockWidget opt;
    q_ptr->initStyleOption(&opt);

    bool customTitleBar = dockLayout->getWidget(MyDockWidget::TitleBar) != 0;

    bool canClose = hasFeature(this, MyDockWidget::DockWidgetClosable);
    bool canFloat = hasFeature(this, MyDockWidget::DockWidgetFloatable);

    QAbstractButton *button
        = qobject_cast<QAbstractButton*>(dockLayout->getWidget(MyDockWidget::FloatButton));
    button->setIcon(q_ptr->style()->standardIcon(QStyle::SP_TitleBarNormalButton, &opt, q_ptr));
    button->setVisible(canFloat && !customTitleBar);

    button
        = qobject_cast <QAbstractButton*>(dockLayout->getWidget(MyDockWidget::CloseButton));
    button->setIcon(q_ptr->style()->standardIcon(QStyle::SP_TitleBarCloseButton, &opt, q_ptr));
    button->setVisible(canClose && !customTitleBar);

    q_ptr->setAttribute(Qt::WA_ContentsPropagated,(canFloat || canClose) && customTitleBar);
}

bool MyDockWidgetPrivate::mousePress(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        mouseStartPoint = event->pos();
        if(dockLayout->titleHeight() >= event->pos().y()){
            mouseMoveable = true;
        }else{
            mouseMoveable = false;
        }
        globalMouseStartPoint = event->globalPos();
        leftButtonPressed = true;
        mousePressArea = countMouseArea(event->pos(), countHorizonalArea(event->pos()));
    }
    q_ptr->raise();
    return true;
}

bool MyDockWidgetPrivate::mouseMove(QMouseEvent *event)
{
    if(mouseMoveable){
        QPoint newPos = q_ptr->pos() + (event->pos() - mouseStartPoint);
        q_ptr->move(newPos);
        q_ptr->setCursor(Qt::ClosedHandCursor);
        return true;
    }

    int poss = countMouseArea(event->pos(), countHorizonalArea(event->pos()));
//    setCursorType(poss);

    if(leftButtonPressed)
    {
        QPoint ptemp = event->globalPos() - globalMouseStartPoint;

        if (mousePressArea != 22)
        {
            QRect wid = q_ptr->geometry();
            switch (mousePressArea)
            {
                case 11:wid.setTopLeft(wid.topLeft() + ptemp); break;
                case 12:wid.setTop(wid.top() + ptemp.y()); break;
                case 13:wid.setTopRight(wid.topRight() + ptemp); break;

                case 21:wid.setLeft(wid.left() + ptemp.x()); break;
                case 23:wid.setRight(wid.right() + ptemp.x()); break;

                case 32:wid.setBottom(wid.bottom() + ptemp.y()); break;
                case 33:wid.setBottomRight(wid.bottomRight() + ptemp); break;
                case 31:wid.setBottomLeft(wid.bottomLeft() + ptemp); break;
            }
            q_ptr->setGeometry(wid);
        }
         globalMouseStartPoint = event->globalPos();
    }

    return true;
}

int MyDockWidgetPrivate::countMouseArea(QPoint p, int row)
{
  if (p.y() < MOUSE_TRIGGER_SPACE)
      return 10 + row;
  else if (p.y()>q_ptr->height() - MOUSE_TRIGGER_SPACE)
      return 30 + row;
  else
      return 20 + row;
}

/*!
 * @brief 根据鼠标所属的区域设置鼠标样式
 * @param[in] areaCode 区域代码
 */
void MyDockWidgetPrivate::setCursorType(int areaCode)
{
    Qt::CursorShape cursor;
    switch (areaCode)
    {
        case 11:
        case 33:
            cursor = Qt::SizeFDiagCursor; break;
        case 13:
        case 31:
            cursor = Qt::SizeBDiagCursor; break;
        case 21:
        case 23:
            cursor = Qt::SizeHorCursor; break;
        case 12:
        case 32:
            cursor = Qt::SizeVerCursor; break;
        case 22:
            cursor = Qt::ArrowCursor; break;
        default:
            cursor = Qt::ArrowCursor; break;
            break;
    }
    q_ptr->setCursor(cursor);
}

/*!
 * @brief 计算鼠标水平方向所处位置
 * @param[in]  p 鼠标当前的位置
 * @return 区域代码
 */
int MyDockWidgetPrivate::countHorizonalArea(QPoint p)
{
    return (p.x() < MOUSE_TRIGGER_SPACE) ? 1 : ((q_ptr->width() - p.x() < MOUSE_TRIGGER_SPACE) ? 3 : 2);
}

bool MyDockWidgetPrivate::mouseRelease(QMouseEvent *event)
{
    mouseMoveable = false;
    leftButtonPressed = false;
    q_ptr->setCursor(Qt::ArrowCursor);
    return true;
}

bool MyDockWidgetPrivate::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && dockLayout->titleHeight() >= event->pos().y()){
        QWidget * content = dockLayout->getWidget(MyDockWidget::Content);
        if(content){
            contentExpanded = !contentExpanded;
            content->setVisible(contentExpanded);
        }
    }
    return true;
}

MyDockWidget::MyDockWidget(QWidget * parent):QWidget(parent),d_ptr(new MyDockWidgetPrivate(this))
{
    d_ptr->init();
    setMouseTracking(true);
    setFocusPolicy(Qt::ClickFocus);
}

MyDockWidget::~MyDockWidget()
{

}

void MyDockWidget::setFeatures(DockWidgetFeatures features)
{
    Q_D(MyDockWidget);
    if(d->features == features)
        return;
}

MyDockWidget::DockWidgetFeatures MyDockWidget::features() const
{
    Q_D(const MyDockWidget);
    return d->features;
}

void MyDockWidget::setTitle(const QString title)
{
    Q_D(MyDockWidget);
    d->fixedWindowTitle = title;
    d->toggleViewAction->setText(title);
    if(QLabel * titleLabel = dynamic_cast<QLabel*>(d->dockLayout->getWidget(MyDockWidget::TitleLabel))){
        titleLabel->setText(title);
    }
}

void MyDockWidget::setWidget(QWidget *widget)
{
    Q_D(MyDockWidget);
    d->dockLayout->addWidget(MyDockWidget::Content,widget);
}

QWidget *MyDockWidget::widget()
{
    Q_D(MyDockWidget);
    return d->dockLayout->getWidget(MyDockWidget::Content);
}

void MyDockWidget::setFloating(bool floating)
{

}

QAction *MyDockWidget::toggleViewAction() const
{
    Q_D(const MyDockWidget);
    return d->toggleViewAction;
}

bool MyDockWidget::event(QEvent *event)
{
    Q_D(MyDockWidget);
    switch(event->type()){
        case event->MouseButtonPress:
            if(d->mousePress(dynamic_cast<QMouseEvent *>(event)))
                return true;
            break;
        case event->MouseMove:
            if(d->mouseMove(dynamic_cast<QMouseEvent *>(event)))
                return true;
            break;
        case event->MouseButtonRelease:
            if(d->mouseRelease(dynamic_cast<QMouseEvent *>(event)))
                return true;
            break;
        case event->MouseButtonDblClick:
            if(d->mouseDoubleClickEvent(dynamic_cast<QMouseEvent *>(event)))
                return true;
            break;
        case event->FocusIn:
            raise();
            break;
        case event->ZOrderChange:
            break;
    case event->Enter:
        break;
    case event->Leave:
        break;
        default:
            break;
    }
    return QWidget::event(event);
}

void MyDockWidget::initStyleOption(QStyleOptionDockWidget *option) const
{
    Q_D(const MyDockWidget);
    if (!option)
        return;

    DockLayout * dwlayout = qobject_cast<DockLayout*>(layout());

    option->initFrom(this);
    option->rect = dwlayout->titleArea();
    option->title = d->fixedWindowTitle;
    option->closable = hasFeature(this, MyDockWidget::DockWidgetClosable);
    option->movable = hasFeature(this, MyDockWidget::DockWidgetMovable);
    option->floatable = hasFeature(this, MyDockWidget::DockWidgetFloatable);
}

void MyDockWidget::toggleTopLevel()
{

}

void MyDockWidget::toggleView(bool visible)
{
    this->setVisible(visible);
}

}
