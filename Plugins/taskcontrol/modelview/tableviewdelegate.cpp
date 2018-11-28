#include "TableViewDelegate.h"

#include <QDateTimeEdit>
#include <QDateTime>
#include <QLineEdit>
#include <QDebug>

#include "../head.h"
#include <Base/common/validator/rvalidator.h>

namespace TaskControlModel {

TableViewDelegate::TableViewDelegate(QObject *parent) :
    QItemDelegate(parent)
{
}
TableViewDelegate::~TableViewDelegate()
{
}


QWidget *TableViewDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    switch(static_cast<TaskHead>(index.column())){
        case T_E_TIME:
            {
                QDateTimeEdit *datetime = new QDateTimeEdit(parent);
                return datetime;
            }
            break;
        case T_PARAMETERS:
        case T_E_TIME_LONG:
            {
                QLineEdit * lineEdit = new QLineEdit(parent);
                return lineEdit;
            }
        default:
            break;
    }
    return NULL;
}

void TableViewDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const
{
    switch(static_cast<TaskHead>(index.column())){
        case T_E_TIME:
            {
                QDateTimeEdit *datetime = static_cast<QDateTimeEdit *>(editor);
                if(datetime){
                    datetime->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
                    QString value = index.model()->data(index, Qt::DisplayRole).toString();
                    datetime->setDateTime(QDateTime::fromString(value,"yyyy-MM-dd hh:mm:ss"));
                }
            }
            break;
        case T_PARAMETERS:
        case T_E_TIME_LONG:
            {
                QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
                if(lineEdit){
                    lineEdit->setStyleSheet("background-color:#0D1A2A;color:white;");
                    lineEdit->setText(index.model()->data(index, Qt::DisplayRole).toString());
                }
            }
        default:
            break;
    }
}

void TableViewDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const
{
    switch(static_cast<TaskHead>(index.column())){
        case T_E_TIME:
            {
                QDateTimeEdit *datetime = static_cast<QDateTimeEdit *>(editor);
                if(datetime){
                    model->setData(index, datetime->dateTime().toString("yyyy-MM-dd hh:mm:ss"));
                }
            }
            break;
        case T_PARAMETERS:
            {
                QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
                if(lineEdit){
                    model->setData(index, lineEdit->text());
                }
                break;
            }
        case T_E_TIME_LONG:
            {
                QLineEdit *lineEdit = static_cast<QLineEdit *>(editor);
                if(lineEdit){
                    RNumericValidator<int> validator(lineEdit->text().toInt(),RValid::Gt,0);
                    if(validator.validate() == RValid::Acceptable)
                        model->setData(index, lineEdit->text());
                }
                break;
            }
        default:
            break;
    }
}

void TableViewDelegate::updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &index) const
{
    Q_UNUSED(index);
    if(editor)
        editor->setGeometry(option.rect);
}

} //namespace TaskControlModel

