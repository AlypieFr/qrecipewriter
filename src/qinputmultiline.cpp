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

#include "qinputmultiline.h"
#include "ui_qinputmultiline.h"

QInputMultiline::QInputMultiline(QWidget *parent, QString title, QString message, QString text, bool wordWrap, int width, int height) :
    QDialog(parent),
    ui(new Ui::QInputMultiline)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->message->setText(message + " :");
    if (!wordWrap)
        ui->text->setWordWrapMode(QTextOption::NoWrap);
    ui->text->setPlainText(text);
    this->text = "";
    this->setBaseSize(width, height);
}

QInputMultiline::~QInputMultiline()
{
    delete ui;
}

void QInputMultiline::on_buttonBox_accepted()
{
    this->text = ui->text->toPlainText();
    this->close();
}

void QInputMultiline::on_buttonBox_rejected()
{
    this->close();
}
