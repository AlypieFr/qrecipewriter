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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QCheckBox>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QPushButton>
#include <QRegExp>
#include <QSpacerItem>
#include <QString>
#include <QStringList>
#include <QVBoxLayout>
#include <QWidget>
#include <QXmlStreamReader>

//Using tinyxml2:
#include "tinyxml2.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
using namespace tinyxml2;

class Functions
{
public:
    Functions();

public:
    static void loadConfig();
    static bool saveRecipe(QString title, QStringList categories, QString tpsPrep,
                           QString tpsCuis, QString tpsRep, QString nbPers, QString precision,
                           QString description, QStringList ingredients, QStringList materiel,
                           QStringList preparation, QStringList conseils, QString picture, QMap<QString, QString> liens,
                           QString filename, QString coupDeCoeur);
    static QMap<QString, QStringList> loadRecipe(QString fileName);
    static QStringList loadCategories();
    static QString generateHtmlCode(QString titre, QString mainPicture, int hPrep, int minPrep, int hCuis, int minCuis, int jRep,
                                    int hRep, int minRep, int nbPersonnes, QString precision,
                                    QString description, QString ingredients, QString materiel, QString preparation,
                                    QString conseils);
    static QString getSimpleListWithSubLists(QStringList items);
    static QString getSimpleList(QStringList items);
    static QString getNumberedList(QStringList items);
    static QString insertLinks(QString data);
    static QString insertPictures(QString data);
    static QString insertMovies(QString data);
    static QStringList getSelectedCategories(QMap<QCheckBox*, QString> categories);
    static QStringList makeTimes (int hPrep, int minPrep, int hCuis, int minCuis, int jRep,
                                  int hRep, int minRep);
    static QString makeExcerpt(QStringList descWords, QString tpsPrep, QString tpsCuis, QString tpsRep);
    static bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath);
    static bool removeDir(const QString &dirName);
    static QString setPrintTags(QString text);
};

#endif // FUNCTIONS_H
