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

#ifndef SEARCHUPDATES_H
#define SEARCHUPDATES_H

#include "functions.h"

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QObject>
#include <QDebug>

class SearchUpdates : public QWidget
{
    Q_OBJECT
public:
    explicit SearchUpdates(QWidget *parent=0);
    void start();

signals:

private slots:
    void replyFinished(QNetworkReply* pReply);

private:
    QNetworkAccessManager *manager;
    QWidget *_parent;
};

#endif // SEARCHUPDATES_H
