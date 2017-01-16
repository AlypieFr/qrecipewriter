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

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "filedownloader.h"

#include <QCheckBox>
#include <QDateTime>
#include <QDialog>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QJsonDocument>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QMap>
#include <QMessageBox>
#include <QObject>
#include <QPushButton>
#include <QRegExp>
#include <QSpacerItem>
#include <QString>
#include <QStringList>
#include <QTextStream>
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
    static QHash<int, QHash<QString, QString>> loadServerConfigs();
    static bool saveRecipeToFile(QString title, QStringList categories, QString tpsPrep,
                                 QString tpsCuis, QString tpsRep, QString nbPers, QString precision,
                                 QString description, QStringList ingredients, QStringList materiel,
                                 QStringList preparation, QStringList conseils, QString picture, QMap<QString, QString> liens,
                                 QFile *file, QString coupDeCoeur, int idRecipe);
    static bool saveRecipe(QString title, QStringList categories, QString tpsPrep,
                           QString tpsCuis, QString tpsRep, QString nbPers, QString precision,
                           QString description, QStringList ingredients, QStringList materiel,
                           QStringList preparation, QStringList conseils, QString picture, QMap<QString, QString> liens,
                           QString filename, QString coupDeCoeur, int idRecipe);
    static bool saveRecipeFromDist(QString title, QStringList categories, QString content, QString picture, QString coupDeCoeur, QFile *file, int idRecipe);
    static QMap<QString, QStringList> loadRecipe(QString fileName);
    static QStringList loadCategories();
    static QString generateHtmlCode(QString titre, QString mainPicture, int hPrep, int minPrep, int hCuis, int minCuis, int jRep,
                                    int hRep, int minRep, int nbPersonnes, int nbPersonnes2, QString precision,
                                    QString description, QString ingredients, QString materiel, QString preparation,
                                    QString conseils);
    static QString getSimpleListWithSubLists(QStringList items);
    static QStringList makeSimpleList(QString text, bool isMat=false);
    static QStringList makeSimpleListWithSubLists(QString text);
    static QString getSimpleList(QStringList items);
    static QString getNumberedList(QStringList items);
    static QStringList makeNumberedList(QString text);
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
    static QString makeRichSnippets(QString title, QString mainPicture, int hPrep, int minPrep, int hCuis, int minCuis, int jRep, int hRep, int minRep,
                             int nbPersonnes, QString precision, QString description, QStringList ingredients, QStringList preparation);
    static QString removeSpecialChars(QString str);
    static bool downloadPicture(QString url, QString fileName, QWidget *parent);
    static QStringList addCommentLines(QString txt);
    static bool saveLastDir(QString action, QString dir);
    static QString getLastDir(QString action);
    static void downloadUpdate(QString adresse, QWidget *parent);
    static bool is_config_valid(int id);
    static void write_categories_file(QStringList new_cats);
};

#endif // FUNCTIONS_H
