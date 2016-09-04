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

#include "qrecipewriter.h"
#include "options.h"
#include <QApplication>
#include "categories.h"
#include "apropos.h"
#include "sendmanual.h"
#include "sendautomatic.h"

#include <QDir>
#include <QFile>
#include <QLocale>
#include <QMessageBox>
#include <QProcess>
#include <QString>
#include <QSplashScreen>
#include <QTimer>
#include <QTextCodec>

//To use system locale:
#include <QLocale>
#include <QTranslator>
#include <QLibraryInfo>

/**
 * POINTERS TO QDIALOGS USED BY THE PROGRAM
 */
void *ptr2opt = NULL;
void *ptr2cat = NULL;
void *ptr2apropos = NULL;
void *ptr2sendManual = NULL;
void *ptr2sendAutomatic = NULL;
void *ptr2insertVideo = NULL;

/**
 * GLOBAL VARIABLES
 */
QString shareDir = "/usr/share/qrecipewriter";
QString userDir = QDir::homePath(); //User directory
QString confDir = userDir + "/.QRecipeWriter/"; //Directory where all config is stored
QString confFile; //Configuration file, where options choosen by user is saved
QFile *confCatFile; //File containing categories choosen by user, if it doesn't want default categories

QStringList otherPicts; //List of pictures, except main picture
QString dirPict = ""; //Default local directory of pictures
QString systExp = ""; //Operating system of the user
#ifdef Q_OS_LINUX
QString dirTmp = "/tmp/QRecipeWriter";
#else
QString dirTmp = userDir + "/AppData/Local/Temp/QRecipeWriter";
#endif
QString cmdNav = ""; //Command to launch navigator
QString dirSav; //Directory used to save recipes or "conseils & technics"
QString pseudoWp = ""; //Pseudo worpdress used for generate "aperçu"
QString dirDistPict = ""; //Distant directory where pictures are saved in server
QString addrSite = ""; //Address of the website
QString addrPub = ""; //Address of publication (XML-RPC)
QString editPict = ""; //Program for advanced picture editor
QString corrOrtho = ""; //Main dictionnary path
QString correction = ""; //Path to the dictionnary (prefix of .dic and .aff files)
QStringList namesCats; //Contains names of categories
bool cancel = false; //Keep if user cancel orthographical correction
bool richSnippets = false; //Active rich snippets for recipes in google search results
bool recPrinter = false; //CeC Printer status
bool sendAuto = true; //Send in an automatic manner
bool recSearch = false; //Activate or not search module
bool recCoupDeCoeur = false; //Activate or not "coups de coeur"
int configActive = 1;
int idRecipe = -1;
bool openLastDir_sauvegarde = false;
bool openLastDir_Img = false;
bool checkF7beforeSend = false;
bool autoCheckUpdt = true;
QString updtUrl = "";

QString VERSION = "3.2.0";
QString QTVERSION;

QMap<QString, QString> liens; //Contains all links registered for one recipe

int main(int argc, char *argv[])
{
    //QCoreApplication app(argc, argv);
    //QStringList args = app.arguments();
#if QT_VERSION < 0x050000
    QTVERSION = "4";
    // UTF-8 Encoding (useless in Qt5, as UTF-8 is now the default encoding)
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#else
    QTVERSION = "5";
#endif
    QApplication a(argc, argv);
    QString openFilename = NULL;
    if (a.arguments().count() > 1) {
        QFile *oFile = new QFile(a.arguments()[1]);
        if (oFile->exists()) {
            openFilename = oFile->fileName();
        }
    }
    QSplashScreen *splash = new QSplashScreen;
    splash -> setPixmap(QPixmap(":/images/splash.png"));
    splash->show();
#ifdef Q_OS_LINUX
    systExp = "linuxAutre"; //Operating system of the user
    QFile fileOs ("/etc/os-release");
    if (fileOs.exists())
    {
        fileOs.open(QFile::ReadOnly);
        QTextStream stream (&fileOs);
        QString osTxt = stream.readAll();
        if (osTxt.contains("ID=slackware")) {
            systExp = "linuxSlackware";
        }
        else if (osTxt.contains("ID=ubuntu") || osTxt.contains("ID=debian")) {
            systExp = "linuxDebian";
        }
        else if (osTxt.contains("ID=arch") || osTxt.contains("ID=manjaro")) {
            systExp = "linuxArch";
        }
        fileOs.close();
    }
#else
    systExp = "windows";
#endif

    //Set locale:
    QString locale = QLocale::system().name();
    /*
     *
     * DEBUG
     *
     */
    locale = "en_US";
    //END DEBUG
    QString lang = locale.split("_")[0];
    QTranslator translator;
    translator.load(QString("qt_") + locale, QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    QString trFile = ":/i18n/qrecipewriter_"+ lang +".qm";
    if (QFile(trFile).exists()) {
        translator.load(trFile);
    }
    a.installTranslator(&translator);

    //Define values
    confFile  = confDir +".config";
    QString tmp = confDir + ".cats";
    confCatFile = new QFile(tmp);
    #ifdef Q_OS_LINUX
    QFile fileDictTest1 ("/usr/share/myspell/dicts/fr_FR.dic");
    QFile fileDictTest2 ("/usr/share/myspell/dicts/fr_FR.aff");
    if (fileDictTest1.exists() && fileDictTest2.exists())
    {
        corrOrtho = "/usr/share/myspell/dicts/fr_FR";
    }
    #else
        #if _WIN64
            QFile fileDictTest1 ("C:/hunspellX64/dicts/fr.dic");
            QFile fileDictTest2 ("C:/hunspellX64/dicts/fr.aff");
            if (fileDictTest1.exists() && fileDictTest2.exists())
            {
                corrOrtho = "C:/hunspellX64/dicts/fr";
            }
        #else
            QFile fileDictTest1 ("C:/hunspellI686/dicts/fr.dic");
            QFile fileDictTest2 ("C:/hunspellI686/dicts/fr.aff");
            if (fileDictTest1.exists() && fileDictTest2.exists())
            {
                corrOrtho = "C:/hunspellI686/dicts/fr";
            }
        #endif
    #endif

    QRecipeWriter w;
    w.openStartupFile = openFilename;

    //Define Dialogs:
    Apropos *apropos = new Apropos(&w);
    ptr2apropos = apropos;
    QFile configFile(confFile);
    QDir tmpD (dirTmp);
    QTimer::singleShot(2000,splash,SLOT(close())); //Start splash screen and keep it 2 seconds
    if (configFile.exists())
    {
        w.init();
        if (!tmpD.exists())
        {
            tmpD.mkpath(".");
        }
        QTimer::singleShot(2000,&w,SLOT(launch())); //Start the application
    }
    else
    {
        QTimer::singleShot(2000,&w,SLOT(config())); //Start initial config
    }
    return a.exec();
}
