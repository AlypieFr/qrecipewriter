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

#include "login.h"
#include "ui_login.h"

extern QString confDir;
extern int configActive;
extern QHash<int,QHash<QString, QString> > serverConfs;

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

    ui->config_nb->clear();
    QStringList validConfs;
    QList<int> idConfs = serverConfs.keys();
    qSort(idConfs);
    foreach (int idConf, idConfs) {
        if (Functions::is_config_valid(idConf)) {
            validConfs.append("Config " + QString::number(idConf));
        }
    }
    ui->config_nb->addItems(validConfs);
    ui->config_nb->setCurrentIndex(validConfs.indexOf("Config " + QString::number(configActive)));

    //Show dialog
    this->exec();
}

void Login::on_valider_clicked()
{
    accepted = true;
    username = ui->user->text();
    password = ui->password->text();
    publish = ui->publier->isChecked();
    config = ui->config_nb->currentIndex() + 1;

    //Save credientials if checked:
    if (ui->saveId->isChecked())
    {
        QFile idFile (confDir + ".id." + QString::number(config));
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

int Login::getConfig() {
    return config;
}

bool Login::getPublish() {
    return publish;
}

void Login::on_config_nb_currentIndexChanged(int index)
{
    int idConfig = index + 1;
    QFile idFile (confDir + ".id." + QString::number(idConfig));
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
}
