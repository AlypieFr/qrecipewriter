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

#include "login.h"
#include "ui_login.h"

extern QString confDir;
extern int configActive;

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::init(bool showPublish) {
    accepted = false;
    if (!showPublish) {
        ui->publier->hide();
        this->adjustSize();
    }
    //Load credentials if available:
    ui->configNb->setText("Config: " + QString::number(configActive));
    QFile idFile (confDir + ".id." + QString::number(configActive));
    if (idFile.exists())
    {
        idFile.open(QFile::ReadOnly);
        QTextStream idStream (&idFile);
        idStream.setCodec("UTF-8"); //Not useful on linux system as it's a default, but Windows has its own defaults....
        ui->user->setText(idStream.readLine());
        ui->password->setText(idStream.readLine());
        idFile.close();
        ui->saveId->setChecked(true);
    }
    else {
        ui->user->setText("");
        ui->password->setText("");
        ui->saveId->setChecked(false);
    }

    //Show dialog
    this->exec();
}

void Login::on_valider_clicked()
{
    accepted = true;
    username = ui->user->text();
    password = ui->password->text();
    publish = ui->publier->isChecked();

    //Save credientials if checked:
    if (ui->saveId->isChecked())
    {
        QFile idFile (confDir + ".id." + QString::number(configActive));
        idFile.open(QFile::WriteOnly);
        QTextStream idStream (&idFile);
        idStream.setCodec("UTF-8"); //Not useful on linux system as it's a default, but Windows has its own defaults....
        idStream << username << "\n" << password << "\n" << endl;
        idFile.close();
    }
    else
    {
        ui->user->setText("");
        ui->password->setText("");
    }

    this->close();
}

void Login::on_annuler_clicked()
{
    accepted = false;
    username = "";
    password = "";
    publish = false;
    this->close();
}

bool Login::getAccepted() {
    return accepted;
}

QString Login::getUsername() {
    return username;
}

QString Login::getPassword() {
    return password;
}

bool Login::getPublish() {
    return publish;
}
