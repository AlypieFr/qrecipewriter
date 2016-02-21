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

#ifndef INSERTVIDEO_H
#define INSERTVIDEO_H

#include <QCloseEvent>
#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QRegExp>
#include <qmath.h>

namespace Ui {
class InsertVideo;
}

class InsertVideo : public QDialog
{
    Q_OBJECT

public:
    explicit InsertVideo(QWidget *parent = 0);
    ~InsertVideo();
    void init(int largeur, int hauteur, int startTime, int bordure, bool fullscreen, bool suggestions, bool commandes, bool titreVideo, bool confidentialite, QString idVideo);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_importFromHtml_clicked();

private:
    Ui::InsertVideo *ui;
    bool checkUrl(QString url);

public:
    bool ok;
    QString urlVideo, idVideo;
    int largeur, hauteur, startTime, bordure;
    bool fullscreen, suggestions, commandes, titreVideo, confidentialite;
};

#endif // INSERTVIDEO_H
