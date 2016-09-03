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

#ifndef QINPUTMULTILINE_H
#define QINPUTMULTILINE_H

#include <QDialog>

namespace Ui {
class QInputMultiline;
}

class QInputMultiline : public QDialog
{
    Q_OBJECT

public:
    explicit QInputMultiline(QWidget *parent = 0, QString title = "Dialog", QString message = "Entrez le texte", QString text = "", bool wordWrap = true, int width=300, int height=400);
    ~QInputMultiline();

private:
    Ui::QInputMultiline *ui;

public:
    QString text;
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // QINPUTMULTILINE_H
