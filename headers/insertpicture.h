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

#ifndef INSERTPICTURE_H
#define INSERTPICTURE_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

namespace Ui {
class InsertPicture;
}

class InsertPicture : public QDialog
{
    Q_OBJECT

public:
    explicit InsertPicture(QWidget *parent = 0);
    ~InsertPicture();

private:
    Ui::InsertPicture *ui;

public:
    int w,h;
    bool ok, centerImg;
    void init(QString imgFile, QString afficher, int width, int height, QString filename, bool center);
    QString print, largeur, hauteur, filename;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_largeur_valueChanged(int arg1);
    void on_hauteur_valueChanged(int arg1);
    void on_changePicture_clicked();
};

#endif // INSERTPICTURE_H
