#include "pageinfo.h"

#include <QPushButton>
#include <QSpinBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QDebug>

#include "Base/util/rsingleton.h"

class SwitchPagePrivate
{
    Q_DECLARE_PUBLIC(SwitchPage)
private:
    SwitchPagePrivate(SwitchPage * q,SwitchPage::PageNumAppearPos pos, Qt::Orientation direction, SwitchPage::DisplayNums buttonNums):q_ptr(q),
        mOrientation(direction),mNumPos(pos),mButNums(buttonNums),mlastPage(1),mtotalPage(1),mcurPage(1),hostClass(NULL)
    {
        initView();
    }

    void initView();
    void genetateLayout();

    SwitchPage * q_ptr;

    PageInfo  * hostClass;                  /*!< 继承pageInfo的类 */
    QSpinBox  * mspinBox;
    Qt::Orientation mOrientation;           /*!< 切换页出现的位置   横向/纵向 */
    SwitchPage::PageNumAppearPos mNumPos;   /*!< 页数显示出现的位置 */
    SwitchPage::DisplayNums     mButNums;   /*!< 按钮显示数目 */

    unsigned int mtotalPage;           /*!< 总页数 */
    unsigned int mcurPage;             /*!< 当前页数 */
    unsigned int mlastPage;            /*!< 上次的页数 */

    QPushButton *firstPage;
    QPushButton *prePage;
    QPushButton *nextPage;
    QPushButton *lastPage;
    QPushButton *jumpPage;
};

void SwitchPagePrivate::initView()
{
    int fixedButtWidth = 80;
    firstPage = new QPushButton();
    prePage = new QPushButton();
    nextPage = new QPushButton();
    lastPage = new QPushButton();
    jumpPage = new QPushButton();

    firstPage->setFixedWidth(fixedButtWidth);
    prePage->setFixedWidth(fixedButtWidth);
    nextPage->setFixedWidth(fixedButtWidth);
    lastPage->setFixedWidth(fixedButtWidth);
    jumpPage->setFixedWidth(fixedButtWidth);

    mspinBox = new QSpinBox();
    mspinBox->setFixedWidth(40);
    mspinBox->setAlignment(Qt::AlignCenter);
    mspinBox->setMinimumHeight(22);
    mspinBox->setRange(1,1);
    mspinBox->setSuffix("/1");
    mspinBox->setButtonSymbols(QAbstractSpinBox::NoButtons);

    prePage->setEnabled(false);
    nextPage->setEnabled(false);

    genetateLayout();

    QObject::connect(firstPage,SIGNAL(pressed()),q_ptr,SLOT(switchToFirst()));
    QObject::connect(prePage,SIGNAL(pressed()),q_ptr,SLOT(switchToPrevious()));
    QObject::connect(nextPage,SIGNAL(pressed()),q_ptr,SLOT(switchToNext()));
    QObject::connect(lastPage,SIGNAL(pressed()),q_ptr,SLOT(switchToLast()));
    QObject::connect(jumpPage,SIGNAL(pressed()),q_ptr,SLOT(switchTo()));
}

void SwitchPagePrivate::genetateLayout()
{
    QBoxLayout *layout = NULL;
    if(mOrientation == Qt::Horizontal)
        layout = new QHBoxLayout();
    else
        layout = new QVBoxLayout();

    layout->addStretch(1);
    switch(mNumPos)
    {
        case SwitchPage::PageNumAppearAtStart:
                layout->addWidget(mspinBox);
                layout->addWidget(jumpPage);
                layout->addWidget(firstPage);
                layout->addWidget(prePage);
                layout->addWidget(nextPage);
                layout->addWidget(lastPage);
                break;
        case SwitchPage::PageNumAppearAtMiddle:
                layout->addWidget(firstPage);
                layout->addWidget(prePage);

                layout->addWidget(mspinBox);
                layout->addWidget(jumpPage);

                layout->addWidget(nextPage);
                layout->addWidget(lastPage);
                break;
        case SwitchPage::PageNumAppearAtEnd:
                layout->addWidget(firstPage);
                layout->addWidget(prePage);
                layout->addWidget(nextPage);
                layout->addWidget(lastPage);

                layout->addWidget(mspinBox);
                layout->addWidget(jumpPage);
                break;
        default:
            break;
    }
    layout->addStretch(1);
    layout->setContentsMargins(0,0,0,0);
    q_ptr->setLayout(layout);

    switch(mButNums)
    {
        case SwitchPage::DisplayPreAndNextNoPageJump:
             firstPage->hide();
             lastPage->hide();
             jumpPage->hide();
             break;
        case SwitchPage::DisplayAllNoPageJump:
             jumpPage->hide();
             break;
        default:
            break;
    }
}

SwitchPage::SwitchPage(SwitchPage::PageNumAppearPos pos, Qt::Orientation direction, DisplayNums buttonNums, QWidget *parent)
    :QWidget(parent),d_ptr(new SwitchPagePrivate(this,pos,direction,buttonNums))
{
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    RSingleton<Core::Subject>::instance()->attach(this);
    retranslateUi();
}

SwitchPage::~SwitchPage()
{

}

/*!
 * \brief SwitchPage::bindCaller  绑定调用者，只有调用这个函数才能知道换页所影响的到底是哪一个继承pageInfo的子类
 * \param subClass  继承pageInfo需要进行换页按钮显示的子类
 */
void SwitchPage::bindCaller(PageInfo *subClass)
{
    Q_D(SwitchPage);
    d->hostClass = subClass;
}

/*!
 * @brief 设置总显示页数
 * @param[in] total 总页数
 */
void SwitchPage::setTotalPageNums(uint total)
{
    Q_D(SwitchPage);
    if(d->mtotalPage == total || total < 1)
        return;

    d->mtotalPage = total;
    if(total < d->mcurPage)
    {
        d->mcurPage = d->mtotalPage;
        d->nextPage->setEnabled(false);
     }
    else
        d->nextPage->setEnabled(true);

    if(d->mcurPage == 1)
        d->prePage->setEnabled(false);
    else
        d->prePage->setEnabled(true);
    d->mspinBox->setRange(1,d->mtotalPage);
    d->mspinBox->setSuffix(QString("/ %1").arg(d->mtotalPage));
}

uint SwitchPage::getCurPage()
{
    Q_D(SwitchPage);
    return d->mcurPage;
}

uint SwitchPage::getLastPage()
{
    Q_D(SwitchPage);
    return d->mlastPage;
}

void SwitchPage::onMessage(MessageType::MessType type)
{
    switch(type){
        case MessageType::MESS_LAN_CHANGED:
            retranslateUi();
            break;
        default:
            break;
    }
}

void SwitchPage::retranslateUi()
{
    Q_D(SwitchPage);
    d->firstPage->setText(QObject::tr("First page"));
    d->prePage->setText(QObject::tr("Pre page"));
    d->nextPage->setText(QObject::tr("Next page"));
    d->lastPage->setText(QObject::tr("Last page"));
    d->jumpPage->setText(QObject::tr("Jump page"));
}

void SwitchPage::switchToFirst()
{
    Q_D(SwitchPage);
    d->mlastPage =  d->mcurPage;
    d->mcurPage = 1;
    d->prePage->setEnabled(false);
    d->nextPage->setEnabled(d->mtotalPage != 1);
    d->mspinBox->setValue(d->mcurPage);
    callBindFun();
}

void SwitchPage::switchToPrevious()
{
    Q_D(SwitchPage);
    d->mlastPage = d->mcurPage;
    d->nextPage->setEnabled(true);
    if(d->mcurPage - 1 > 1)
    {
        d->mcurPage--;
        d->prePage->setEnabled(true);
    }
    else
    {
        d->mcurPage = 1;
        d->prePage->setEnabled(false);
    }
    d->mspinBox->setValue(d->mcurPage);
    callBindFun();
}

void SwitchPage::switchToNext()
{
    Q_D(SwitchPage);
    d->mlastPage = d->mcurPage;
    d->prePage->setEnabled(true);
    if(d->mcurPage + 1 < d->mtotalPage)
    {
        d->mcurPage++;
        d->nextPage->setEnabled(true);
    }
    else
    {
        d->mcurPage = d->mtotalPage;
        d->nextPage->setEnabled(false);
    }
    d->mspinBox->setValue(d->mcurPage);
    callBindFun();

}

void SwitchPage::switchToLast()
{
    Q_D(SwitchPage);
    d->mlastPage = d->mcurPage;
    d->mcurPage = d->mtotalPage;
    d->nextPage->setEnabled(false);
    d->prePage->setEnabled(d->mtotalPage != 1);
    d->mspinBox->setValue(d->mcurPage);
    callBindFun();
}

void SwitchPage::switchTo()
{
    Q_D(SwitchPage);
    d->mlastPage = d->mcurPage;
    d->mcurPage = d->mspinBox->value();
    if(1 != d->mtotalPage)
    {
        if(1== d->mcurPage)
        {
            d->prePage->setEnabled(false);
            d->nextPage->setEnabled(true);
        }
        else
            if(d->mtotalPage == d->mcurPage)
            {
                d->nextPage->setEnabled(false);
                d->prePage->setEnabled(true);
            }
            else
            {
                d->nextPage->setEnabled(true);
                d->prePage->setEnabled(true);
            }
    }
    callBindFun();
}

void SwitchPage::callBindFun()
{
    Q_D(SwitchPage);
    if(d->hostClass)
        d->hostClass->upDateSwitchPageInfo(d->mcurPage);
}
