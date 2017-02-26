/*
 * © Copyright 2013-2017 Flo-Art.fr
 *
 * QRecipeWriter is available under the Creative Common BY-NC-SA licence.
 * All modifications and redistributions on the code are allowed for a NON COMMERCIAL usage.
 * Also, modifications and reproductions of the code must agree these conditions:
 *    - This copyright header must NOT be deleted.
 *    - You MUST redistribute the modified or not version under the Creative Common licence at least as restrictive as ours.
 *    - Flo-Art.fr can't be responsible for any version modified or redistributed by third parties.
 *    - Any commercial usage of this code, full or partial, is forbidden.
 *
 * QRecipeWriter est fournit sous la licence Creative Common BY-NC-SA.
 * Toutes les modifications et la redistribution sont autorisés pour une utilisation NON COMMERCIALE.
 * Par ailleurs, les modifications et la reproduction doivent respecter les règles ci-dessous :
 *    - Cette en-tête doit être maintenue.
 *    - Vous devez redistribuer la version modifiée ou non sous licence Creative Common au moins autant
 *      restrictive.
 *    - Flo-Art.fr ne peut être tenu pour responsable des versions modifiées et/ou redistribuées.
 *    - Toute utilisation commerciale partielle ou complète est interdite.
 */

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
