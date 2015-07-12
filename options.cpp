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
extern bool sendAuto;

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
    QFile idFile(idfile);

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

    if(idFile.exists())
    {
        ui->buttonOptionsSupprimerDonneesPerso->setEnabled(1);
    }
    else
    {
        ui->buttonOptionsSupprimerDonneesPerso->setEnabled(0);
    }
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
    if(ui->lineEditAdressePublication->text()==""||ui->lineEditAdresseSite->text()==""||ui->lineEditCommandeNavigateur->text()==""||ui->lineEditDossierDefautImages->text()==""\
            ||ui->lineEditDossierDistantImages->text()==""||ui->lineEditDossierSauvegardes->text()==""||ui->lineEditDossierTemp->text()==""||ui->lineEditEditeurImages->text()==""\
            ||ui->lineEditPseudoWordpress->text()==""||(!ui->radioButtonLinuxAutre->isChecked()&&!ui->radioButtonLinuxSlack->isChecked()&&!ui->radioButtonLinuxUbuntu->isChecked()\
            &&!ui->radioButtonWindows->isChecked()))
    {
        QMessageBox::critical(this,"Options","Veuillez remplir tous les champs",QMessageBox::Ok);
    }
    else
    {
        if (ui->lineEditDossierDistantImages->text().right(1) != "/")
        {
            ui->lineEditDossierDistantImages->setText(ui->lineEditDossierDistantImages->text().append("/"));
        }
        //Il n'y a pas de champs sans valeur donc on peut passer à la sauvegarde du fichier
        saveXML();
        //Sauvegarde fait maintenant il faut modifier les valeurs des parametres dans le programme
        dirPict=ui->lineEditDossierDefautImages->text();
        dirSav=ui->lineEditDossierSauvegardes->text();
        dirTmp=ui->lineEditDossierTemp->text();
        cmdNav=ui->lineEditCommandeNavigateur->text();
        pseudoWp=ui->lineEditPseudoWordpress->text();
        addrSite=ui->lineEditAdresseSite->text();
        addrPub=ui->lineEditAdressePublication->text();
        editPict=ui->lineEditEditeurImages->text();
        dirDistPict=ui->lineEditDossierDistantImages->text();
        corrOrtho = ui->lineEditDictionnairePath->text();
        cecPrinter = ui->cecPrinter->isChecked();
        sendAuto = ui->sendAuto->isChecked();
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

        close();
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

void Options::saveXML()
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
    writer.writeTextElement("addrSite",ui->lineEditAdresseSite->text());
    writer.writeTextElement("addrPub",ui->lineEditAdressePublication->text());
    writer.writeTextElement("editPict",ui->lineEditEditeurImages->text());
    writer.writeTextElement("dirDistPict",ui->lineEditDossierDistantImages->text());
    QString activeCeCPrinter = ui->cecPrinter->isChecked() ? "1": "0";
    writer.writeTextElement("cecPrinter",activeCeCPrinter);
    QString sendType = ui->sendAuto->isChecked() ? "1" : "0";
    writer.writeTextElement("sendAuto", sendType);

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
    writer.writeEndElement();

    file.close();

}
