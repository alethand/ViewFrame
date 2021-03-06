﻿#include "taskdialogproxy.h"

#include <QMessageBox>
#include <QComboBox>
#include <QCheckBox>
#include <QRadioButton>
#include <QTextEdit>
#include <QDateEdit>
#include <QTimeEdit>
#include <QFileInfo>

#include "tasklayout.h"
#include <base/common/validator/rcombinevalidator.h>

namespace TaskControlModel {

class TaskDialogProxyPrivate
{
    Q_DECLARE_PUBLIC(TaskDialogProxy)
private:
    TaskDialogProxyPrivate(TaskDialogProxy * q):q_ptr(q),clickedButt(DialogProxy::NoButton),parseResult(false),taskInfo(nullptr){

    }

    TaskLayoutParse * limitDisplay;
    bool parseResult;
    DialogProxy::StandardButton clickedButt;
    QString parsedFileName;         /*!< 解析好的文件名，没有路径和后缀 */
    QString parsedFileFullName;     /*!< 全路径文件名 */
    NewTaskInfo * taskInfo;
    TaskDialogProxy * q_ptr;
};


TaskDialogProxy::TaskDialogProxy(QWidget *parent)
    :d_ptr(new TaskDialogProxyPrivate(this)),DialogProxy(parent)
{
    setButton(DialogProxy::Ok|DialogProxy::Cancel);
}

TaskDialogProxy::~TaskDialogProxy()
{
    Q_D(TaskDialogProxy);
    if(d->parseResult){
        delete d->limitDisplay;
    }

//    if(d->taskInfo){
//        delete d->taskInfo;
//    }
}

/*!
 * @brief 解析对应任务布局文件
 * @note 若文件解析失败，则窗口禁止显示。
 * @param[in] fileName 待解析的文件名称
 * @return 是否解析成功
 */
bool TaskDialogProxy::parseLayout(QString &fileName)
{
    Q_D(TaskDialogProxy);
    d->limitDisplay = new TaskLayoutParse();

    Base::RXmlFile file(fileName);
    file.setParseMethod(d->limitDisplay,false);
    d->parseResult = file.startParse();

    if(d->parseResult){
        QFileInfo info(fileName);
        d->parsedFileName = info.baseName();
        d->parsedFileFullName = fileName;
    }

    return d->parseResult;
}

void TaskDialogProxy::showMe()
{
    Q_D(TaskDialogProxy);
    if(d->parseResult){
        setContentWidget(d->limitDisplay->getContainer()->continer.widget);
        setTitle(d->parsedFileName);
        exec();
    }
}

void TaskDialogProxy::setTaskInfo(NewTaskInfo *info)
{
    Q_D(TaskDialogProxy);
    if(d->parseResult){
        d->taskInfo = info;
        Core::Datastruct::Container * container = d->limitDisplay->getContainer();
        fillFiled(container);
    }
}

/*!
 * @brief 填充数据字段
 * @details 遍历当前布局中的每个字段，根据创建时指定的index，查找taskinfo中对应的值，若可以找到 @n
 *          则将值设置至对应的控件中。
 * @param[in]  container 待填充的容器
 */
void TaskDialogProxy::fillFiled(Core::Datastruct::Container *container)
{
    Q_D(TaskDialogProxy);
    if(container->childContainer.size() == 0){
        std::for_each(container->fileds.begin(),container->fileds.end(),[&](Core::Datastruct::Field * filed){
            Core::Datastruct::Data_Word curFieldDesc = d->taskInfo->fields.value(filed->data.index);

        });
    }else{
        std::for_each(container->childContainer.begin(),container->childContainer.end(),[&](Core::Datastruct::Container * cont){
            fillFiled(cont);
        });
    }
}

NewTaskInfo *TaskDialogProxy::getTaskInfo()
{
    Q_D(TaskDialogProxy);
    return d->taskInfo;
}

void TaskDialogProxy::respButtClicked(DialogProxy::StandardButton butt)
{
    Q_D(TaskDialogProxy);
    d->clickedButt = butt;
    switch(butt){
        case DialogProxy::Ok:
                respOk();
            break;
        case DialogProxy::Cancel:
                respCancel();
            break;
        default:
            break;
    }
}

/*!
 * @brief 确认添加新的任务
 * @details 1.检查字段值的正确性;
 *          2.按照顺序保存各个字段值
 */
void TaskDialogProxy::respOk()
{
    Q_D(TaskDialogProxy);

    d->taskInfo = new NewTaskInfo;
    d->taskInfo->excuteTime = QDateTime::currentDateTime();
    d->taskInfo->lastTime = 1;
    d->taskInfo->taskName = d->parsedFileName;
    d->taskInfo->localParsedFileName = d->parsedFileFullName;

    Core::Datastruct::Container * container = d->limitDisplay->getContainer();
    findFiled(container);

    QMap<int,Core::Datastruct::Data_Word>::iterator iter = d->taskInfo->fields.begin();
    RAndCombineValidator validator;
    //TODO 20181102对字段详细信息进行验证
    while(iter != d->taskInfo->fields.end()){
        Core::Datastruct::Data_Word data = iter.value();
        switch(data.type){
            case Core::Datastruct::ValueIntEdit:
                validator.addValidator(new RNumericValidator<int>(data.value.toInt(),RValid::Ge,0));
                break;
            //待扩充对不同的类型验证
            default:
                break;
        }
        iter++;
    }

//    if(validator.validate() == RValid::Invalid){
//        QMessageBox::warning(this,tr("warning"),tr("Input information validation failed!"),QMessageBox::Yes);
//        return;
//    }
    respCancel();
}

/*!
 * @brief 查找容器中存在的字段，并按照索引保存
 * @param[in] container 待查找的容器
 */
void TaskDialogProxy::findFiled(Core::Datastruct::Container * container)
{
    Q_D(TaskDialogProxy);
    if(container->childContainer.size() == 0){
        std::for_each(container->fileds.begin(),container->fileds.end(),[&](Core::Datastruct::Field * field){
            QVariant fieldValue;
            switch(field->type){
                case Core::Datastruct::ComboBox:
                    {
                        QComboBox * widget = dynamic_cast<QComboBox *>(field->widget);
                        if(widget)
                            fieldValue = QVariant::fromValue(widget->currentIndex());
                    }
                    break;
                case Core::Datastruct::CheckBox:
                    {
                        QCheckBox * widget = dynamic_cast<QCheckBox *>(field->widget);
                        if(widget)
                            fieldValue = QVariant::fromValue(widget->isChecked());
                    }
                    break;
                case Core::Datastruct::RadioButton:
                    {
                        QRadioButton * widget = dynamic_cast<QRadioButton *>(field->widget);
                        if(widget)
                            fieldValue = QVariant::fromValue(widget->isChecked());
                    }
                    break;
                case Core::Datastruct::TextEdit:
                    {
                        QLineEdit * widget = dynamic_cast<QLineEdit *>(field->widget);
                        if(widget)
                            fieldValue = QVariant::fromValue(widget->text());
                    }
                    break;
                case Core::Datastruct::ValueIntEdit:
                    {
                        QSpinBox * widget = dynamic_cast<QSpinBox *>(field->widget);
                        if(widget)
                            fieldValue = QVariant::fromValue(widget->value());
                    }
                    break;
                case Core::Datastruct::ValueFloatEdit:
                    {
                        QDoubleSpinBox * widget = dynamic_cast<QDoubleSpinBox *>(field->widget);
                        if(widget)
                            fieldValue = QVariant::fromValue(widget->value());
                    }
                    break;
                case Core::Datastruct::DateEdit:
                    {
                        QDateEdit * widget = dynamic_cast<QDateEdit *>(field->widget);
                        if(widget)
                            fieldValue = QVariant::fromValue(widget->text());
                    }
                    break;
                case Core::Datastruct::TimeEdit:
                    {
                        QTimeEdit * widget = dynamic_cast<QTimeEdit *>(field->widget);
                        if(widget)
                            fieldValue = QVariant::fromValue(widget->text());
                    }
                    break;
                default:
                    break;
            }

            field->data.type = field->type;
            field->data.value = fieldValue;
            d->taskInfo->fields.insert(field->data.index,field->data);
        });
    }else{
        std::for_each(container->childContainer.begin(),container->childContainer.end(),[&](Core::Datastruct::Container * cont){
            findFiled(cont);
        });
    }
}

void TaskDialogProxy::respCancel()
{
    close();
}

}
