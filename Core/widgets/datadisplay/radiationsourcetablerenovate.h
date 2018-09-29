#ifndef RADIATIONSOURCETABLERENOVATE_H
#define RADIATIONSOURCETABLERENOVATE_H

#include <QWidget>

namespace Ui {
class RadiationSourceTableRenovate;
}

class RadiationSourceTableRenovate : public QWidget
{
    Q_OBJECT

public:
    explicit RadiationSourceTableRenovate(QWidget *parent = 0);
    ~RadiationSourceTableRenovate();

private:
    Ui::RadiationSourceTableRenovate *ui;
};

#endif // RADIATIONSOURCETABLERENOVATE_H
