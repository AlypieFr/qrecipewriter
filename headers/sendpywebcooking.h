#ifndef SENDPYWEBCOOKING_H
#define SENDPYWEBCOOKING_H

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

class SendPyWebCooking : public QDialog
{
    Q_OBJECT

public:
    explicit SendPyWebCooking(QWidget *parent = 0);
    void init(QString title_lu, QString mainPicture_lu, QString mainPictureName_lu, QString precision_lu, QString description_lu, QString coupDeCoeur_lu,
              QList<int> tpsPrep_lu, QList<int> tpsCuis_lu, QList<int> tpsRep_lu, QStringList categories_lu, QStringList ingredients_lu,
              QStringList material_lu, QStringList instructions_lu, QStringList proposals_lu, int nbPeople_lu, int nbPeopleMax_lu,
              bool publish_lu, QString user_lu, QString password_lu, int config_lu, QDialog *envoiEnCours_lu);

private:
    QString title, mainPicture, mainPictureName, precision, description, coupDeCoeur;
    int tpsPrep, tpsCuis, tpsRep;
    QStringList categories;
    QVariantMap ingredients;
    QVariantMap ingredients_groups;
    QVariantMap ingredients_in_groups;
    QVariantList material, instructions, proposals;
    int nbPeople, nbPeopleMax;
    QString user, passwd;
    int config;
    bool publish;
    QDialog *envoiEnCours;
    QStringList otherPicts;

private slots:
    void handle_result(HttpRequestWorker *worker);

private:
    void sendRecipe();
    void buildIngredients(QStringList ingrsList);
    QVariantList buildMaterial(QStringList mats);
    QVariantList buildInstructions(QStringList instrsList);
    QVariantList buildProposal(QStringList props);
    QString insertPictures(QString item);
    double getQuantity(QString qte);
};

#endif // SENDPYWEBCOOKING_H
