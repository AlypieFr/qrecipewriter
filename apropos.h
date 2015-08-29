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

#ifndef APROPOS_H
#define APROPOS_H

#include <QDialog>

namespace Ui {
class Apropos;
}

class Apropos : public QDialog
{
    Q_OBJECT

public:
    explicit Apropos(QWidget *parent = 0);
    ~Apropos();
    void init();

private:
    Ui::Apropos *ui;
};

#endif // APROPOS_H
