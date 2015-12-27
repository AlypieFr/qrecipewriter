#ifndef OPENDISTANT_H
#define OPENDISTANT_H

#include "filedownloader.h"
#include "Json.h"

#include <QCheckBox>
#include <QDialog>
#include <QMessageBox>
#include <QModelIndex>
#include <QRegExp>
#include <QScrollArea>
#include <QVBoxLayout>

namespace Ui {
class OpenDistant;
}

class OpenDistant : public QDialog
{
    Q_OBJECT

public:
    explicit OpenDistant(QWidget *parent = 0);
    ~OpenDistant();
    void init();
    int idRecipeToOpen;

private slots:
    void on_search_textChanged(const QString &arg1);
    void on_catCheckBox_stateChanged();

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

    void on_listRecipes_doubleClicked();

private:
    Ui::OpenDistant *ui;
    QWidget *parentWidget;
    QStringList items;
    QMap<QString,QMap<QString,QVariant> > recipes;
    QMap<QString, QStringList > recipesByCats;
    void updateNbRecipes(int nbRecipes);
};

#endif // OPENDISTANT_H
