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

#ifndef SENDAUTOMATIC_H
#define SENDAUTOMATIC_H

#include "functions.h"
#include "login.h"

#include <QCoreApplication>
#include <QDataStream>
#include <QDialog>
#include <QKeyEvent>
#include <QPlainTextEdit>
#include <QProcess>

class SendWordpress : public QDialog
{
    Q_OBJECT

public:
    explicit SendWordpress(QWidget *parent = 0);
    ~SendWordpress();
    void init(QString htmlCode_lu, QString titre_lu, QStringList categories_lu, QList<int> tpsPrep, QList<int> tpsCuis,
              QList<int> tpsRep, QString mainPicture_lu, QString excerpt_lu, QString coupDeCoeur_lu,
              QString user_lu, QString passwd_lu, int config_lu, bool publier_lu, QDialog *envoiEnCours_lu);

private slots:
    void errorDetails_clicked();

    void errorOk_clicked();

private:
    QString htmlCode, excerpt, titre, mainPicture, tags;
    QStringList categories;
    QString user, passwd;
    int config;
    bool publier;
    QString resultSend;
    QDialog *envoiEnCours;
    QDialog *errorShow;
    QVBoxLayout *verrorShowContent;
    QPlainTextEdit *showError;
    bool isErrorDetailsOpened;
    QLabel *mainError;
    QSize errorSize;
    QPushButton *details;
    bool isSending;

private:
    QString makeExcerpt (QStringList descWords, QString tpsPrep, QString tpsCuis, QString tpsRep);
    QString makeTags(QList<int> tpsPrep, QList<int> tpsCuis, QList<int> tpsRep);
    void sendRecipe();
};

#endif // SENDAUTOMATIC_H
