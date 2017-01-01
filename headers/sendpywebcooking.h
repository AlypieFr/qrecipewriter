#ifndef SENDPYWEBCOOKING_H
#define SENDPYWEBCOOKING_H

#include <QDialog>

class SendPyWebCooking : public QDialog
{
    Q_OBJECT

public:
    explicit SendPyWebCooking(QWidget *parent = 0);
    void init(QString title_lu, QString mainPicture_lu, QString precision_lu, QString description_lu, QString coupDeCoeur_lu,
              QList<int> tpsPrep_lu, QList<int> tpsCuis_lu, QList<int> tpsRep_lu, QStringList categories_lu, QStringList ingredients_lu,
              QStringList material_lu, QStringList instructions_lu, QStringList proposals_lu, int nbPeople_lu, int nbPeopleMax_lu,
              bool publish_lu, QString user_lu, QString password_lu, int config_lu, QDialog *envoiEnCours_lu);

private:
    QString title, mainPicture, precision, description, coupDeCoeur;
    QList<int> tpsPrep, tpsCuis, tpsRep;
    QStringList categories, ingredients, material, instructions, proposals;
    int nbPeople, nbPeopleMax;
    QString user, passwd;
    int config;
    bool publish;
    QDialog *envoiEnCours;

private:
    void sendRecipe();

};

#endif // SENDPYWEBCOOKING_H
