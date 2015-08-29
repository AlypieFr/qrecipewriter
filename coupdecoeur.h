#ifndef COUPDECOEUR_H
#define COUPDECOEUR_H

#include <QDialog>

namespace Ui {
class CoupDeCoeur;
}

class CoupDeCoeur : public QDialog
{
    Q_OBJECT

public:
    explicit CoupDeCoeur(QWidget *parent = 0);
    ~CoupDeCoeur();
    void init(QString value);
    QString coupDeCoeur;

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CoupDeCoeur *ui;
};

#endif // COUPDECOEUR_H
