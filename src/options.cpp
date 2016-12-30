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
extern QString dirDistPict; //Distant directory where pictures are saved in server
extern QString addrSite; //Address of the website
extern QString addrPub; //Address of publication (XML-RPC)
extern QString typeServer;
extern QString systExp; //Operating system of the user
extern QString editPict; //Program for advanced picture editor
extern QString corrOrtho; //Use orthograph correction when typing
extern QString appI18n; //App language
extern bool richSnippets;
extern bool recPrinter; //CeC Printer status
extern bool recSearch;
extern bool recCoupDeCoeur;
extern int configActive;
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
    ui->lineEditDossierDistantImages->setText(dirDistPict);
    ui->lineEditDossierSauvegardes->setText(dirSav);
    ui->lineEditDossierSauvegardes->setDisabled(openLastDir_sauvegarde);
    ui->backup_useLastOne->setChecked(openLastDir_sauvegarde);
    ui->lineEditDossierTemp->setText(dirTmp);
    ui->lineEditCommandeNavigateur->setText(cmdNav);
    ui->lineEditEditeurImages->setText(editPict);
    ui->lineEditDictionnairePath->setText(corrOrtho);
    ui->lineEditAdresseSite->setText(addrSite);
    ui->lineEditAdressePublication->setText(addrPub);
    ui->recPrinter->setChecked(recPrinter);
    ui->recSearch->setChecked(recSearch);
    ui->recCoupDeCoeur->setChecked(recCoupDeCoeur);
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
            &&!ui->radioButtonLinuxArch->isChecked()&&!ui->radioButtonWindows->isChecked()))
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
        if (proceed && ((ui->configActive->isChecked() && (ui->lineEditAdressePublication->text()==""||ui->lineEditAdresseSite->text()==""||ui->lineEditDossierDistantImages->text()==""||(!ui->typeServer_wordpress->isChecked() && !ui->typeServer_pywebcooking->isChecked())))\
            || (ui->configActive_2->isChecked() && (ui->lineEditAdressePublication_2->text()==""||ui->lineEditAdresseSite_2->text()==""||ui->lineEditDossierDistantImages_2->text()==""||(!ui->typeServer_wordpress_2->isChecked() && !ui->typeServer_pywebcooking_2->isChecked())))\
            || (ui->configActive_3->isChecked() && (ui->lineEditAdressePublication_3->text()==""||ui->lineEditAdresseSite_3->text()==""||ui->lineEditDossierDistantImages_3->text()==""||(!ui->typeServer_wordpress_3->isChecked() && !ui->typeServer_pywebcooking_3->isChecked())))\
            || (ui->configActive_4->isChecked() && (ui->lineEditAdressePublication_4->text()==""||ui->lineEditAdresseSite_4->text()==""||ui->lineEditDossierDistantImages_4->text()==""||(!ui->typeServer_wordpress_4->isChecked() && !ui->typeServer_pywebcooking_4->isChecked())))\
            || (ui->configActive_5->isChecked() && (ui->lineEditAdressePublication_5->text()==""||ui->lineEditAdresseSite_5->text()==""||ui->lineEditDossierDistantImages_5->text()==""||(!ui->typeServer_wordpress_5->isChecked() && !ui->typeServer_pywebcooking_5->isChecked()))))) {
            QMessageBox::critical(this,tr("Préférences"),tr("Veuillez remplir tous les champs des paramètres serveur de la configuration active"),QMessageBox::Ok);
        }
        else if (proceed && !ui->configActive->isChecked() && !ui->configActive_2->isChecked() && !ui->configActive_3->isChecked() && !ui->configActive_4->isChecked() && !ui->configActive_5->isChecked()) {
            QMessageBox::critical(this,tr("Préférences"),tr("Vous devez avoir une configuration serveur active"),QMessageBox::Ok);
        }
        else if(proceed) {
            if (ui->lineEditDossierDistantImages->text() != "" && ui->lineEditDossierDistantImages->text().right(1) != "/")
            {
                ui->lineEditDossierDistantImages->setText(ui->lineEditDossierDistantImages->text().append("/"));
            }
            if (ui->lineEditDossierDistantImages_2->text() != "" && ui->lineEditDossierDistantImages_2->text().right(1) != "/")
            {
                ui->lineEditDossierDistantImages_2->setText(ui->lineEditDossierDistantImages_2->text().append("/"));
            }
            if (ui->lineEditDossierDistantImages_3->text() != "" && ui->lineEditDossierDistantImages_3->text().right(1) != "/")
            {
                ui->lineEditDossierDistantImages_3->setText(ui->lineEditDossierDistantImages_3->text().append("/"));
            }
            if (ui->lineEditDossierDistantImages_4->text() != "" && ui->lineEditDossierDistantImages_4->text().right(1) != "/")
            {
                ui->lineEditDossierDistantImages_4->setText(ui->lineEditDossierDistantImages_4->text().append("/"));
            }
            if (ui->lineEditDossierDistantImages_5->text() != "" && ui->lineEditDossierDistantImages_5->text().right(1) != "/")
            {
                ui->lineEditDossierDistantImages_5->setText(ui->lineEditDossierDistantImages_5->text().append("/"));
            }
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
            else if(ui->radioButtonWindows->isChecked())
            {
                systExp="windows";
            }
            int activeServerConfig = 0;
            if (ui->configActive->isChecked()) {
                addrSite=ui->lineEditAdresseSite->text();
                addrPub=ui->lineEditAdressePublication->text();
                dirDistPict=ui->lineEditDossierDistantImages->text();
                typeServer=(ui->typeServer_wordpress->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking->isChecked() ? "pywebcooking" : ""));
                richSnippets = ui->richSnippets->isChecked();
                recPrinter = ui->recPrinter->isChecked();
                recSearch = ui->recSearch->isChecked();
                recCoupDeCoeur = ui->recCoupDeCoeur->isChecked();
                activeServerConfig = 1;
            }
            else if (ui->configActive_2->isChecked()) {
                addrSite=ui->lineEditAdresseSite_2->text();
                addrPub=ui->lineEditAdressePublication_2->text();
                dirDistPict=ui->lineEditDossierDistantImages_2->text();
                typeServer=(ui->typeServer_wordpress_2->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_2->isChecked() ? "pywebcooking" : ""));
                richSnippets = ui->richSnippets_2->isChecked();
                recPrinter = ui->recPrinter_2->isChecked();
                recSearch = ui->recSearch_2->isChecked();
                recCoupDeCoeur = ui->recCoupDeCoeur_2->isChecked();
                activeServerConfig = 2;
            }
            else if (ui->configActive_3->isChecked()) {
                addrSite=ui->lineEditAdresseSite_3->text();
                addrPub=ui->lineEditAdressePublication_3->text();
                dirDistPict=ui->lineEditDossierDistantImages_3->text();
                typeServer=(ui->typeServer_wordpress_3->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_3->isChecked() ? "pywebcooking" : ""));
                richSnippets = ui->richSnippets_3->isChecked();
                recPrinter = ui->recPrinter_3->isChecked();
                recSearch = ui->recSearch_3->isChecked();
                recCoupDeCoeur = ui->recCoupDeCoeur_3->isChecked();
                activeServerConfig = 3;
            }
            else if (ui->configActive_4->isChecked()) {
                addrSite=ui->lineEditAdresseSite_4->text();
                addrPub=ui->lineEditAdressePublication_4->text();
                dirDistPict=ui->lineEditDossierDistantImages_4->text();
                typeServer=(ui->typeServer_wordpress_4->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_4->isChecked() ? "pywebcooking" : ""));
                richSnippets = ui->richSnippets_4->isChecked();
                recPrinter = ui->recPrinter_4->isChecked();
                recSearch = ui->recSearch_4->isChecked();
                recCoupDeCoeur = ui->recCoupDeCoeur_4->isChecked();
                activeServerConfig = 4;
            }
            else if (ui->configActive_5->isChecked()) {
                addrSite=ui->lineEditAdresseSite_5->text();
                addrPub=ui->lineEditAdressePublication_5->text();
                dirDistPict=ui->lineEditDossierDistantImages_5->text();
                typeServer=(ui->typeServer_wordpress_5->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_5->isChecked() ? "pywebcooking" : ""));
                richSnippets = ui->richSnippets_5->isChecked();
                recPrinter = ui->recPrinter_5->isChecked();
                recSearch = ui->recSearch_5->isChecked();
                recCoupDeCoeur = ui->recCoupDeCoeur_5->isChecked();
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

void Options::on_buttonOptionsSupprimerDonneesPerso_clicked()
{
    QString idFile  = confDir;
    idFile.append(".id");
    QFile file(idFile);
    file.remove();
    ui->buttonOptionsSupprimerDonneesPerso->setEnabled(0);
}

void Options::loadConfigsServer() {
    QHash<int, QHash<QString, QString> > configServer;
    for (int i = 1; i <= 5; i++) {
        QFile *file = new QFile(confFile + ".serv" + QString::number(i));
        if (file->exists()) {
            file->open(QIODevice::ReadOnly);
            QXmlStreamReader xml(file);
            while(!xml.atEnd() &&
                    !xml.hasError()) {

                QXmlStreamReader::TokenType token = xml.readNext();
                if(token == QXmlStreamReader::StartDocument) {
                    continue;
                }
                if(token == QXmlStreamReader::StartElement) {
                    if(xml.name() == "addrSite") {
                        xml.readNext();
                        configServer[i]["addrSite"] = xml.text().toString();
                        continue;
                    }
                    if(xml.name() == "addrPub") {
                        xml.readNext();
                        configServer[i]["addrPub"] = xml.text().toString();
                        continue;
                    }
                    if(xml.name() == "dirDistPict") {
                        xml.readNext();
                        configServer[i]["dirDistPict"] = xml.text().toString();
                        continue;
                    }
                    if(xml.name() == "typeServer") {
                        xml.readNext();
                        configServer[i]["typeServer"] = xml.text().toString();
                        continue;
                    }
                    if(xml.name() == "richSnippets") {
                        xml.readNext();
                        configServer[i]["richSnippets"] = xml.text().toString();
                        continue;
                    }
                    if(xml.name() == "recPrinter") {
                        xml.readNext();
                        configServer[i]["recPrinter"] = xml.text().toString();
                        continue;
                    }
                    if(xml.name() == "recSearch") {
                        xml.readNext();
                        configServer[i]["recSearch"] = xml.text().toString();
                        continue;
                    }
                    if(xml.name() == "recCoupDeCoeur") {
                        xml.readNext();
                        configServer[i]["recCoupDeCoeur"] = xml.text().toString();
                        continue;
                    }
                }
            }
        }
    }
    if (configServer.contains(1)) {
        ui->lineEditAdresseSite->setText(configServer[1]["addrSite"]);
        ui->lineEditAdressePublication->setText(configServer[1]["addrPub"]);
        ui->lineEditDossierDistantImages->setText(configServer[1]["dirDistPict"]);
        ui->typeServer_wordpress->setChecked(configServer[1]["typeServer"] == "wordpress");
        ui->typeServer_pywebcooking->setChecked(configServer[1]["typeServer"] == "pywebcooking");
        ui->richSnippets->setChecked(configServer[1]["richSnippets"] == "1");
        ui->recPrinter->setChecked(configServer[1]["recPrinter"] == "1");
        ui->recSearch->setChecked(configServer[1]["recSearch"] == "1");
        ui->recCoupDeCoeur->setChecked(configServer[1]["recCoupDeCoeur"] == "1");
    }
    if (configServer.contains(2)) {
        ui->lineEditAdresseSite_2->setText(configServer[2]["addrSite"]);
        ui->lineEditAdressePublication_2->setText(configServer[2]["addrPub"]);
        ui->lineEditDossierDistantImages_2->setText(configServer[2]["dirDistPict"]);
        ui->typeServer_wordpress_2->setChecked(configServer[2]["typeServer"] == "wordpress");
        ui->typeServer_pywebcooking_2->setChecked(configServer[2]["typeServer"] == "pywebcooking");
        ui->richSnippets_2->setChecked(configServer[2]["richSnippets"] == "1");
        ui->recPrinter_2->setChecked(configServer[2]["recPrinter"] == "1");
        ui->recSearch_2->setChecked(configServer[2]["recSearch"] == "1");
        ui->recCoupDeCoeur_2->setChecked(configServer[2]["recCoupDeCoeur"] == "1");
    }
    if (configServer.contains(3)) {
        ui->lineEditAdresseSite_3->setText(configServer[3]["addrSite"]);
        ui->lineEditAdressePublication_3->setText(configServer[3]["addrPub"]);
        ui->lineEditDossierDistantImages_3->setText(configServer[3]["dirDistPict"]);
        ui->typeServer_wordpress_3->setChecked(configServer[3]["typeServer"] == "wordpress");
        ui->typeServer_pywebcooking_3->setChecked(configServer[3]["typeServer"] == "pywebcooking");
        ui->richSnippets_3->setChecked(configServer[3]["richSnippets"] == "1");
        ui->recPrinter_3->setChecked(configServer[3]["recPrinter"] == "1");
        ui->recSearch_3->setChecked(configServer[3]["recSearch"] == "1");
        ui->recCoupDeCoeur_3->setChecked(configServer[3]["recCoupDeCoeur"] == "1");
    }
    if (configServer.contains(4)) {
        ui->lineEditAdresseSite_4->setText(configServer[4]["addrSite"]);
        ui->lineEditAdressePublication_4->setText(configServer[4]["addrPub"]);
        ui->lineEditDossierDistantImages_4->setText(configServer[4]["dirDistPict"]);
        ui->typeServer_wordpress_4->setChecked(configServer[4]["typeServer"] == "wordpress");
        ui->typeServer_pywebcooking_4->setChecked(configServer[4]["typeServer"] == "pywebcooking");
        ui->richSnippets_4->setChecked(configServer[4]["richSnippets"] == "1");
        ui->recPrinter_4->setChecked(configServer[4]["recPrinter"] == "1");
        ui->recSearch_4->setChecked(configServer[4]["recSearch"] == "1");
        ui->recCoupDeCoeur_4->setChecked(configServer[4]["recCoupDeCoeur"] == "1");
    }
    if (configServer.contains(5)) {
        ui->lineEditAdresseSite_5->setText(configServer[5]["addrSite"]);
        ui->lineEditAdressePublication_5->setText(configServer[5]["addrPub"]);
        ui->lineEditDossierDistantImages_5->setText(configServer[5]["dirDistPict"]);
        ui->typeServer_wordpress_5->setChecked(configServer[5]["typeServer"] == "wordpress");
        ui->typeServer_pywebcooking_5->setChecked(configServer[5]["typeServer"] == "pywebcooking");
        ui->richSnippets_5->setChecked(configServer[5]["richSnippets"] == "1");
        ui->recPrinter_5->setChecked(configServer[5]["recPrinter"] == "1");
        ui->recSearch_5->setChecked(configServer[5]["recSearch"] == "1");
        ui->recCoupDeCoeur_5->setChecked(configServer[5]["recCoupDeCoeur"] == "1");
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
    if(ui->radioButtonLinuxAutre->isChecked())
    {
        writer.writeTextElement("systExp","linuxAutre");
    }
    else if(ui->radioButtonLinuxSlack->isChecked())
    {
        writer.writeTextElement("systExp","linuxSlackware");
    }
    else if(ui->radioButtonLinuxUbuntu->isChecked())
    {
        writer.writeTextElement("systExp","linuxDebian");
    }
    else if(ui->radioButtonWindows->isChecked())
    {
        writer.writeTextElement("systExp","windows");
    }
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
    writerS->writeTextElement("addrPub",ui->lineEditAdressePublication->text());
    writerS->writeTextElement("dirDistPict",ui->lineEditDossierDistantImages->text());
    writerS->writeTextElement("typeServer", (ui->typeServer_wordpress->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking->isChecked() ? "pywebcooking" : "none")));
    QString activeRichSnippets = ui->richSnippets->isChecked() ? "1" : "0";
    writerS->writeTextElement("richSnippets", activeRichSnippets);
    QString activeCeCPrinter = ui->recPrinter->isChecked() ? "1": "0";
    writerS->writeTextElement("recPrinter",activeCeCPrinter);
    QString cSearch = ui->recSearch->isChecked() ? "1" : "0";
    writerS->writeTextElement("recSearch", cSearch);
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
    writerS->writeTextElement("addrPub",ui->lineEditAdressePublication_2->text());
    writerS->writeTextElement("dirDistPict",ui->lineEditDossierDistantImages_2->text());
    writerS->writeTextElement("typeServer", (ui->typeServer_wordpress_2->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_2->isChecked() ? "pywebcooking" : "none")));
    activeRichSnippets = ui->richSnippets_2->isChecked() ? "1" : "0";
    writerS->writeTextElement("richSnippets", activeRichSnippets);
    activeCeCPrinter = ui->recPrinter_2->isChecked() ? "1": "0";
    writerS->writeTextElement("recPrinter",activeCeCPrinter);
    cSearch = ui->recSearch_2->isChecked() ? "1" : "0";
    writerS->writeTextElement("recSearch", cSearch);
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
    writerS->writeTextElement("addrPub",ui->lineEditAdressePublication_3->text());
    writerS->writeTextElement("dirDistPict",ui->lineEditDossierDistantImages_3->text());
    writerS->writeTextElement("typeServer", (ui->typeServer_wordpress_3->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_3->isChecked() ? "pywebcooking" : "none")));
    activeRichSnippets = ui->richSnippets_3->isChecked() ? "1" : "0";
    writerS->writeTextElement("richSnippets", activeRichSnippets);
    activeCeCPrinter = ui->recPrinter_3->isChecked() ? "1": "0";
    writerS->writeTextElement("recPrinter",activeCeCPrinter);
    cSearch = ui->recSearch_3->isChecked() ? "1" : "0";
    writerS->writeTextElement("recSearch", cSearch);
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
    writerS->writeTextElement("addrPub",ui->lineEditAdressePublication_4->text());
    writerS->writeTextElement("dirDistPict",ui->lineEditDossierDistantImages_4->text());
    writerS->writeTextElement("typeServer", (ui->typeServer_wordpress_4->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_4->isChecked() ? "pywebcooking" : "none")));
    activeRichSnippets = ui->richSnippets_4->isChecked() ? "1" : "0";
    writerS->writeTextElement("richSnippets", activeRichSnippets);
    activeCeCPrinter = ui->recPrinter_4->isChecked() ? "1": "0";
    writerS->writeTextElement("recPrinter",activeCeCPrinter);
    cSearch = ui->recSearch_4->isChecked() ? "1" : "0";
    writerS->writeTextElement("recSearch", cSearch);
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
    writerS->writeTextElement("addrPub",ui->lineEditAdressePublication_5->text());
    writerS->writeTextElement("dirDistPict",ui->lineEditDossierDistantImages_5->text());
    writerS->writeTextElement("typeServer", (ui->typeServer_wordpress_5->isChecked() ? "wordpress" : (ui->typeServer_pywebcooking_5->isChecked() ? "pywebcooking" : "none")));
    activeRichSnippets = ui->richSnippets_5->isChecked() ? "1" : "0";
    writerS->writeTextElement("richSnippets", activeRichSnippets);
    activeCeCPrinter = ui->recPrinter_5->isChecked() ? "1": "0";
    writerS->writeTextElement("recPrinter",activeCeCPrinter);
    cSearch = ui->recSearch_5->isChecked() ? "1" : "0";
    writerS->writeTextElement("recSearch", cSearch);
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
