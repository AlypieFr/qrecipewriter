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

#ifndef SENDWORDPRESS_H
#define SENDWORDPRESS_H

#include <QDialog>
#include <QDebug>
#include <QHash>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMessageBox>
#include <QNetworkReply>
#include <QProcess>
#include <QRegExp>
#include <QWidget>

#include "filenotfoundexception.h"
#include "functions.h"
#include "globalexception.h"
#include "httprequestworker.h"

class SendWordpress : public QDialog
{
    Q_OBJECT

public:
    explicit SendWordpress(QWidget *parent = 0);
    void init(QString htmlCode_lu, QString title_lu, QString mainPicture_lu, QString mainPictureName_lu,
              QString excerpt_lu, QString coupDeCoeur_lu,
              QList<int> tpsPrep_lu, QList<int> tpsCuis_lu, QList<int> tpsRep_lu, QStringList categories_lu,
              bool publish_lu, QString user_lu, QString password_lu, int config_lu, QDialog *envoiEnCours_lu);

private:
    QString htmlCode, title, mainPicture, mainPictureName, precision, description, excerpt, coupDeCoeur, tags;
    int tpsPrep, tpsCuis, tpsRep;
    QStringList categories;
    QString user, passwd;
    int config;
    bool publish;
    QDialog *envoiEnCours;

private slots:
    void handle_result(HttpRequestWorker *worker);

private:
    void sendRecipe();
};

#endif // SENDWORDPRESS_H
