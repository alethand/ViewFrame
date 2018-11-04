#include "filterdockpanel.h"
#include "ui_filterdockpanel.h"
#include <QDebug>

filterDockPanel::filterDockPanel(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::filterDockPanel)
{
    ui->setupUi(this);
}

filterDockPanel::~filterDockPanel()
{
    delete ui;
}

void filterDockPanel::on_btn_ok_clicked()
{
    filterMessages.clear();
    double lower,upper;
    if(ui->lineEdit_lowerBound->text()==NULL)lower = DBL_MIN ;
    else lower = ui->lineEdit_lowerBound->text().toDouble();

    if(ui->lineEdit_upperBound->text()==NULL) upper = DBL_MAX;
    else upper = ui->lineEdit_upperBound->text().toDouble();

    filterMessages.append(lower);
    filterMessages.append(upper);
    emit sendFilterMessage(filterMessages);
    close();

}

void filterDockPanel::on_btn_cancel_clicked()
{
    close();
}

