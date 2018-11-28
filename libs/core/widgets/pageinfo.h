/*
 * note:表格常用的首页，尾页，上一页，下一页的控件类
 * date:2018/8/28
 * 责任人：杨维维
 * 说明：敬请测试，
 * details:实现与外部的具体操作逻辑解耦
 *         使用说明见每个类前方解释
 */

#ifndef PAGEINFO_H
#define PAGEINFO_H

#include <QWidget>
class QPushButton;
class QSpinBox;

#include "../pluginmanager/observer.h"

namespace Core{

/*!
 * \brief 想要拥有换页按钮的类必须继承这个类并实现upDateInterface方法
 */
class PageInfo
{
public:
    PageInfo(){}
    virtual void upDateSwitchPageInfo(int indexsb)=0;
};

/*!
 * @brief  根据参数进行自动布局的按钮切换控件
 * @details  包括上页，下页，首页，尾页，总行数与当前页数，可进行跳转
 * @cond  使用条件：用户在需要有换页的界面继承pageIno类并实现upDateInterface(int index)函数
 *               在类中新建SwitchPage类的实例
 *                 调用bindCaller进行绑定
 *                 调用switchPage设定总页数
 * @warning 页面数由第1页开始
 */
class SwitchPagePrivate;

class CORESHARED_EXPORT SwitchPage : public QWidget,public Core::Observer
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SwitchPage)
public:
    /*!
    *  @brief 页面跳转显示位置
    */
     enum PageNumAppearPos
     {
         PageNumAppearAtStart,         /*!< 开始 */
         PageNumAppearAtMiddle,        /*!< 中间 */
         PageNumAppearAtEnd            /*!< 结尾 */
     };

     /*!
     *  @brief 显示的控件的数目
     */
     enum DisplayNums
     {
          DisplayPreAndNextNoPageJump,          //仅显示上下页切换按钮且无页面跳转
          DisplayAllNoPageJump,                 //显示首页，上下页，和尾页无页面跳转
          DisplayAllWithPageJump                //显示首页，上下页且支持页面跳转
     };

     SwitchPage(PageNumAppearPos pos = PageNumAppearAtMiddle,Qt::Orientation direction = Qt::Horizontal,DisplayNums buttonNums = DisplayAllWithPageJump,QWidget *parent=0);
     ~SwitchPage();

     void bindCaller(PageInfo *subClass);

     void setTotalPageNums(uint total);
     uint getCurPage();
     uint getLastPage();

     void onMessage(MessageType::MessType type);
     void retranslateUi();

private slots:
     void switchToFirst();
     void switchToPrevious();
     void switchToNext();
     void switchToLast();
     void switchTo();

private:
     void callBindFun();

private:
     SwitchPagePrivate * d_ptr;
};

} //namespace Core

#endif // PAGEINFO_H
