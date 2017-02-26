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

#include "options.h"
#include "ui_options.h"
#include <QString>
#include <QFileDialog>
#include <fstream>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QApplication>
#include <QDesktopWidget>


using namespace std;

extern QString userDir;
extern QString confDir;
extern QString confFile;


extern QString dirPict; //Default local directory of pictures
extern QString dirTmp; //Temp directory
extern QString cmdNav; //Command to launch navigator
extern QString dirSav; //Directory used to save recipes or "conseils & technics"
extern QString addrSite; //Address of the website
extern QString typeServer;
extern QString systExp; //Operating system of the user
extern QString editPict; //Program for advanced picture editor
extern QString corrOrtho; //Use orthograph correction when typing
extern QString appI18n; //App language
extern bool richSnippets;
extern bool recPrinter; //CeC Printer status
extern bool recCoupDeCoeur;
extern int configActive;
extern QHash<int,QHash<QString, QString>> serverConfs;
extern bool openLastDir_sauvegarde;
extern bool openLastDir_Img;
extern bool checkF7beforeSend;
extern bool autoCheckUpdt;

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    QPushButton* applyButton = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(applyButton, SIGNAL(clicked()), this, SLOT(on_buttonBox_accepted()));
}

Options::~Options()
{
    delete ui;
}

void Options::keyPressEvent(QKeyEvent *e)
{
    //Disable hide dialog by press escape
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
}

void Options::init()
{
    QString idfile  = confDir + ".id";

    ui->lineEditDossierDefautImages->setText(dirPict);
    ui->lineEditDossierDefautImages->setDisabled(openLastDir_Img);
    ui->img_useLastOne->setChecked(openLastDir_Img);
    ui->lineEditDossierSauvegardes->setText(dirSav);
    ui->lineEditDossierSauvegardes->setDisabled(openLastDir_sauvegarde);
    ui->backup_useLastOne->setChecked(openLastDir_sauvegarde);
    ui->lineEditDossierTemp->setText(dirTmp);
    ui->lineEditCommandeNavigateur->setText(cmdNav);
    ui->lineEditEditeurImages->setText(editPict);
    ui->lineEditDictionnairePath->setText(corrOrtho);
    ui->autoSearchUpdt->setChecked(autoCheckUpdt);
    ui->checkF7Send->setChecked(checkF7beforeSend);
    if (appI18n == "fr") {
        ui->radioButton_lang_fr->setChecked(true);
    }
    else {
        ui->radioButton_lang_en->setChecked(true);
    }
    if(systExp=="linuxAutre")
    {
        ui->radioButtonLinuxAutre->setChecked(1);
    }
    else if (systExp=="windows")
    {
        ui->radioButtonWindows->setChecked(1);
    }
    else if(systExp=="linuxSlackware")
    {
        ui->radioButtonLinuxSlack->setChecked(1);
    }
    else if(systExp=="linuxDebian")
    {
        ui->radioButtonLinuxUbuntu->setChecked(1);
    }
    else if (systExp=="linuxArch")
    {
        ui->radioButtonLinuxArch->setChecked(true);
    }
    else if (systExp=="linuxFedora")
    {
        ui->radioButtonLinuxFedora->setChecked(true);
    }
    else if (systExp=="linuxNeon")
    {
        ui->radioButtonLinuxNeon->setChecked(true);
    }

    QFile *idFile = new QFile(idfile + ".1");
    if(idFile->exists())
    {
        ui->buttonOptionsSupprimerDonneesPerso->setEnabled(1);
    }
    else
    {
        ui->buttonOptionsSupprimerDonneesPerso->setEnabled(0);
    }
    idFile = new QFile(idfile + ".2");
    if(idFile->exists())
    {
        ui->buttonOptionsSupprimerDonneesPerso_2->setEnabled(1);
    }
    else
    {
        ui->buttonOptionsSupprimerDonneesPerso_2->setEnabled(0);
    }
    idFile = new QFile(idfile + ".3");
    if(idFile->exists())
    {
        ui->buttonOptionsSupprimerDonneesPerso_3->setEnabled(1);
    }
    else
    {
        ui->buttonOptionsSupprimerDonneesPerso_3->setEnabled(0);
    }
    idFile = new QFile(idfile + ".4");
    if(idFile->exists())
    {
        ui->buttonOptionsSupprimerDonneesPerso_4->setEnabled(1);
    }
    else
    {
        ui->buttonOptionsSupprimerDonneesPerso_4->setEnabled(0);
    }
    idFile = new QFile(idfile + ".5");
    if(idFile->exists())
    {
        ui->buttonOptionsSupprimerDonneesPerso_5->setEnabled(1);
    }
    else
    {
        ui->buttonOptionsSupprimerDonneesPerso_5->setEnabled(0);
    }
    configActiveOpts = configActive;
    switch (configActive) {
    case 1:
        ui->configActive->setChecked(true);
        break;
    case 2:
        ui->configActive_2->setChecked(true);
        break;
    case 3:
        ui->configActive_3->setChecked(true);
        break;
    case 4:
        ui->configActive_4->setChecked(true);
        break;
    case 5:
        ui->configActive_5->setChecked(true);
        break;
    }
    loadConfigsServer();
    ui->tabWidget->setCurrentIndex(0);
    ui->configs->setCurrentIndex(0);
}

void Options::on_buttonModifDefautImages_clicked()
{
    QString fichier = QFileDialog::getExistingDirectory(this,tr("Choisir un dossier"), userDir);
    if(fichier != "")
    {
       ui->lineEditDossierDefautImages->setEnabled(true);
       ui->lineEditDossierDefautImages->setText(fichier);
       ui->img_useLastOne->setChecked(false);
    }

}

void Options::on_buttonModifCommandeNavigateur_clicked()
{
    //Chemin qui mène à l'exécutable windows qui lance le navigateur
    QString fichier = QFileDialog::getOpenFileName(this,tr("Choisir un fichier"),"/");
    if(fichier != "")
    {
        ui->lineEditCommandeNavigateur->setText(fichier);
    }

}

void Options::on_buttonModifDossierTemp_clicked()
{
    //Selection de dossier
    QString fichier = QFileDialog::getExistingDirectory(this,tr("Choisir un dossier"), "/");
    if(fichier != "")
    {
        ui->lineEditDossierTemp->setText(fichier);
    }

}

void Options::on_buttonModifDossierSauvegardes_clicked()
{
    //Sélection de dossier
    QString fichier = QFileDialog::getExistingDirectory(this,tr("Choisir un dossier"), userDir);
    if(fichier != "")
    {
        ui->lineEditDossierSauvegardes->setEnabled(true);
        ui->lineEditDossierSauvegardes->setText(fichier);
        ui->backup_useLastOne->setChecked(false);
    }

}

void Options::on_buttonModifEditeurImages_clicked()
{
    //Chemin qui mène à l'exécutable windows pour lancer l'éditeur d'images
    QString fichier = QFileDialog::getOpenFileName(this,tr("Choisir un fichier"), "/");
    if(fichier != "")
    {
        ui->lineEditEditeurImages->setText(fichier);
    }
}

void Options::on_buttonBox_rejected() {
    this->close();
}

void Options::on_buttonBox_accepted()
{
    if (ui->lineEditDossierDefautImages->text() == "") {
        ui->img_useLastOne->setChecked(true);
        ui->lineEditDossierDefautImages->setEnabled(false);
    }
    if (ui->lineEditDossierSauvegardes->text() == "") {
        ui->backup_useLastOne->setChecked(true);
        ui->lineEditDossierSauvegardes->setEnabled(false);
    }
    if(ui->lineEditCommandeNavigateur->text()==""||ui->lineEditDossierTemp->text()==""\
            ||(!ui->radioButtonLinuxAutre->isChecked()&&!ui->radioButtonLinuxSlack->isChecked()&&!ui->radioButtonLinuxUbuntu->isChecked()\
            &&!ui->radioButtonLinuxArch->isChecked()&&!ui->radioButtonLinuxNeon->isChecked()&&!ui->radioButtonLinuxFedora->isChecked()\
            &&!ui->radioButtonWindows->isChecked()))
    {
        QMessageBox::critical(this,tr("Préférences"),tr("Veuillez remplir tous les champs obligatoires des paramètres client"),QMessageBox::Ok);
    }
    else
    {
        bool proceed = true;
        if (!QDir(ui->lineEditDossierSauvegardes->text()).exists() && !ui->backup_useLastOne->isChecked()) {
            QMessageBox::critical(this, tr("Préférences"), tr("Le dossier de sauvegardes indiqué n'existe pas"), QMessageBox::Ok);
            proceed = false;
        }
        else if (!QDir(ui->lineEditDossierDefautImages->text()).exists() && !ui->img_useLastOne->isChecked()) {
            QMessageBox::critical(this, tr("Préférences"), tr("Le dossier par défaut des images indiqué n'existe pas"), QMessageBox::Ok);
            proceed = false;
        }
        else if(!QDir(ui->lineEditDossierTemp->text()).exists()) {
            QDir().mkpath(ui->lineEditDossierTemp->text());
            if(!QDir(ui->lineEditDossierTemp->text()).exists()) {
                QMessageBox::critical(this, tr("Préférences"), tr("Le dossier temporaire indiqué n'existe pas et ne peut être créé"), QMessageBox::Ok);
                proceed = false;
            }
        }
        else if (ui->lineEditDictionnairePath->text() != "" && (!QFile(ui->lineEditDictionnairePath->text() + ".dic").exists() || !QFile(ui->lineEditDictionnairePath->text() + ".aff").exists())) {
            QMessageBox::critical(this, tr("Préférences"), tr("Le dictionnaire path indiqué est incorrect : les fichiers (avec extension .dic et/ou .aff) n'existe(nt) pas"), QMessageBox::Ok);
            proceed = false;
        }
        if (proceed && ((ui->configActive->isChecked() && (ui->lineEditAdresseSite->text()==""||(!ui->typeServer_wordpress->isChecked() && !ui->typeServer_pywebcooking->isChecked())))\
            || (ui->configActive_2->isChecked() && (ui->lineEditAdresseSite_2->text()==""||(!ui->typeServer_wordpress_2->isChecked() && !ui->typeServer_pywebcooking_2->isChecked())))\
            || (ui->configActive_3->isChecked() && (ui->lineEditAdresseSite_3->text()==""||(!ui->typeServer_wordpress_3->isChecked() && !ui->typeServer_pywebcooking_3->isChecked())))\
            || (ui->configActive_4->isChecked() && (ui->lineEditAdresseSite_4->text()==""||(!ui->typeServer_wordpress_4->isChecked() && !ui->typeServer_pywebcooking_4->isChecked())))\
            || (ui->configActive_5->isChecked() && (ui->lineEditAdresseSite_5->text()==""||(!ui->typeServer_wordpress_5->isChecked() && !ui->typeServer_pywebcooking_5->isChecked()))))) {
            QMessageBox::critical(this,tr("Préférences"),tr("Veuillez remplir tous les champs des paramètres serveur de la configuration active"),QMessageBox::Ok);
        }
        else if (proceed && !ui->configActive->isChecked() && !ui->configActive_2->isChecked() && !ui->configActive_3->isChecked() && !ui->configActive_4->isChecked() && !ui->configActive_5->isChecked()) {
            QMessageBox::critical(this,tr("Préférences"),tr("Vous devez avoir une configuration serveur active"),QMessageBox::Ok);
        }
        else if(proceed) {
            //Test should restart the app:
            QString oldAppI18n = appI18n;
            appI18n=ui->radioButton_lang_fr->isChecked() ? "fr" : "en";
            if (appI18n != oldAppI18n) {
                QMessageBox::information(this, tr("Redémarrage nécessaire"), tr("Vous devez redémarrer l'application pour prendre en compte le changement de langue."), QMessageBox::Ok);
            }
            //Il faut modifier les valeurs des parametres dans le programme
            dirPict=ui->lineEditDossierDefautImages->text();
            dirSav=ui->lineEditDossierSauvegardes->text();
            dirTmp=ui->lineEditDossierTemp->text();
            cmdNav=ui->lineEditCommandeNavigateur->text();
            editPict=ui->lineEditEditeurImages->text();
            corrOrtho = ui->lineEditDictionnairePath->text();
            autoCheckUpdt = ui->autoSearchUpdt->isChecked();
            checkF7beforeSend = ui->checkF7Send->isChecked();
            if(ui->radioButtonLinuxAutre->isChecked())
            {
                systExp="linuxAutre";
            }
            else if(ui->radioButtonLinuxSlack->isChecked())
            {
                systExp="linuxSlackware";
            }
            else if (ui->radioButtonLinuxUbuntu->isChecked())
            {
                systExp="linuxDebian";
            }
            else if (ui->radioButtonLinuxArch->isChecked())
            {
                systExp="linuxArch";
            }
            else if (ui->radioButtonLinuxFedora->isChecked())
            {
                systExp="linuxFedora";
            }
            else if (ui->radioButtonLinuxNeon->isChecked())
            {
                systExp="linuxNeon";
            }
            else if(ui->radioButtonWindows->isChecked())
            {
                systExp="windows";
            }
            int activeServerConfig = 0;
            updateServerConfs();
            if (ui->configActive->isChecked()) {
                addrSite=serverConfs[1]["addrSite"];
                typeServer=serverConfs[1]["typeServer"];
                richSnippets = serverConfs[1]["richSnippets"] == "1";
                recPrinter = serverConfs[1]["recPrinter"] == "1";
                recCoupDeCoeur = serverConfs[1]["recCoupDeCoeur"] == "1";
                activeServerConfig = 1;
            }
            else if (ui->configActive_2->isChecked()) {
                addrSite=serverConfs[2]["addrSite"];
                typeServer=serverConfs[2]["typeServer"];
                richSnippets = serverConfs[2]["richSnippets"] == "1";
                recPrinter = serverConfs[2]["recPrinter"] == "1";
                recCoupDeCoeur = serverConfs[2]["recCoupDeCoeur"] == "1";
                activeServerConfig = 2;
            }
            else if (ui->configActive_3->isChecked()) {
                addrSite=serverConfs[3]["addrSite"];
                typeServer=serverConfs[3]["typeServer"];
                richSnippets = serverConfs[3]["richSnippets"] == "1";
                recPrinter = serverConfs[3]["recPrinter"] == "1";
                recCoupDeCoeur = serverConfs[3]["recCoupDeCoeur"] == "1";
                activeServerConfig = 3;
            }
            else if (ui->configActive_4->isChecked()) {
                addrSite=serverConfs[4]["addrSite"];
                typeServer=serverConfs[4]["typeServer"];
                richSnippets = serverConfs[4]["richSnippets"] == "1";
                recPrinter = serverConfs[4]["recPrinter"] == "1";
                recCoupDeCoeur = serverConfs[4]["recCoupDeCoeur"] == "1";
                activeServerConfig = 4;
            }
            else if (ui->configActive_5->isChecked()) {
                addrSite=serverConfs[5]["addrSite"];
                typeServer=serverConfs[5]["typeServer"];
                richSnippets = serverConfs[5]["richSnippets"] == "1";
                recPrinter = serverConfs[5]["recPrinter"] == "1";
                recCoupDeCoeur = serverConfs[5]["recCoupDeCoeur"] == "1";
                activeServerConfig = 5;
            }
            openLastDir_sauvegarde = ui->backup_useLastOne->isChecked();
            openLastDir_Img = ui->img_useLastOne->isChecked();
            //Il n'y a pas de champs sans valeur donc on peut passer à la sauvegarde du fichier
            saveXML(activeServerConfig);
            configActive = activeServerConfig;
            close();
        }
    }
}

void Options::updateServerConfs() {
    //Server 1
    serverConfs[1]["addrSite"] = ui->lineEditAdresseSite->text();
    serverConfs[1]["typeServer"] = (ui->typeServer_wordpress->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking->isChecked() ? "pywebcooking" : ""));
    serverConfs[1]["richSnippets"] = ui->richSnippets->isChecked() ? "1" : "0";
    serverConfs[1]["recPrinter"] = ui->recPrinter->isChecked() ? "1" : "0";
    serverConfs[1]["recCoupDeCoeur"] = ui->recCoupDeCoeur->isChecked() ? "1" : "0";
    //Server 2
    serverConfs[2]["addrSite"] = ui->lineEditAdresseSite_2->text();
    serverConfs[2]["typeServer"] = (ui->typeServer_wordpress_2->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_2->isChecked() ? "pywebcooking" : ""));
    serverConfs[2]["richSnippets"] = ui->richSnippets_2->isChecked() ? "1" : "0";
    serverConfs[2]["recPrinter"] = ui->recPrinter_2->isChecked() ? "1" : "0";
    serverConfs[2]["recCoupDeCoeur"] = ui->recCoupDeCoeur_2->isChecked() ? "1" : "0";
    //Server 3
    serverConfs[3]["addrSite"] = ui->lineEditAdresseSite_3->text();
    serverConfs[3]["typeServer"] = (ui->typeServer_wordpress_3->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_3->isChecked() ? "pywebcooking" : ""));
    serverConfs[3]["richSnippets"] = ui->richSnippets_3->isChecked() ? "1" : "0";
    serverConfs[3]["recPrinter"] = ui->recPrinter_3->isChecked() ? "1" : "0";
    serverConfs[3]["recCoupDeCoeur"] = ui->recCoupDeCoeur_3->isChecked() ? "1" : "0";
    //Server 4
    serverConfs[4]["addrSite"] = ui->lineEditAdresseSite_4->text();
    serverConfs[4]["typeServer"] = (ui->typeServer_wordpress_4->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_4->isChecked() ? "pywebcooking" : ""));
    serverConfs[4]["richSnippets"] = ui->richSnippets_4->isChecked() ? "1" : "0";
    serverConfs[4]["recPrinter"] = ui->recPrinter_4->isChecked() ? "1" : "0";
    serverConfs[4]["recCoupDeCoeur"] = ui->recCoupDeCoeur_4->isChecked() ? "1" : "0";
    //Server 5
    serverConfs[5]["addrSite"] = ui->lineEditAdresseSite_5->text();
    serverConfs[5]["typeServer"] = (ui->typeServer_wordpress_5->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_5->isChecked() ? "pywebcooking" : ""));
    serverConfs[5]["richSnippets"] = ui->richSnippets_5->isChecked() ? "1" : "0";
    serverConfs[5]["recPrinter"] = ui->recPrinter_5->isChecked() ? "1" : "0";
    serverConfs[5]["recCoupDeCoeur"] = ui->recCoupDeCoeur_5->isChecked() ? "1" : "0";
}

void Options::on_buttonOptionsSupprimerDonneesPerso_clicked()
{
    QString idFile  = confDir;
    idFile.append(".id");
    QFile file(idFile);
    file.remove();
    ui->buttonOptionsSupprimerDonneesPerso->setEnabled(0);
}

void Options::loadConfigsServer() {
    if (serverConfs.contains(1)) {
        if (serverConfs[1]["typeServer"] == "pywebcooking") {
            ui->typeServer_pywebcooking->setChecked(true);
            ui->richSnippets->setVisible(false);
            ui->recPrinter->setVisible(false);
        }
        else {
            ui->typeServer_wordpress->setChecked(true);
            ui->richSnippets->setVisible(true);
            ui->recPrinter->setVisible(true);
            ui->richSnippets->setChecked(serverConfs[1]["richSnippets"] == "1");
            ui->recPrinter->setChecked(serverConfs[1]["recPrinter"] == "1");
        }
        ui->recCoupDeCoeur->setChecked(serverConfs[1]["recCoupDeCoeur"] == "1");
        ui->lineEditAdresseSite->setText(serverConfs[1]["addrSite"]);
    }
    if (serverConfs.contains(2)) {
        if (serverConfs[2]["typeServer"] == "pywebcooking") {
            ui->typeServer_pywebcooking_2->setChecked(true);
            ui->richSnippets_2->setVisible(false);
            ui->recPrinter_2->setVisible(false);
        }
        else {
            ui->typeServer_wordpress_2->setChecked(true);
            ui->richSnippets_2->setVisible(true);
            ui->recPrinter_2->setVisible(true);
            ui->richSnippets_2->setChecked(serverConfs[2]["richSnippets"] == "1");
            ui->recPrinter_2->setChecked(serverConfs[2]["recPrinter"] == "1");
        }
        ui->recCoupDeCoeur_2->setChecked(serverConfs[2]["recCoupDeCoeur"] == "1");
        ui->lineEditAdresseSite_2->setText(serverConfs[2]["addrSite"]);
    }
    if (serverConfs.contains(3)) {
        if (serverConfs[3]["typeServer"] == "pywebcooking") {
            ui->typeServer_pywebcooking_3->setChecked(true);
            ui->richSnippets_3->setVisible(false);
            ui->recPrinter_3->setVisible(false);
        }
        else {
            ui->typeServer_wordpress_3->setChecked(true);
            ui->richSnippets_3->setVisible(true);
            ui->recPrinter_3->setVisible(true);
            ui->richSnippets_3->setChecked(serverConfs[3]["richSnippets"] == "1");
            ui->recPrinter_3->setChecked(serverConfs[3]["recPrinter"] == "1");
        }
        ui->recCoupDeCoeur_3->setChecked(serverConfs[3]["recCoupDeCoeur"] == "1");
        ui->lineEditAdresseSite_3->setText(serverConfs[3]["addrSite"]);
    }
    if (serverConfs.contains(4)) {
        if (serverConfs[4]["typeServer"] == "pywebcooking") {
            ui->typeServer_pywebcooking_4->setChecked(true);
            ui->richSnippets_4->setVisible(false);
            ui->recPrinter_4->setVisible(false);
        }
        else {
            ui->typeServer_wordpress_4->setChecked(true);
            ui->richSnippets_4->setVisible(true);
            ui->recPrinter_4->setVisible(true);
            ui->richSnippets_4->setChecked(serverConfs[4]["richSnippets"] == "1");
            ui->recPrinter_4->setChecked(serverConfs[4]["recPrinter"] == "1");
        }
        ui->recCoupDeCoeur_4->setChecked(serverConfs[4]["recCoupDeCoeur"] == "1");
        ui->lineEditAdresseSite_4->setText(serverConfs[4]["addrSite"]);
    }
    if (serverConfs.contains(5)) {
        if (serverConfs[5]["typeServer"] == "pywebcooking") {
            ui->typeServer_pywebcooking_5->setChecked(true);
            ui->richSnippets_5->setVisible(false);
            ui->recPrinter_5->setVisible(false);
        }
        else {
            ui->typeServer_wordpress_5->setChecked(true);
            ui->richSnippets_5->setVisible(true);
            ui->recPrinter_5->setVisible(true);
            ui->richSnippets_5->setChecked(serverConfs[5]["richSnippets"] == "1");
            ui->recPrinter_5->setChecked(serverConfs[5]["recPrinter"] == "1");
        }
        ui->recCoupDeCoeur_5->setChecked(serverConfs[5]["recCoupDeCoeur"] == "1");
        ui->lineEditAdresseSite_5->setText(serverConfs[5]["addrSite"]);
    }
}

void Options::saveXML(int activeServerConfig)
{

    QFile file(confFile);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0");
    writer.writeStartElement("parameters");
    writer.writeTextElement("dirPict",ui->lineEditDossierDefautImages->text());
    writer.writeTextElement("dirSav",ui->lineEditDossierSauvegardes->text());
    writer.writeTextElement("dirTmp",ui->lineEditDossierTemp->text());
    writer.writeTextElement("cmdNav",ui->lineEditCommandeNavigateur->text());
    writer.writeTextElement("corrOrtho", ui->lineEditDictionnairePath->text());
    writer.writeTextElement("editPict",ui->lineEditEditeurImages->text());
    writer.writeTextElement("appI18n", appI18n);
    writer.writeTextElement("systExp", systExp);
    writer.writeTextElement("activeServerConfig", QString::number(activeServerConfig));
    QString openlast_s = ui->backup_useLastOne->isChecked() ? "1" : "0";
    writer.writeTextElement("openLastDir_Sauvegarde", openlast_s);
    QString openlast_i = ui->img_useLastOne->isChecked() ? "1" : "0";
    writer.writeTextElement("openLastDir_img", openlast_i);
    QString autoUpdt = ui->autoSearchUpdt->isChecked() ? "1" : "0";
    writer.writeTextElement("autoCheckUpdt",autoUpdt);
    QString chkF7 = ui->checkF7Send->isChecked() ? "1" : "0";
    writer.writeTextElement("checkF7beforeSend", chkF7);
    writer.writeEndElement();

    file.close();

    //Server configs:
    ////Config 1:
    QFile *fileS = new QFile (confFile + ".serv1");
    fileS->open(QIODevice::WriteOnly);
    QXmlStreamWriter *writerS = new QXmlStreamWriter(fileS);
    writerS->setAutoFormatting(true);
    writerS->writeStartDocument("1.0");
    writerS->writeStartElement("parameters");
    writerS->writeTextElement("addrSite",ui->lineEditAdresseSite->text());
    writerS->writeTextElement("typeServer", (ui->typeServer_wordpress->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking->isChecked() ? "pywebcooking" : "none")));
    QString activeRichSnippets = ui->richSnippets->isChecked() ? "1" : "0";
    writerS->writeTextElement("richSnippets", activeRichSnippets);
    QString activeCeCPrinter = ui->recPrinter->isChecked() ? "1": "0";
    writerS->writeTextElement("recPrinter",activeCeCPrinter);
    QString cCoupDeCoeur = ui->recCoupDeCoeur->isChecked() ? "1" : "0";
    writerS->writeTextElement("recCoupDeCoeur", cCoupDeCoeur);
    writerS->writeEndElement();
    fileS->close();
    ////Config 2:
    fileS = new QFile (confFile + ".serv2");
    fileS->open(QIODevice::WriteOnly);
    writerS = new QXmlStreamWriter(fileS);
    writerS->setAutoFormatting(true);
    writerS->writeStartDocument("1.0");
    writerS->writeStartElement("parameters");
    writerS->writeTextElement("addrSite",ui->lineEditAdresseSite_2->text());
    writerS->writeTextElement("typeServer", (ui->typeServer_wordpress_2->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_2->isChecked() ? "pywebcooking" : "none")));
    activeRichSnippets = ui->richSnippets_2->isChecked() ? "1" : "0";
    writerS->writeTextElement("richSnippets", activeRichSnippets);
    activeCeCPrinter = ui->recPrinter_2->isChecked() ? "1": "0";
    writerS->writeTextElement("recPrinter",activeCeCPrinter);
    cCoupDeCoeur = ui->recCoupDeCoeur_2->isChecked() ? "1" : "0";
    writerS->writeTextElement("recCoupDeCoeur", cCoupDeCoeur);
    writerS->writeEndElement();
    fileS->close();
    ////Config 3:
    fileS = new QFile (confFile + ".serv3");
    fileS->open(QIODevice::WriteOnly);
    writerS = new QXmlStreamWriter(fileS);
    writerS->setAutoFormatting(true);
    writerS->writeStartDocument("1.0");
    writerS->writeStartElement("parameters");
    writerS->writeTextElement("addrSite",ui->lineEditAdresseSite_3->text());
    writerS->writeTextElement("typeServer", (ui->typeServer_wordpress_3->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_3->isChecked() ? "pywebcooking" : "none")));
    activeRichSnippets = ui->richSnippets_3->isChecked() ? "1" : "0";
    writerS->writeTextElement("richSnippets", activeRichSnippets);
    activeCeCPrinter = ui->recPrinter_3->isChecked() ? "1": "0";
    writerS->writeTextElement("recPrinter",activeCeCPrinter);
    cCoupDeCoeur = ui->recCoupDeCoeur_3->isChecked() ? "1" : "0";
    writerS->writeTextElement("recCoupDeCoeur", cCoupDeCoeur);
    writerS->writeEndElement();
    fileS->close();
    ////Config 4:
    fileS = new QFile (confFile + ".serv4");
    fileS->open(QIODevice::WriteOnly);
    writerS = new QXmlStreamWriter(fileS);
    writerS->setAutoFormatting(true);
    writerS->writeStartDocument("1.0");
    writerS->writeStartElement("parameters");
    writerS->writeTextElement("addrSite",ui->lineEditAdresseSite_4->text());
    writerS->writeTextElement("typeServer", (ui->typeServer_wordpress_4->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_4->isChecked() ? "pywebcooking" : "none")));
    activeRichSnippets = ui->richSnippets_4->isChecked() ? "1" : "0";
    writerS->writeTextElement("richSnippets", activeRichSnippets);
    activeCeCPrinter = ui->recPrinter_4->isChecked() ? "1": "0";
    writerS->writeTextElement("recPrinter",activeCeCPrinter);
    cCoupDeCoeur = ui->recCoupDeCoeur_4->isChecked() ? "1" : "0";
    writerS->writeTextElement("recCoupDeCoeur", cCoupDeCoeur);
    writerS->writeEndElement();
    fileS->close();
    ////Config 5:
    fileS = new QFile (confFile + ".serv5");
    fileS->open(QIODevice::WriteOnly);
    writerS = new QXmlStreamWriter(fileS);
    writerS->setAutoFormatting(true);
    writerS->writeStartDocument("1.0");
    writerS->writeStartElement("parameters");
    writerS->writeTextElement("addrSite",ui->lineEditAdresseSite_5->text());
    writerS->writeTextElement("typeServer", (ui->typeServer_wordpress_5->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_5->isChecked() ? "pywebcooking" : "none")));
    activeRichSnippets = ui->richSnippets_5->isChecked() ? "1" : "0";
    writerS->writeTextElement("richSnippets", activeRichSnippets);
    activeCeCPrinter = ui->recPrinter_5->isChecked() ? "1": "0";
    writerS->writeTextElement("recPrinter",activeCeCPrinter);
    cCoupDeCoeur = ui->recCoupDeCoeur_5->isChecked() ? "1" : "0";
    writerS->writeTextElement("recCoupDeCoeur", cCoupDeCoeur);
    writerS->writeEndElement();
    fileS->close();
}

void Options::on_configActive_released()
{
    if (ui->configActive->isChecked()) {
        ui->configActive_2->setChecked(false);
        ui->configActive_3->setChecked(false);
        ui->configActive_4->setChecked(false);
        ui->configActive_5->setChecked(false);
        configActiveOpts = 1;
    }
}

void Options::on_configActive_2_released()
{
    if (ui->configActive_2->isChecked()) {
        ui->configActive->setChecked(false);
        ui->configActive_3->setChecked(false);
        ui->configActive_4->setChecked(false);
        ui->configActive_5->setChecked(false);
        configActiveOpts = 2;
    }
}

void Options::on_configActive_3_released()
{
    if (ui->configActive_3->isChecked()) {
        ui->configActive_2->setChecked(false);
        ui->configActive->setChecked(false);
        ui->configActive_4->setChecked(false);
        ui->configActive_5->setChecked(false);
        configActiveOpts = 3;
    }
}

void Options::on_configActive_4_released()
{
    if (ui->configActive_4->isChecked()) {
        ui->configActive_2->setChecked(false);
        ui->configActive_3->setChecked(false);
        ui->configActive->setChecked(false);
        ui->configActive_5->setChecked(false);
        configActiveOpts = 4;
    }
}

void Options::on_configActive_5_released()
{
    if (ui->configActive_5->isChecked()) {
        ui->configActive_2->setChecked(false);
        ui->configActive_3->setChecked(false);
        ui->configActive_4->setChecked(false);
        ui->configActive->setChecked(false);
        configActiveOpts = 5;
    }
}

void Options::on_backup_useLastOne_clicked()
{
    if (ui->backup_useLastOne->isChecked()) {
        ui->lineEditDossierSauvegardes->setEnabled(false);
    }
    else {
        ui->lineEditDossierSauvegardes->setEnabled(true);
    }
}

void Options::on_img_useLastOne_clicked()
{
    if (ui->img_useLastOne->isChecked()) {
        ui->lineEditDossierDefautImages->setEnabled(false);
    }
    else {
        ui->lineEditDossierDefautImages->setEnabled(true);
    }
}

void Options::on_typeServer_pywebcooking_toggled(bool checked)
{
    if (checked) {
        ui->richSnippets->setVisible(false);
        ui->recPrinter->setVisible(false);
    }
    else {
        ui->richSnippets->setVisible(true);
        ui->recPrinter->setVisible(true);
    }
}

void Options::on_typeServer_pywebcooking_2_toggled(bool checked)
{
    if (checked) {
        ui->richSnippets_2->setVisible(false);
        ui->recPrinter_2->setVisible(false);
    }
    else {
        ui->richSnippets_2->setVisible(true);
        ui->recPrinter_2->setVisible(true);
    }
}

void Options::on_typeServer_pywebcooking_3_toggled(bool checked)
{
    if (checked) {
        ui->richSnippets_3->setVisible(false);
        ui->recPrinter_3->setVisible(false);
    }
    else {
        ui->richSnippets_3->setVisible(true);
        ui->recPrinter_3->setVisible(true);
    }
}

void Options::on_typeServer_pywebcooking_4_toggled(bool checked)
{
    if (checked) {
        ui->richSnippets_4->setVisible(false);
        ui->recPrinter_4->setVisible(false);
    }
    else {
        ui->richSnippets_4->setVisible(true);
        ui->recPrinter_4->setVisible(true);
    }
}

void Options::on_typeServer_pywebcooking_5_toggled(bool checked)
{
    if (checked) {
        ui->richSnippets_5->setVisible(false);
        ui->recPrinter_5->setVisible(false);
    }
    else {
        ui->richSnippets_5->setVisible(true);
        ui->recPrinter_5->setVisible(true);
    }
}
