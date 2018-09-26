/*!
 *  @brief     xls文件操作
 *  @details   XlsGenerateMethod 类提供了对XLS文件的基础功能操作，包括设置工作表、单元格、布局等 @n
 *             TableXLSProxery 类提供基于以表格控件为单元的的操作 @n
 *  @attention 若支持其它数据格式的保存，只需继承XlsGenerateMethod
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.26
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note   使用QActiveX控件时参考的网址
 *       https://blog.csdn.net/psujtfc/article/details/42740761
 *       https://blog.csdn.net/fcqwin/article/details/17885557
 *       https://www.cnblogs.com/chanrom/p/3500264.html
 */
#ifndef XLSFILESAVER_H
#define XLSFILESAVER_H

#include <QThread>
#include <QVariant>

class QAbstractItemModel;
class QTableWidgetItem;
class QTableWidget;
class XlsFileProxery;
class QAxObject;

#include "../base_global.h"

class  BASESHARED_EXPORT  XlsGenerateMethod : public QThread
{
   Q_OBJECT
public:
    virtual ~XlsGenerateMethod();

    void setXlsFie(QString fileName);

    void setTitle(QString title);
    int getRowCount();
    int getColCount();

    void save();
    void close();

    //工作表
    int  getSheetsCount();
    void addWorkSheet(QString sheetName);
    void deleteSheet(const QString& sheetName);
    void deleteSheet(int sheetIndex);
    bool openWorkSheet(int index);
    bool openWorkSheet(QString sheetName);                      ///暂时不可用，detail查看函数注释

    //单元格
    bool     setCellData(uint row, uint column, QVariant data);
    QVariant getCellData(uint row, uint column);                //获取指定单元数据

    void mergeCells(int topLeftRow, int topLeftColumn, int bottomRightRow, int bottomRightColumn);
    void clearCelll(int row,int column);

    //格式布局
    void setRowHeight(int row, int height);
    void setAutoFitRow(int row);                               //自动适应行高
    void setColumnWidth(int column, int width);

    void setCellTextCenter(int row, int column);               //设置单元格居中对齐
    void setCellTextWrap(int row, int column, bool isWrap);    //作用不明
    void setCellFontSize(const QString &cell, int size);       //设置字体大小
    void setCellFontBold(const QString &cell, bool isBold);    //设置粗体

public:
    bool bIsSaveAlready;
    QString mxlsPath;

protected:
    explicit XlsGenerateMethod(bool isVisible);
    explicit XlsGenerateMethod();

private:
    QAxObject *pExcel;                    //指向整个excel应用程序
    QAxObject *pWorkBook;
    QAxObject *pWorkbooks;                //指向工作簿集,excel有很多工作簿
    QAxObject *pSheets;
    QAxObject *pWorksheet;                //指向工作簿中的某个sheet表单
    bool bIsANewFile;
    bool bhasSetFileName;
};

/*!
 * @brief 表格形式的数据存储类
 */
class  BASESHARED_EXPORT  TableXLSProxery:public XlsGenerateMethod
{
public:
    TableXLSProxery(bool isVisible = false);
    virtual ~TableXLSProxery();
    virtual bool ifSetData();               //是否已经设置过数据

    bool addData(QTableWidget *tableWidget);
    bool addData(QAbstractItemModel *model);

    void setXlsFie(QString fileName);

protected:
    virtual void run();

private:
    bool mHasSetFileName;
    bool mifVisible;

   //默认代理支持的数据类型
   QTableWidget *mtable;
   QAbstractItemModel *mModel;
private:
    void WriteData();

};

#endif // XLSFILESAVER_H
