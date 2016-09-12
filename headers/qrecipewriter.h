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

#ifndef CECWRITER_H
#define CECWRITER_H

#include "categories.h"
#include "listviewdelegate.h"
#include "options.h"
#include "functions.h"
#include "apropos.h"
#include "sendmanual.h"
#include "sendautomatic.h"
#include "spellcheckdialog.h"
#include "spellchecker.h"
#include "insertvideo.h"
#include "insertpicture.h"
#include "filedownloader.h"
#include "qinputmultiline.h"
#include "coupdecoeur.h"
#include "opendistant.h"
#include "searchupdates.h"

#include <QMainWindow>

//Graphic components
#include <QApplication>
#include <QChar>
#include <QCheckBox>
#include <QColor>
#include <QDate>
#include <QDateTime>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QFontDatabase>
#include <QInputDialog>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QList>
#include <QListView>
#include <QMap>
#include <qmath.h>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QRegExp>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QTextStream>
#include <QTimer>
#include <QTranslator>
#include <QVBoxLayout>
#include <QWidget>

namespace Ui {


class QRecipeWriter;
}

class QRecipeWriter : public QMainWindow
{
    Q_OBJECT

public:
    explicit QRecipeWriter(QWidget *parent = 0);
    ~QRecipeWriter();
    void resizeEvent(QResizeEvent* event);
    bool isMax;
    void setLogoWindow();
    QString openStartupFile;

private slots:

    bool eventFilter(QObject *object, QEvent *event);

    void format_clicked(QString typeF);

    void on_actionOptions_triggered();

    void on_actionGerer_les_categories_triggered();

    void on_mainPicture_clicked();

    void on_grasButton_clicked();

    void on_enregistrer_clicked();

    void on_listIngr_doubleClicked(const QModelIndex &index);

    void on_listMat_doubleClicked(const QModelIndex &index);

    void on_ingrListPlus_clicked();

    void on_ingrListMoins_clicked();

    void on_prepListPlus_clicked();

    void on_prepListMoins_clicked();

    void on_listCons_doubleClicked(const QModelIndex &index);

    void on_listPrep_doubleClicked(const QModelIndex &index);

    void handleSaveValider();

    void handleSaveAnnuler();

    void handleChooseLnkValider();

    void handleChooseLnkAnnuler();

    void refreshState();

    void on_action_propos_triggered();

    void on_actionOuvrir_une_recette_existante_triggered();

    void on_listIngr_customContextMenuRequested(const QPoint &pos);

    void on_listMat_customContextMenuRequested(const QPoint &pos);

    void on_listPrep_customContextMenuRequested(const QPoint &pos);

    void on_listCons_customContextMenuRequested(const QPoint &pos);

    void on_italicButton_clicked();

    void on_soulignButton_clicked();

    void on_commButton_clicked();

    void on_lienButton_clicked();

    void on_imgButton_clicked();

    void on_abcButton_clicked();

    void on_envoyer_clicked();

    void on_actionCorrection_orthographique_triggered();

    void on_actionNouvelle_Recette_triggered();

    void on_apercu_clicked();

    void on_actionEnregistrer_sous_triggered();

    void on_actionEnregistrer_triggered();

    void on_actionApercu_triggered();

    void on_actionEnvoyer_triggered();

    void on_actionAide_triggered();

    void on_actionRechercher_une_mise_jour_triggered();

    void on_noPrint_clicked();

    void on_printOnly_clicked();

    void on_minPrep_editingFinished();

    void on_minCuis_editingFinished();

    void on_minRep_editingFinished();

    void on_movie_clicked();

    void on_editIngr_selectionChanged();

    void on_editMat_selectionChanged();

    void on_editPrep_selectionChanged();

    void on_editCons_selectionChanged();

    void on_description_selectionChanged();

    void on_editIngr_customContextMenuRequested(const QPoint &pos);

    void on_editMat_customContextMenuRequested(const QPoint &pos);

    void on_editPrep_customContextMenuRequested(const QPoint &pos);

    void on_editCons_customContextMenuRequested(const QPoint &pos);

    void on_setCoupDeCoeur_clicked();

    void on_actionOuvrir_une_recette_en_ligne_triggered();

    void on_switchNbPers_clicked();

    void on_tab_ingrs_currentChanged(int index);

    void on_titlegroup_ingr_selectionChanged();

    void on_comment_ingr_selectionChanged();

    void on_tab_mats_currentChanged(int index);

    void on_comment_mat_selectionChanged();

public slots:
    void init();
    void config();
    void launch();

private:
    Ui::QRecipeWriter *ui;

private:
    int ingrEdit, matEdit, prepEdit, consEdit; //Keeps if item in ingr, mat, prep or cons is in edition (>=0) or is a new item (==-1)
    int idIngr; //Keeps identation level for ingredients (0 is basic level, 1 level 1, etc...)
    QString idPrep; //Keeps identation level and puce number : idPrep.split('.').size() - 2 indicates indentation level, idPrep.split('.') store puce number in the penultimate item list
    QMap<QCheckBox*, QString> categories; //Keeps categories QCheckbox shown in application associated to their name
    int idLien; //Indicates the number to the next link in link balises shown (<L1> texte </L1> for example)
    QHBoxLayout *catLay; //Layout for categories
    QStandardItemModel *model1, *model2, *model3, *model4; //Four models for respectively ingredients, materiel, préparation and conseils listviews
    QString imgFile, imgFileName; //Main picte file (full path) and filename (name of the file only)
    QString coupDeCoeur; //Coup de coeur of the recipe
    QString saveFileName; //filename (without extension) used for save the file
    QDialog *chooseSaveFileName, *chooseLink, *selectSendType; //Dialog for, respectively choose fileName of the saved recipe, set http adress of a link inserted, and choose the send type (manual or automatic)
    QString addrLnk; //The http adress link choosen (needed it to be stored just after user click ok button, then we store it in the QMap.
    QLineEdit *chooseSaveFN, *chooseAddrLnk; //LineEdits in, respectively choosSaveFileName and chooseLink QDialogs
    bool doSave; //Stores if we do save (we do it if user has not click on annuler
    QColor colorEdit; //The background color of an edited item (it is set depending on the color of the text in the QListViews)
    bool ingrComm, matComm, prepComm, consComm; //Stores if we are writting comments on, respectively,
    QString htmlCode, excerpt; //Stores generated html code and excerpt (an excerpt is an abstract of the recipe that is shown in the home page of wordpress, if the theme used is compatible
    //To keep saved variables:
    QString saveTitre, saveMainImage, savePrecision, saveDescription, saveIngredients, saveMateriel, savePreparation, saveConseils, saveNbPersonnes;
    QStringList saveCategories;
    int saveHPrep, saveMinPrep, saveHCuis, saveMinCuis, saveJRep, saveHRep, saveMinRep;
    QString balise; //Stores the tag that will be deleted
    QStringList balises; //Stores tags that will be deleted
    int resetPosCaret; //Stores position of the cursot at which we must set cursor after deleting tags
    int roundValueToFive(double value);
    bool askSaveFile();
    void toggleEditPict();

private:
    void closeEvent (QCloseEvent *event);
    void changeEvent (QEvent * event);
    void saveSizeAndQuit();
    void resetCats();
    void resetFields();
    void saveVariables(QString ingr, QString mat, QString prep, QString cons);
    bool checkHasBeenModified();
    bool isReadyTosend(bool alert);
    void modifierIngr(const QModelIndex &index);
    void modifierMat(const QModelIndex &index);
    void modifierPrep(const QModelIndex &index);
    void modifierCons(const QModelIndex &index);
    void supprimerIngr();
    void supprimerMat();
    void supprimerPrep();
    void supprimerCons();
    QString convert(QString ret);
    void makeHtmlCode();
    void openEditor();
    void checkRemoveLnkBckp(QString text, int caretPos);
    void checkRemoveLnkSupprInLineEdit(QLineEdit* areaTxt);
    void checkRemoveLnkSupprInPlainTextEdit(QPlainTextEdit* areaTxt);
    void checkRemoveLnkSuppr(QString text, int caretPos);
    void checkRemoveLnkSlct(QString text, QList<int> slct);
    QString checkRemoveBalBckp(QString text, int caretPos);
    QString checkRemoveBalSuppr(QString text, int caretPos);
    QString checkRemoveBalSlct(QString text, QList<int> slct);
    void checkRemoveBalBckpInLineEdit(QLineEdit* areaTxt);
    void checkRemoveBalBckpInPlainTextEdit(QPlainTextEdit* areaTxt);
    void checkRemoveBalSupprInLineEdit(QLineEdit* areaTxt);
    void checkRemoveBalSupprInPlainTextEdit(QPlainTextEdit* areaTxt);
    void checkRemoveBalSlctInLineEdit(QLineEdit* areaTxt);
    void checkRemoveBalSlctInPlainTextEdit(QPlainTextEdit* areaTxt);
    void deleteBalInLineEdit(QLineEdit* areaTxt);
    void deleteBalInPlainTextEdit(QPlainTextEdit* areaTxt);
    void deleteBalsInLineEdit(QLineEdit* areaTxt);
    void deleteBalsInPlainTextEdit(QPlainTextEdit* areaTxt);
    void updatePrepItems();
    void insertIngredient(QString qte, QString unit, QString name);
    void insertIngredientGroupTitle(QString title);
    void insertIngredientComment(QString comment);
    void insertMateriel(QString qte, QString name);
    void insertMaterielComment(QString comment);
    void insertPreparation(QString text);
    void insertConseil(QString text);
    void sendManual();
    void sendAutomatic();
    void loadRecipe(QString fileName, bool testReadyToSend);
    void searchUpdate();
    void startAbcCheck(bool silent);
};

#endif // CECWRITER_H
