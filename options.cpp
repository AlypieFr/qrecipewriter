/*
 * © 2013-2014 Conseils en Cuisine !
 *
 * CeCWriter et l'ensemble de ses putils est fournit sous la licence Creative Common BY-NC-SA.
 * Toutes les modifications et la redistribution sont autorisés pour une utilisation NON COMMERCIALE.
 * Par ailleurs, les modifications et la reproduction doivent respecter les règles ci-dessous :
 *    - Cette en-tête doit être maintenue.
 *    - Vous devez redistribuer la version modifiée ou non sous licence Creative Common au moins autant
 *      restrictive.
 *    - ConseilsEnCuisine! ne peut être tenu pour responsable des versions modifiées et/ou redistribuées.
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
extern QString pseudoWp; //Pseudo worpdress used for generate "aperçu"
extern QString dirDistPict; //Distant directory where pictures are saved in server
extern QString addrSite; //Address of the website
extern QString addrPub; //Address of publication (XML-RPC)
extern QString systExp; //Operating system of the user
extern QString editPict; //Program for advanced picture editor
extern QString corrOrtho; //Use orthograph correction when typing
extern bool cecPrinter; //CeC Printer status
extern bool cecSearch;
extern bool cecCoupDeCoeur;
extern bool sendAuto;
extern int configActive;

Options::Options(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);

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
    ui->lineEditDossierDistantImages->setText(dirDistPict);
    ui->lineEditDossierSauvegardes->setText(dirSav);
    ui->lineEditDossierTemp->setText(dirTmp);
    ui->lineEditCommandeNavigateur->setText(cmdNav);
    ui->lineEditEditeurImages->setText(editPict);
    ui->lineEditDictionnairePath->setText(corrOrtho);
    ui->lineEditAdresseSite->setText(addrSite);
    ui->lineEditAdressePublication->setText(addrPub);
    ui->lineEditPseudoWordpress->setText(pseudoWp);
    ui->cecPrinter->setChecked(cecPrinter);
    ui->cecSearch->setChecked(cecSearch);
    ui->cecCoupDeCoeur->setChecked(cecCoupDeCoeur);
    ui->sendAuto->setChecked(sendAuto);
    ui->sendManual->setChecked(!sendAuto);
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
    QString fichier = QFileDialog::getExistingDirectory(this,"Choisir un dossier", userDir);
    if(fichier != "")
    {
       ui->lineEditDossierDefautImages->setText(fichier);
    }

}

void Options::on_buttonModifCommandeNavigateur_clicked()
{
    //Chemin qui mène à l'exécutable windows qui lance le navigateur
    QString fichier = QFileDialog::getOpenFileName(this,"Choisir un fichier","/");
    if(fichier != "")
    {
        ui->lineEditCommandeNavigateur->setText(fichier);
    }

}

void Options::on_buttonModifDossierTemp_clicked()
{
    //Selection de dossier
    QString fichier = QFileDialog::getExistingDirectory(this,"Choisir un dossier", "/");
    if(fichier != "")
    {
        ui->lineEditDossierTemp->setText(fichier);
    }

}

void Options::on_buttonModifDossierSauvegardes_clicked()
{
    //Sélection de dossier
    QString fichier = QFileDialog::getExistingDirectory(this,"Choisir un dossier", userDir);
    if(fichier != "")
    {
        ui->lineEditDossierSauvegardes->setText(fichier);
    }

}

void Options::on_buttonModifEditeurImages_clicked()
{
    //Chemin qui mène à l'exécutable windows pour lancer l'éditeur d'images
    QString fichier = QFileDialog::getOpenFileName(this,"Choisir un fichier", "/");
    if(fichier != "")
    {
        ui->lineEditEditeurImages->setText(fichier);
    }
}

void Options::on_buttonOptionsValider_clicked()
{
    if(ui->lineEditCommandeNavigateur->text()==""||ui->lineEditDossierDefautImages->text()==""\
            ||ui->lineEditDossierSauvegardes->text()==""||ui->lineEditDossierTemp->text()==""||ui->lineEditEditeurImages->text()==""\
            ||ui->lineEditPseudoWordpress->text()==""||(!ui->radioButtonLinuxAutre->isChecked()&&!ui->radioButtonLinuxSlack->isChecked()&&!ui->radioButtonLinuxUbuntu->isChecked()\
            &&!ui->radioButtonWindows->isChecked()))
    {
        QMessageBox::critical(this,"Options","Veuillez remplir tous les champs des paramètres client",QMessageBox::Ok);
    }
    else
    {
        if ((ui->configActive->isChecked() && (ui->lineEditAdressePublication->text()==""||ui->lineEditAdresseSite->text()==""||ui->lineEditDossierDistantImages->text()==""))\
            || (ui->configActive_2->isChecked() && (ui->lineEditAdressePublication_2->text()==""||ui->lineEditAdresseSite_2->text()==""||ui->lineEditDossierDistantImages_2->text()==""))\
            || (ui->configActive_3->isChecked() && (ui->lineEditAdressePublication_3->text()==""||ui->lineEditAdresseSite_3->text()==""||ui->lineEditDossierDistantImages_3->text()==""))\
            || (ui->configActive_4->isChecked() && (ui->lineEditAdressePublication_4->text()==""||ui->lineEditAdresseSite_4->text()==""||ui->lineEditDossierDistantImages_4->text()==""))\
            || (ui->configActive_5->isChecked() && (ui->lineEditAdressePublication_5->text()==""||ui->lineEditAdresseSite_5->text()==""||ui->lineEditDossierDistantImages_5->text()==""))) {
            QMessageBox::critical(this,"Options","Veuillez remplir tous les champs des paramètres serveur de la configuration active",QMessageBox::Ok);
        }
        else if (!ui->configActive->isChecked() && !ui->configActive_2->isChecked() && !ui->configActive_3->isChecked() && !ui->configActive_4->isChecked() && !ui->configActive_5->isChecked()) {
            QMessageBox::critical(this,"Options","Vous devez avoir une configuration serveur active",QMessageBox::Ok);
        }
        else {
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
            //Il faut modifier les valeurs des parametres dans le programme
            dirPict=ui->lineEditDossierDefautImages->text();
            dirSav=ui->lineEditDossierSauvegardes->text();
            dirTmp=ui->lineEditDossierTemp->text();
            cmdNav=ui->lineEditCommandeNavigateur->text();
            pseudoWp=ui->lineEditPseudoWordpress->text();
            //addrSite=ui->lineEditAdresseSite->text();
            //addrPub=ui->lineEditAdressePublication->text();
            editPict=ui->lineEditEditeurImages->text();
            //dirDistPict=ui->lineEditDossierDistantImages->text();
            corrOrtho = ui->lineEditDictionnairePath->text();
            //cecPrinter = ui->cecPrinter->isChecked();
            sendAuto = ui->sendAuto->isChecked();
            //cecSearch = ui->cecSearch->isChecked();
            //cecCoupDeCoeur = ui->cecCoupDeCoeur->isChecked();
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
                cecPrinter = ui->cecPrinter->isChecked();
                cecSearch = ui->cecSearch->isChecked();
                cecCoupDeCoeur = ui->cecCoupDeCoeur->isChecked();
                activeServerConfig = 1;
            }
            else if (ui->configActive_2->isChecked()) {
                addrSite=ui->lineEditAdresseSite_2->text();
                addrPub=ui->lineEditAdressePublication_2->text();
                dirDistPict=ui->lineEditDossierDistantImages_2->text();
                cecPrinter = ui->cecPrinter_2->isChecked();
                cecSearch = ui->cecSearch_2->isChecked();
                cecCoupDeCoeur = ui->cecCoupDeCoeur_2->isChecked();
                activeServerConfig = 2;
            }
            else if (ui->configActive_3->isChecked()) {
                addrSite=ui->lineEditAdresseSite_3->text();
                addrPub=ui->lineEditAdressePublication_3->text();
                dirDistPict=ui->lineEditDossierDistantImages_3->text();
                cecPrinter = ui->cecPrinter_3->isChecked();
                cecSearch = ui->cecSearch_3->isChecked();
                cecCoupDeCoeur = ui->cecCoupDeCoeur_3->isChecked();
                activeServerConfig = 3;
            }
            else if (ui->configActive_4->isChecked()) {
                addrSite=ui->lineEditAdresseSite_4->text();
                addrPub=ui->lineEditAdressePublication_4->text();
                dirDistPict=ui->lineEditDossierDistantImages_4->text();
                cecPrinter = ui->cecPrinter_4->isChecked();
                cecSearch = ui->cecSearch_4->isChecked();
                cecCoupDeCoeur = ui->cecCoupDeCoeur_4->isChecked();
                activeServerConfig = 4;
            }
            else if (ui->configActive_5->isChecked()) {
                addrSite=ui->lineEditAdresseSite_5->text();
                addrPub=ui->lineEditAdressePublication_5->text();
                dirDistPict=ui->lineEditDossierDistantImages_5->text();
                cecPrinter = ui->cecPrinter_5->isChecked();
                cecSearch = ui->cecSearch_5->isChecked();
                cecCoupDeCoeur = ui->cecCoupDeCoeur_5->isChecked();
                activeServerConfig = 5;
            }
            //Il n'y a pas de champs sans valeur donc on peut passer à la sauvegarde du fichier
            saveXML(activeServerConfig);
            configActive = activeServerConfig;
            close();
        }
    }
    QDir *dir = new QDir();
    if (!QDir(dirPict).exists())
    {
        dir->mkpath(dirPict);
    }
    if (!QDir(dirSav).exists())
    {
        dir->mkpath(dirSav);
    }
    if (!QDir(dirTmp).exists())
    {
        dir->mkpath(dirTmp);
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
                    if(xml.name() == "cecPrinter") {
                        xml.readNext();
                        configServer[i]["cecPrinter"] = xml.text().toString();
                        continue;
                    }
                    if(xml.name() == "cecSearch") {
                        xml.readNext();
                        configServer[i]["cecSearch"] = xml.text().toString();
                        continue;
                    }
                    if(xml.name() == "cecCoupDeCoeur") {
                        xml.readNext();
                        configServer[i]["cecCoupDeCoeur"] = xml.text().toString();
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
        ui->cecPrinter->setChecked(configServer[1]["cecPrinter"] == "1");
        ui->cecSearch->setChecked(configServer[1]["cecSearch"] == "1");
        ui->cecCoupDeCoeur->setChecked(configServer[1]["cecCoupDeCoeur"] == "1");
    }
    if (configServer.contains(2)) {
        ui->lineEditAdresseSite_2->setText(configServer[2]["addrSite"]);
        ui->lineEditAdressePublication_2->setText(configServer[2]["addrPub"]);
        ui->lineEditDossierDistantImages_2->setText(configServer[2]["dirDistPict"]);
        ui->cecPrinter_2->setChecked(configServer[2]["cecPrinter"] == "1");
        ui->cecSearch_2->setChecked(configServer[2]["cecSearch"] == "1");
        ui->cecCoupDeCoeur_2->setChecked(configServer[2]["cecCoupDeCoeur"] == "1");
    }
    if (configServer.contains(3)) {
        ui->lineEditAdresseSite_3->setText(configServer[3]["addrSite"]);
        ui->lineEditAdressePublication_3->setText(configServer[3]["addrPub"]);
        ui->lineEditDossierDistantImages_3->setText(configServer[3]["dirDistPict"]);
        ui->cecPrinter_3->setChecked(configServer[3]["cecPrinter"] == "1");
        ui->cecSearch_3->setChecked(configServer[3]["cecSearch"] == "1");
        ui->cecCoupDeCoeur_3->setChecked(configServer[3]["cecCoupDeCoeur"] == "1");
    }
    if (configServer.contains(4)) {
        ui->lineEditAdresseSite_4->setText(configServer[4]["addrSite"]);
        ui->lineEditAdressePublication_4->setText(configServer[4]["addrPub"]);
        ui->lineEditDossierDistantImages_4->setText(configServer[4]["dirDistPict"]);
        ui->cecPrinter_4->setChecked(configServer[4]["cecPrinter"] == "1");
        ui->cecSearch_4->setChecked(configServer[4]["cecSearch"] == "1");
        ui->cecCoupDeCoeur_4->setChecked(configServer[4]["cecCoupDeCoeur"] == "1");
    }
    if (configServer.contains(5)) {
        ui->lineEditAdresseSite_5->setText(configServer[5]["addrSite"]);
        ui->lineEditAdressePublication_5->setText(configServer[5]["addrPub"]);
        ui->lineEditDossierDistantImages_5->setText(configServer[5]["dirDistPict"]);
        ui->cecPrinter_5->setChecked(configServer[5]["cecPrinter"] == "1");
        ui->cecSearch_5->setChecked(configServer[5]["cecSearch"] == "1");
        ui->cecCoupDeCoeur_5->setChecked(configServer[5]["cecCoupDeCoeur"] == "1");
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
    writer.writeTextElement("pseudoWp",ui->lineEditPseudoWordpress->text());
    writer.writeTextElement("corrOrtho", ui->lineEditDictionnairePath->text());
    //writer.writeTextElement("addrSite",ui->lineEditAdresseSite->text());
    //writer.writeTextElement("addrPub",ui->lineEditAdressePublication->text());
    writer.writeTextElement("editPict",ui->lineEditEditeurImages->text());
    //writer.writeTextElement("dirDistPict",ui->lineEditDossierDistantImages->text());
    //QString activeCeCPrinter = ui->cecPrinter->isChecked() ? "1": "0";
    //writer.writeTextElement("cecPrinter",activeCeCPrinter);
    QString sendType = ui->sendAuto->isChecked() ? "1" : "0";
    writer.writeTextElement("sendAuto", sendType);
    //QString cSearch = ui->cecSearch->isChecked() ? "1" : "0";
    //writer.writeTextElement("cecSearch", cSearch);
    //QString cCoupDeCoeur = ui->cecCoupDeCoeur->isChecked() ? "1" : "0";
    //writer.writeTextElement("cecCoupDeCoeur", cCoupDeCoeur);

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
    QString activeCeCPrinter = ui->cecPrinter->isChecked() ? "1": "0";
    writerS->writeTextElement("cecPrinter",activeCeCPrinter);
    QString cSearch = ui->cecSearch->isChecked() ? "1" : "0";
    writerS->writeTextElement("cecSearch", cSearch);
    QString cCoupDeCoeur = ui->cecCoupDeCoeur->isChecked() ? "1" : "0";
    writerS->writeTextElement("cecCoupDeCoeur", cCoupDeCoeur);
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
    activeCeCPrinter = ui->cecPrinter_2->isChecked() ? "1": "0";
    writerS->writeTextElement("cecPrinter",activeCeCPrinter);
    cSearch = ui->cecSearch_2->isChecked() ? "1" : "0";
    writerS->writeTextElement("cecSearch", cSearch);
    cCoupDeCoeur = ui->cecCoupDeCoeur_2->isChecked() ? "1" : "0";
    writerS->writeTextElement("cecCoupDeCoeur", cCoupDeCoeur);
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
    activeCeCPrinter = ui->cecPrinter_3->isChecked() ? "1": "0";
    writerS->writeTextElement("cecPrinter",activeCeCPrinter);
    cSearch = ui->cecSearch_3->isChecked() ? "1" : "0";
    writerS->writeTextElement("cecSearch", cSearch);
    cCoupDeCoeur = ui->cecCoupDeCoeur_3->isChecked() ? "1" : "0";
    writerS->writeTextElement("cecCoupDeCoeur", cCoupDeCoeur);
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
    activeCeCPrinter = ui->cecPrinter_4->isChecked() ? "1": "0";
    writerS->writeTextElement("cecPrinter",activeCeCPrinter);
    cSearch = ui->cecSearch_4->isChecked() ? "1" : "0";
    writerS->writeTextElement("cecSearch", cSearch);
    cCoupDeCoeur = ui->cecCoupDeCoeur_4->isChecked() ? "1" : "0";
    writerS->writeTextElement("cecCoupDeCoeur", cCoupDeCoeur);
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
    activeCeCPrinter = ui->cecPrinter_5->isChecked() ? "1": "0";
    writerS->writeTextElement("cecPrinter",activeCeCPrinter);
    cSearch = ui->cecSearch_5->isChecked() ? "1" : "0";
    writerS->writeTextElement("cecSearch", cSearch);
    cCoupDeCoeur = ui->cecCoupDeCoeur_5->isChecked() ? "1" : "0";
    writerS->writeTextElement("cecCoupDeCoeur", cCoupDeCoeur);
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
