#ifndef UNITS_H
#define UNITS_H

#include <QDialog>
#include <QRegExp>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

namespace Ui {
class Units;
}

class Units : public QDialog
{
    Q_OBJECT

public:
    explicit Units(QWidget *parent = 0);
    ~Units();
    void init();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::Units *ui;
    void saveUnits();
};

#endif // UNITS_H
