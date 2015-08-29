#include "coupdecoeur.h"
#include "ui_coupdecoeur.h"

CoupDeCoeur::CoupDeCoeur(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CoupDeCoeur)
{
    ui->setupUi(this);
}

CoupDeCoeur::~CoupDeCoeur()
{
    delete ui;
}

void CoupDeCoeur::init(QString value) {
    if (value == "no_coup_de_coeur") {
        ui->niv0->setChecked(true);
    }
    else if (value == "coup_de_coeur_1") {
        ui->niv1->setChecked(true);
    }
    else if (value == "coup_de_coeur_2") {
        ui->niv2->setChecked(true);
    }
    else if (value == "coup_de_coeur_3") {
        ui->niv3->setChecked(true);
    }
    coupDeCoeur = value;
}

void CoupDeCoeur::on_buttonBox_accepted()
{
    if (ui->niv0->isChecked()) {
        coupDeCoeur = "no_coup_de_coeur";
    }
    else if (ui->niv1->isChecked()) {
        coupDeCoeur = "coup_de_coeur_1";
    }
    else if (ui->niv2->isChecked()) {
        coupDeCoeur = "coup_de_coeur_2";
    }
    else if (ui->niv3->isChecked()) {
        coupDeCoeur = "coup_de_coeur_3";
    }
    this->close();
}

void CoupDeCoeur::on_buttonBox_rejected()
{
    this->close();
}
