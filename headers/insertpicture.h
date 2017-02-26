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
