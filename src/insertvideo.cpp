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

#include "insertvideo.h"
#include "ui_insertvideo.h"

InsertVideo::InsertVideo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertVideo)
{
    ui->setupUi(this);
}

void InsertVideo::init(int largeur, int hauteur, int startTime, int bordure, bool fullscreen, bool suggestions, bool commandes, bool titreVideo, bool confidentialite, QString idVideo) {
    //Reset values:
    int startMin = qFloor(startTime / 60);
    int startSec = startTime - (startMin * 60);
    ok = false;
    ui->urlVideo->setText(idVideo);
    ui->largeur->setValue(largeur);
    ui->hauteur->setValue(hauteur);
    ui->startMin->setValue(startMin);
    ui->startSec->setValue(startSec);
    ui->bordure->setValue(bordure);
    ui->fullScreen->setChecked(fullscreen);
    ui->suggestions->setChecked(suggestions);
    ui->commandes->setChecked(commandes);
    ui->titreVideo->setChecked(titreVideo);
    ui->confidentialite->setChecked(confidentialite);
}

InsertVideo::~InsertVideo()
{
    delete ui;
}

bool InsertVideo::checkUrl(QString url) {
    QRegExp exp ("^http[s]?://www.youtube.com/watch?v=([\\w-]+)[^\"\\s]*$");
    idVideo = "";
    if (url.contains(exp)) {
        idVideo = exp.cap(1);
    }
    else {
        exp.setPattern("^http[s]?://www.youtube-nocookie.com/watch?v=([\\w-]+)[^\"\\s]*$");
        if (url.contains(exp)) {
            idVideo = exp.cap(1);
        }
        else {
            exp.setPattern("^http[s]?://www.youtube.com/embed/([\\w-]+)[^\"\\s]*$");
            if (url.contains(exp)) {
                idVideo = exp.cap(1);
            }
            else {
                exp.setPattern("^http[s]?://www.youtube-nocookie.com/embed/([\\w-]+)[^\"\\s]*$");
                if (url.contains(exp)) {
                    idVideo = exp.cap(1);
                }
                else {
                    exp.setPattern("^([\\w-]+)$");
                    if (url.contains(exp)) {
                        idVideo = exp.cap(1);
                    }
                    else {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}

void InsertVideo::on_buttonBox_accepted()
{
    urlVideo = ui->urlVideo->text();
    largeur = ui->largeur->value();
    hauteur = ui->hauteur->value();
    startTime = (60 * ui->startMin->value()) + ui->startSec->value();
    bordure = ui->bordure->value();
    fullscreen = ui->fullScreen->isChecked();
    suggestions = ui->suggestions->isChecked();
    commandes = ui->commandes->isChecked();
    titreVideo = ui->titreVideo->isChecked();
    confidentialite = ui->confidentialite->isChecked();
    if (urlVideo != "") {
        //Check URL:
        bool validUrl = checkUrl(urlVideo);
        if (validUrl) {
            if (idVideo.length() == 11) {
                ok = true;
                this->close();
            }
            else {
                QMessageBox::critical(this, "URL incorrect", "Erreur : l'URL de la vidéo que vous avez renseigné n'est pas un lien youtube valide : \nl'id de la vidéo doit être de 11 caractères.");
            }
        }
        else {
            QMessageBox::critical(this, "URL incorrect", "Erreur : l'URL de la vidéo que vous avez renseigné n'est pas un lien youtube valide !");
        }
    }
    else {
        QMessageBox::critical(this, "Champs vides", "Erreur : vous n'avez pas renseigné d'URL !");
    }
}

void InsertVideo::on_buttonBox_rejected()
{
    this->close();
}

void InsertVideo::on_importFromHtml_clicked()
{
    QString html = QInputDialog::getText(this, "HTML video tag", "Entrez le code HTML youtube de la vidéo :");
    if (html != "") {
        QRegExp exp ("<iframe.+src=\"([^\"]+)\".*></iframe>");
        if (html.contains(exp)) {
            QString url = exp.cap(1);
            if (checkUrl(url)) {
                exp.setPattern("http[s]?://[^?]+");
                if (url.contains(exp))
                    ui->urlVideo->setText(exp.cap(0).replace("-nocookie", ""));
                if (url.contains("-nocookie")) {
                    ui->confidentialite->setChecked(true);
                }
                else {
                    ui->confidentialite->setChecked(false);
                }
                if (url.contains("rel=0")) {
                    ui->suggestions->setChecked(false);
                }
                else {
                    ui->suggestions->setChecked(true);
                }
                if (url.contains("controls=0")) {
                    ui->commandes->setChecked(false);
                }
                else {
                    ui->commandes->setChecked(true);
                }
                if (url.contains("showinfo=0")) {
                    ui->titreVideo->setChecked(false);
                }
                else {
                    ui->titreVideo->setChecked(true);
                }
                exp.setPattern("<iframe.+width=\"(\\d+)\".*></iframe>");
                if (html.contains(exp)) {
                    ui->largeur->setValue(exp.cap(1).toInt());
                }
                else {
                    ui->largeur->setValue(640);
                }
                exp.setPattern("<iframe.+height=\"(\\d+)\".*></iframe>");
                if (html.contains(exp)) {
                    ui->hauteur->setValue(exp.cap(1).toInt());
                }
                else {
                    ui->hauteur->setValue(480);
                }
                exp.setPattern("<iframe.+frameborder=\"(\\d+)\".*></iframe>");
                if (html.contains(exp)) {
                    ui->bordure->setValue(exp.cap(1).toInt());
                }
                else {
                    ui->bordure->setValue(0);
                }
                if (html.contains("allowfullscreen")) {
                    ui->fullScreen->setChecked(true);
                }
                else {
                    ui->fullScreen->setChecked(false);
                }
            }
            else {
                QMessageBox::critical(this, "Données incorrect", "Erreur : le code HTML fournit contient un url non valide !");
            }
        }
        else {
            QMessageBox::critical(this, "Données incorrect", "Erreur : le code HTML fournit n'est pas correct !");
        }
    }
}
