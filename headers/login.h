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

#ifndef LOGIN_H
#define LOGIN_H

#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QTextStream>

#include "functions.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_valider_clicked();

    void on_annuler_clicked();

    void on_config_nb_currentIndexChanged(int index);

public:
    void init(bool showPublish=true);
    bool getAccepted();
    QString getUsername();
    QString getPassword();
    int getConfig();
    bool getPublish();

private:
    Ui::Login *ui;
    bool accepted;
    QString username;
    QString password;
    int config;
    bool publish;
};

#endif // LOGIN_H
