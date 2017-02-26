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

#include "apropos.h"
#include "ui_apropos.h"
#include <QPixmap>

extern QString VERSION;

Apropos::Apropos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Apropos)
{
    ui->setupUi(this);
}

Apropos::~Apropos()
{
    delete ui;
}

void Apropos::init() {
    QPixmap logo(":/images/apropos2.png");
    ui->label_3->setPixmap(logo);
    ui->versionApp->setTextFormat(Qt::RichText);
    ui->versionApp->setText("<span style=' font-size:12pt;'>" + tr("VERSION ACTUELLE :") + " "+ VERSION +"</span>");
}
