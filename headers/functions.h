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
    static bool saveRecipeFromPwcJson(QVariantMap recipe, int config, QFile *tmpFile);
    static QMap<QString, QStringList> loadRecipe(QString fileName);
    static QStringList loadCategories();
    static QString generateHtmlCode(QString titre, QString mainPicture, int hPrep, int minPrep, int hCuis, int minCuis, int jRep,
                                    int hRep, int minRep, int nbPersonnes, int nbPersonnes2, QString precision,
                                    QString description, QString ingredients, QString materiel, QString preparation,
                                    QString conseils, int config);
    static QString getSimpleListWithSubLists(QStringList items);
    static QStringList makeSimpleList(QString text, bool isMat=false);
    static QStringList makeSimpleListWithSubLists(QString text);
    static QString getSimpleList(QStringList items);
    static QString getNumberedList(QStringList items);
    static QStringList makeNumberedList(QString text);
    static QString insertLinks(QString data);
    static int restoreLinks(QString *data, QMap<QString, QString> *liens, int nbLien);
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
    static QString get_ingredient_type(QString ingr);
    static bool validate_quantity(QString qte);
    static QStringList get_wp_tags(QVariantList tags);
    static QString get_img_wp_tag(QString img);
};

#endif // FUNCTIONS_H
