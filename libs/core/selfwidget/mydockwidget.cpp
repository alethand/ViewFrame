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
#include <qmath.h>
#include <QDebug>
#include <QMenu>
#include <QMap>
#include <QDrag>
#include <QMimeData>

#include "Base/util/rutil.h"
#include "Base/util/widgetpropsetting.h"

namespace Core{

class MyDockWidgetPrivate
{
    Q_DECLARE_PUBLIC(MyDockWidget)
public:
    MyDockWidgetPrivate(MyDockWidget * q):q_ptr(q), content(NULL),mouseMoveable(false),rightPopMenu(NULL){
        dockLayout = new DockLayout(q);
        dockLayout->setContentsMargins(1,1,1,1);
    }

    enum WindowDirection{
        W_Left = 0x01,
        W_Right,
        W_Bottom
    };

    void init();
    void updateButtons();

    bool mousePress(QMouseEvent * event);
    bool mouseMove(QMouseEvent * event);
    bool mouseRelease(QMouseEvent * event);
    bool mouseDoubleClickEvent(QMouseEvent * event);
    bool contextMenu(QContextMenuEvent * event);

    void setExpand(bool expanded);

    QString fixedWindowTitle;
    QPoint mouseStartPoint;     /*!< 鼠标按下位置 */
    QWidget * titleBar;         /*!< 标题栏,可只显示图标 */
    QWidget * content;          /*!< 内容区,可折叠 */
    bool mouseMoveable;         /*!< 鼠标点下的位置是否支持移动 */

    QMenu * rightPopMenu;       /*!< 标题栏右键菜单 */
    QMap<Widget::WidgetFeature,QAction *> popActions;  /*!< 标题栏弹出按钮 */

    QAction *toggleViewAction;

    QRect currentGeometry;      /*!< 未压缩显示时窗口尺寸 */

    MyDockWidget * q_ptr;
    DockLayout * dockLayout;
};

static inline bool hasFeature(const MyDockWidgetPrivate *priv, Widget::WidgetFeatures feature)
{ return (priv->q_ptr->getWidgetFeatures() & feature) == feature; }

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

        q->setMinimumSize(minimumTitleWidth(),titleHeight);

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
            int topPos = 0;
            int itemSpace = 1;

            if (QLayoutItem *item = items[MyDockWidget::CloseButton]) {
                if (!item->isEmpty()) {
                    if(verticalTitleBar){
                        item->setGeometry(QRect(0,topPos,item->widget()->width(),item->widget()->height()));
                        topPos += item->widget()->height();
                        topPos += itemSpace;
                    }
                    else
                    {
                        leftPos -= item->widget()->width();
                        leftPos -= itemSpace;
                        item->setGeometry(QRect(leftPos,0,item->widget()->width(),item->widget()->height()));
                    }
                }
            }

            if (QLayoutItem *item = items[MyDockWidget::MoveButton]) {
                if (!item->isEmpty()) {
                    if(verticalTitleBar){
                        item->setGeometry(QRect(0,topPos,item->widget()->width(),item->widget()->height()));
                        topPos += item->widget()->height();
                        topPos += itemSpace;
                    }
                    else
                    {
                        leftPos -= item->widget()->width();
                        leftPos -= itemSpace;
                        item->setGeometry(QRect(leftPos,0,item->widget()->width(),item->widget()->height()));
                    }
                }
            }

            if(QLayoutItem *item = items[MyDockWidget::TitleLabel]){
                if (!item->isEmpty()) {
                    QLabel * label = dynamic_cast<QLabel *>(item->widget());
                    if(label)
                        label->setWordWrap(verticalTitleBar);

                    if(verticalTitleBar){
                        item->setGeometry(QRect(0,topPos,_titleArea.width(),_titleArea.height() - topPos));
                    }
                    else
                    {
                        leftPos -= itemSpace;
                        item->setGeometry(QRect(0,0,leftPos,_titleArea.height()));
                    }
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
    if (q->testFeatures(Widget::WidgetClosable)) {
        if (QLayoutItem *item = items[MyDockWidget::CloseButton])
            closeSize = item->widget()->sizeHint();
    }
    if (q->testFeatures(Widget::WidgetMovable)) {
        if (QLayoutItem *item = items[MyDockWidget::MoveButton])
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
    if (QLayoutItem *item = items[MyDockWidget::MoveButton])
        floatSize = item->widget()->sizeHint();

    int buttonHeight = qMin(perp(verticalTitleBar, closeSize),
                            perp(verticalTitleBar, floatSize));
    QFontMetrics titleFontMetrics = q->fontMetrics();
    int mw = q->style()->pixelMetric(QStyle::PM_DockWidgetTitleMargin, 0, q);

    return qMax(buttonHeight , titleFontMetrics.height() + 2*mw);
}

int DockLayout::titleWidth() const
{
    MyDockWidget *q = qobject_cast<MyDockWidget*>(parentWidget());

    if (QWidget *title = getWidget(MyDockWidget::TitleBar))
        return perp(verticalTitleBar, title->sizeHint());

    QSize closeSize(0, 0);
    QSize floatSize(0, 0);
    if (QLayoutItem *item = items[MyDockWidget::CloseButton])
        closeSize = item->widget()->sizeHint();
    if (QLayoutItem *item = items[MyDockWidget::MoveButton])
        floatSize = item->widget()->sizeHint();

    int buttonWidth = qMax(perp(verticalTitleBar, closeSize),
                            perp(verticalTitleBar, floatSize));
    return buttonWidth;
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

    return sizeFromContent(content, w->isFloating());
}

void DockLayout::setVertical(bool flag)
{
    verticalTitleBar = flag;
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
            content = items[MyDockWidget::Content]->minimumSize();
        }
        content = sizeFromContent(content, parentWidget()->isWindow());
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
        content = items[MyDockWidget::Content]->maximumSize();

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
    QAbstractButton * moveButt = new MyDockWidgetTitleButton(q_ptr);
    moveButt->setFixedSize(22,22);
    moveButt->setObjectName(QLatin1String("dockwidget_floatbutton"));
    QObject::connect(moveButt, SIGNAL(clicked()), q_ptr, SLOT(toggleTopLevel()));
    dockLayout->addWidget(MyDockWidget::MoveButton, moveButt);

    QAbstractButton *closeButt = new MyDockWidgetTitleButton(q_ptr);
    closeButt->setFixedSize(22,22);
    closeButt->setObjectName(QLatin1String("dockwidget_closebutton"));
    QObject::connect(closeButt, SIGNAL(clicked()), q_ptr, SLOT(hideWidget()));
    dockLayout->addWidget(MyDockWidget::CloseButton, closeButt);

    QLabel * titleLabel = new QLabel(q_ptr);
    titleLabel->setObjectName(QLatin1String("dockwidget_titlelabel"));
    dockLayout->addWidget(MyDockWidget::TitleLabel, titleLabel);

    toggleViewAction = new QAction(q_ptr);
    toggleViewAction->setCheckable(true);
    toggleViewAction->setChecked(true);
    toggleViewAction->setText(fixedWindowTitle);
    QObject::connect(toggleViewAction, SIGNAL(triggered(bool)),q_ptr, SLOT(toggleView(bool)));
    QObject::connect(toggleViewAction, SIGNAL(toggled(bool)),q_ptr, SLOT(toggleView(bool)));

    /**********右键弹出菜单************/
    rightPopMenu = new QMenu();
    Base::WidgetPropSetting::enableWidgetTransparency(rightPopMenu,true);

    QAction * action = new QAction(rightPopMenu);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(triggerMoveable(bool)));
    action->setCheckable(true);
    popActions.insert(Widget::WidgetMovable,action);

    action = new QAction(rightPopMenu);
    action->setCheckable(true);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(triggerResizeable(bool)));
    popActions.insert(Widget::WidgetResizeable,action);

    action = new QAction(rightPopMenu);
    action->setCheckable(true);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(triggerExpanable(bool)));
    popActions.insert(Widget::WidgetExpanable,action);

    action = new QAction(rightPopMenu);
    action->setCheckable(true);
    QObject::connect(action, SIGNAL(triggered(bool)), q_ptr, SLOT(hideWidget()));
    popActions.insert(Widget::WidgetVisible,action);

    QList<QAction *> valueList = popActions.values();
    std::for_each(valueList.begin(),valueList.end(),[&](QAction * action){
        rightPopMenu->addAction(action);
    });

    updateButtons();
}

void MyDockWidgetPrivate::updateButtons()
{
    QStyleOptionDockWidget opt;
    q_ptr->initStyleOption(&opt);

    bool customTitleBar = dockLayout->getWidget(MyDockWidget::TitleBar) != 0;

    bool canClose = hasFeature(this, Widget::WidgetClosable);
    bool canMove = hasFeature(this, Widget::WidgetMovable);

    QAbstractButton *button = qobject_cast<QAbstractButton*>(dockLayout->getWidget(MyDockWidget::MoveButton));
    QIcon icon;
    if(canMove)
        icon = QIcon(":/tech/resource/technology/unlocked.png");
    else
        icon = QIcon(":/tech/resource/technology/Lock_Locked.png");

    button->setIcon(icon);
    button->setVisible(!customTitleBar);

    button = qobject_cast <QAbstractButton*>(dockLayout->getWidget(MyDockWidget::CloseButton));
    button->setIcon(QIcon(":/tech/resource/technology/dock_close.png"));
//    button->setIcon(q_ptr->style()->standardIcon(QStyle::SP_TitleBarCloseButton, &opt, q_ptr));
    button->setVisible(canClose && !customTitleBar);

    q_ptr->setAttribute(Qt::WA_ContentsPropagated,(canMove || canClose) && customTitleBar);
}

bool MyDockWidgetPrivate::mousePress(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        mouseStartPoint = event->pos();
        if(q_ptr->testFeatures(Widget::WidgetMovable)){
            if(q_ptr->testFeatures(Widget::WidgetVerticalTitleBar)){
                if(dockLayout->titleWidth() >= event->pos().x()){
                    mouseMoveable = true;
                }else{
                    mouseMoveable = false;
                }
            }else{
                if(dockLayout->titleHeight() >= event->pos().y()){
                    mouseMoveable = true;
                }else{
                    mouseMoveable = false;
                }
            }
        }

    }
    q_ptr->raise();
    return true;
}

/*!
 * @brief 处理dock移动事件
 * @details 1.若设置了WidgetRangeLimitf属性，用于控制dock移动过程中任一边不能移出屏幕显示区域.
 *            若检测到任一边超过了屏幕范围，程序及时修复对应边移动的大小。 @n
 *          2.若设置了WidgetMovable属性，点击titleBar区域时，dock在处理完事件后，会终止事件流的传播，防止与widget中mousemove事件冲突，造成窗口移动混乱。
 *
 *          其它eature的处理，可参照父类widget的mousemove处理。
 * @param[in] event 鼠标事件
 * @return 是否终止事件流传播
 */
bool MyDockWidgetPrivate::mouseMove(QMouseEvent *event)
{
    if(mouseMoveable){
        QPoint newPos = q_ptr->pos() + (event->pos() - mouseStartPoint);

        if(q_ptr->testFeatures(Widget::WidgetRangeLimit)){
            QRect newRect(newPos,q_ptr->size());
            QRect screenSize = RUtil::screenGeometry();
            if(!screenSize.contains(newRect)){
                if(newPos.x() < screenSize.x())
                    newPos.setX(screenSize.x());
                else if(newPos.x() > (screenSize.width() - newRect.width()))
                    newPos.setX(screenSize.width() - newRect.width());

                if(newPos.y() < screenSize.y())
                    newPos.setY(screenSize.y());
                else if(newPos.y() > (screenSize.height() - newRect.height()))
                    newPos.setY(screenSize.height() - newRect.height());
            }
        }

        q_ptr->move(newPos);
        q_ptr->setCursor(Qt::ClosedHandCursor);
        return true;
    }
    return false;
}

bool MyDockWidgetPrivate::mouseRelease(QMouseEvent *event)
{
    mouseMoveable = false;
    q_ptr->setCursor(Qt::ArrowCursor);
    return true;
}

/*!
 * @brief 处理双击titlebar区域事件
 * @param[in] event 鼠标事件
 * @return 是否终止事件传播流程
 */
bool MyDockWidgetPrivate::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton && dockLayout->titleHeight() >= event->pos().y()){
        q_ptr->triggerExpanable(false);
    }
    return true;
}

bool MyDockWidgetPrivate::contextMenu(QContextMenuEvent *event)
{
    popActions[Widget::WidgetMovable]->setText(QObject::tr("moveable"));
    popActions[Widget::WidgetResizeable]->setText(QObject::tr("resizeable"));
    popActions[Widget::WidgetExpanable]->setText(QObject::tr("expanded"));
    popActions[Widget::WidgetVisible]->setText(QObject::tr("visible"));

    rightPopMenu->exec(event->globalPos());
    return true;
}

/*!
 * @brief 设置内容显示区是否扩展显示
 * @details 1.双击titlebar区域可控制内容区显示或隐藏; @n
 *          2.隐藏时根据隐藏前与屏幕四周的距离进行计算停靠; @n
 *              a.未隐藏前，四边与屏幕之间三边之一(左、右、下)之间的间距小于10px，将控件停靠在最小的一边；
 *                若存在距离一致的情况，优先级为下>左>右
 *              b.在其它区域隐藏时，则在原处展开。
 *                若在隐藏时被拖动至三边(左、右、下)之一时，展开依据与a中一致。
 *          3.隐藏展开时：
 *              a.若未移动，则直接在原地展开；
 *              b.若移动后，先判断若直接展开其尺寸是否会超过屏幕，若超过则在超过的方向反向减去一定值；
 */
void MyDockWidgetPrivate::setExpand(bool expanded)
{
    QWidget * content = dockLayout->getWidget(MyDockWidget::Content);
    if(!content || !q_ptr->testFeatures(Widget::WidgetExpanable))
        return;

    typedef std::pair<WindowDirection,int> DirPair;
    std::vector<DirPair> paris;

    QRect screenSize = RUtil::screenGeometry();

    if(expanded){
        QPoint newPoint = q_ptr->pos();
        QSize newSize(currentGeometry.width(),newPoint.y() + currentGeometry.height());
        if(newSize.height() > screenSize.height()){
            newPoint.setY(newPoint.y() + screenSize.height() - newSize.height());
        }
        q_ptr->setGeometry(QRect(newPoint,currentGeometry.size()));

    }else{
        int hotArea = 10;

        if(dockLayout->verticalTitleBar){
            paris.push_back({W_Bottom,abs(screenSize.bottom() - q_ptr->geometry().bottom())});
            paris.push_back({W_Left,abs(screenSize.left() - q_ptr->geometry().left())});
            paris.push_back({W_Right,abs(screenSize.right() - q_ptr->geometry().right())});

            auto index = std::min_element(paris.begin(),paris.end(),[&](DirPair & a,DirPair & b){
                return a.second < b.second;
            });
            DirPair minDir = *index;
            QPoint pos;

            //靠边停靠
            if(minDir.second - hotArea <= 0 ){
                switch(minDir.first){
                    case W_Left:
                            pos = QPoint(screenSize.left(),q_ptr->y());
                        break;
                    case W_Right:
                            pos = QPoint(screenSize.right() - q_ptr->width(),q_ptr->y());
                        break;
                    case W_Bottom:
                            pos = QPoint(q_ptr->x(),screenSize.bottom()-q_ptr->minimumHeight());
                        break;
                    default:
                        break;
                }
            }else{
                pos = q_ptr->pos();
            }
            q_ptr->setGeometry(QRect(pos,QSize(q_ptr->width(),q_ptr->minimumHeight())));
        }else{
            paris.push_back({W_Bottom,abs(screenSize.bottom() - q_ptr->geometry().bottom())});
            paris.push_back({W_Left,abs(screenSize.left() - q_ptr->geometry().left())});
            paris.push_back({W_Right,abs(screenSize.right() - q_ptr->geometry().right())});

            auto index = std::min_element(paris.begin(),paris.end(),[&](DirPair & a,DirPair & b){
                return a.second < b.second;
            });
            DirPair minDir = *index;
            QPoint pos;

            //靠边停靠
            if(minDir.second - hotArea <= 0 ){
                switch(minDir.first){
                    case W_Left:
                            pos = QPoint(screenSize.left(),q_ptr->y());
                        break;
                    case W_Right:
                            pos = QPoint(screenSize.right() - q_ptr->width(),q_ptr->y());
                        break;
                    case W_Bottom:
                            pos = QPoint(q_ptr->x(),screenSize.bottom()-q_ptr->minimumHeight());
                        break;
                    default:
                        break;
                }
            }else{
                pos = q_ptr->pos();
            }
            q_ptr->setGeometry(QRect(pos,QSize(q_ptr->width(),q_ptr->minimumHeight())));
        }
    }
    content->setVisible(expanded);
}

MyDockWidget::MyDockWidget(QWidget * parent):Widget(parent),d_ptr(new MyDockWidgetPrivate(this))
{
    d_ptr->init();
    setFocusPolicy(Qt::ClickFocus);
    setWidgetFeatures(currentFeatures |= WidgetRangeLimit);
//    setWidgetFeatures(currentFeatures |= WidgetVerticalTitleBar );

    setAcceptDrops(true);
}

MyDockWidget::~MyDockWidget()
{

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

void MyDockWidget::setGeometry(const QRect &rect)
{
    Q_D(MyDockWidget);
    if(widgetExpanded)
        d->currentGeometry = rect;

    Widget::setGeometry(rect);
}

QRect MyDockWidget::getGeometry() const
{
    Q_D(const MyDockWidget);
    if(widgetExpanded){
        return geometry();
    }else{
        return d->currentGeometry;
    }
}

bool MyDockWidget::event(QEvent *event)
{
    Q_D(MyDockWidget);
    switch(event->type()){
        case event->MouseButtonPress:
                d->mousePress(dynamic_cast<QMouseEvent *>(event));
            break;
        case event->MouseMove:
                if(d->mouseMove(dynamic_cast<QMouseEvent *>(event)))
                    return true;
            break;
        case event->MouseButtonRelease:
                d->mouseRelease(dynamic_cast<QMouseEvent *>(event));
            break;
        case event->MouseButtonDblClick:
            if(d->mouseDoubleClickEvent(dynamic_cast<QMouseEvent *>(event)))
                return true;
            break;
        case event->ContextMenu:
                return d->contextMenu(dynamic_cast<QContextMenuEvent *>(event));
            break;
        case event->FocusIn:
            raise();
            break;
        case event->Resize:
            if(widgetExpanded)
                d->currentGeometry = geometry();
            break;
        default:
            break;
    }
    return Widget::event(event);
}

/*!
 * @brief 根据dock具有的feature特点，对控件进行设置
 * @details 1.对控件显示状态进行设置 @n
 *          2.对控件的移动、尺寸属性进行控制; @n
 *          3.对控件的可嵌入属性控制; @n
 */
void MyDockWidget::updateFeatures()
{
    Q_D(MyDockWidget);

    d->toggleViewAction->setChecked(testFeatures(WidgetVisible));

    d->popActions[Widget::WidgetVisible]->setChecked(testFeatures(WidgetVisible));
    d->popActions[Widget::WidgetResizeable]->setChecked(testFeatures(WidgetResizeable));
    d->popActions[Widget::WidgetMovable]->setChecked(testFeatures(WidgetMovable));
    d->popActions[Widget::WidgetExpanable]->setChecked(widgetExpanded);

    d->setExpand(widgetExpanded);
    d->dockLayout->setVertical(testFeatures(WidgetVerticalTitleBar));
    d->updateButtons();
}

void MyDockWidget::initStyleOption(QStyleOptionDockWidget *option)
{
    Q_D(const MyDockWidget);
    if (!option)
        return;

    DockLayout * dwlayout = qobject_cast<DockLayout*>(layout());

    option->initFrom(this);
    option->rect = dwlayout->titleArea();
    option->title = d->fixedWindowTitle;
    option->closable = testFeatures(Widget::WidgetClosable);
    option->movable = testFeatures(Widget::WidgetMovable);
    option->floatable = testFeatures(Widget::WidgetMovable);
}

void MyDockWidget::toggleTopLevel()
{
    Q_D(MyDockWidget);
    triggerMoveable(!testFeatures(Widget::WidgetMovable));
    d->popActions[Widget::WidgetMovable]->setChecked(testFeatures(Widget::WidgetMovable));
}

void MyDockWidget::toggleView(bool visible)
{
    Q_D(MyDockWidget);
    this->setVisible(visible);
    d->popActions[Widget::WidgetVisible]->setChecked(visible);
    if(visible)
        currentFeatures |= Widget::WidgetVisible;
    else
        currentFeatures &= ~Widget::WidgetVisible;
}

void MyDockWidget::triggerMoveable(bool moveable)
{
    Q_D(MyDockWidget);
    if(moveable)
        currentFeatures |= Widget::WidgetMovable;
    else
        currentFeatures &= ~Widget::WidgetMovable;
    d->updateButtons();
}

void MyDockWidget::triggerResizeable(bool resizeable)
{
    Q_D(MyDockWidget);
    if(resizeable)
        currentFeatures |= Widget::WidgetResizeable;
    else
        currentFeatures &= ~Widget::WidgetResizeable;
}

void MyDockWidget::triggerExpanable(bool /*expanded*/)
{
    Q_D(MyDockWidget);
    if(widgetExpanded)
        d->currentGeometry = geometry();
     widgetExpanded = !widgetExpanded;
     d->setExpand(widgetExpanded);
     d->popActions[Widget::WidgetExpanable]->setChecked(widgetExpanded);
}

void MyDockWidget::hideWidget()
{
    Q_D(MyDockWidget);
    d->toggleViewAction->setChecked(false);
}

}
