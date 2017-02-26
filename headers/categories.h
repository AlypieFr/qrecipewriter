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

#ifndef CATEGORIES_H
#define CATEGORIES_H

#include "functions.h"

#include <QDialog>
#include <QFile>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLayout>
#include <QLineEdit>
#include <QMap>
#include <QPushButton>
#include <QSignalMapper>
#include <QVBoxLayout>
#include <QWidget>
#include <QXmlStreamWriter>

namespace Ui {
class Categories;
}

class Categories : public QDialog
{
    Q_OBJECT


public slots:
    void init(QStringList cats);
    void reset();

private slots:
    void delCatPushed(int id);
    void addCatPushed();
    void on_valeurs_def_clicked();
    void on_valider_clicked();
    void on_annuler_clicked();

public:
    static bool saveRecipe(QString filename);
    explicit Categories(QWidget *parent = 0);
    ~Categories();

private:
    QString cat_save;
    QWidget* wid;
    Ui::Categories *ui;
    QVBoxLayout *vLay;
    QMap<int, QWidget*> sendW;
    int idCatList;
    QMap<int,QLineEdit*> catList;
    QSignalMapper mapper;

private:
    void initCats(QStringList cats);
    void keyPressEvent(QKeyEvent *e);
};

#endif // CATEGORIES_H
