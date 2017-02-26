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
