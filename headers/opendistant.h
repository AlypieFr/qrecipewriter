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

#ifndef OPENDISTANT_H
#define OPENDISTANT_H

#include "filedownloader.h"
#include "Json.h"

#include <QCheckBox>
#include <QDialog>
#include <QList>
#include <QMessageBox>
#include <QModelIndex>
#include <QRegExp>
#include <QScrollArea>
#include <QVBoxLayout>

namespace Ui {
class OpenDistant;
}

class OpenDistant : public QDialog
{
    Q_OBJECT

public:
    explicit OpenDistant(QWidget *parent = 0);
    ~OpenDistant();
    void init();
    int idRecipeToOpen;

private slots:
    void on_search_textChanged(const QString &arg1);
    void stateChanged();

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_listRecipes_doubleClicked();

    void on_pushButton_clicked();

private:
    Ui::OpenDistant *ui;
    QWidget *parentWidget;
    QStringList items;
    QMap<QString,QMap<QString,QVariant> > recipes;
    QMap<QString, QStringList > recipesByCats;
    void updateNbRecipes(int nbRecipes);
    QList<QCheckBox*> cboxes;
};

#endif // OPENDISTANT_H
