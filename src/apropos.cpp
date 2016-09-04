/*
 * © 2013-2016 Flo-Art.fr
 *
 * QRecipeWriter et l'ensemble de ses putils est fournit sous la licence Creative Common BY-NC-SA.
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
