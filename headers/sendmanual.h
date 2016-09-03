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

#ifndef SENDMANUAL_H
#define SENDMANUAL_H

#include <QApplication>
#include <QClipboard>
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QProcess>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QTextStream>

namespace Ui {
class SendManual;
}

class SendManual : public QDialog
{
    Q_OBJECT

public:
    explicit SendManual(QWidget *parent = 0);
    ~SendManual();
    void init(QString titre_lu, QString htmlCode_lu, QString excerpt_lu, QStringList imagesToAdd_lu, QStringList categories_lu);

private slots:
    void on_closeDialog_clicked();

    void on_saveInFile_clicked();

    void on_copyTitre_clicked();

    void on_copyHtmlCode_clicked();

    void on_copyExcerpt_clicked();

    void on_openInline_clicked();

private:
    Ui::SendManual *ui;

private:
    QString titre;
    QString htmlCode;
    QString excerpt;
    QStringList imagesToAdd;
    QStringList categories;
};

#endif // SENDMANUAL_H
