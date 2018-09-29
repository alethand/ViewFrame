#include "radiationsourcetablerenovate.h"
#include "ui_radiationsourcetablerenovate.h"

RadiationSourceTableRenovate::RadiationSourceTableRenovate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RadiationSourceTableRenovate)
{
    ui->setupUi(this);
}

RadiationSourceTableRenovate::~RadiationSourceTableRenovate()
{
    delete ui;
}
