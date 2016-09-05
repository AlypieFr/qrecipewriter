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

#include "opendistant.h"
#include "ui_opendistant.h"

extern QString addrSite;
extern QStringList namesCats;
extern QString pseudoWp;

OpenDistant::OpenDistant(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenDistant)
{
    ui->setupUi(this);
    parentWidget = parent;
}

OpenDistant::~OpenDistant()
{
    delete ui;
}

void OpenDistant::init() {
    idRecipeToOpen = -1;
    ui->categoriesBox->setContentsMargins(0,10,0,0);
    /*ui->categoriesBox->setMinimumHeight(70);*/
    QVBoxLayout *catLay = new QVBoxLayout(ui->scrollCatsContent);
    catLay->setMargin(0);
    catLay->setContentsMargins(0,10,0,0);
    catLay->setSpacing(2);
    foreach (QString cat, namesCats) {
        QCheckBox *catI = new QCheckBox(cat);
        catI->setMaximumHeight(21);
        catI->setMinimumHeight(21);
        catLay->addWidget(catI);
        cboxes.append(catI);
        connect(catI, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));;
    }
    FileDownloader *fdower = new FileDownloader(addrSite + "/requests/getPostsJson.php?user=" + pseudoWp, tr("Récupération de la liste des recettes..."), parentWidget);
    QByteArray resData = fdower->downloadedData();
    QJsonParseError ok;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(resData, &ok);
    if (!jsonDoc.isNull() && ! jsonDoc.isEmpty()) {
        QJsonObject json = jsonDoc.object();
        QVariantMap result = json.toVariantMap();
        if (result["success"].toString() == "true") {
            QList<QVariant> recipesRaw = result["recettes"].toList();
            foreach (QVariant recipeRaw, recipesRaw) {
                QMap<QString, QVariant> recipe = recipeRaw.toMap();
                QStringList catsOld = recipe["cats"].toStringList();
                QStringList catsNew;
                foreach (QString catO, catsOld) {
                    catsNew.append(catO.replace("&", ""));
                }
                recipe["cats"] = catsNew;
                recipes[recipe["title"].toString()] = recipe;
                foreach (QString cat, recipe["cats"].toStringList()) {
                    recipesByCats[cat].append(recipe["title"].toString());
                }
                items.append(recipe["title"].toString());
            }
            ui->listRecipes->addItems(items);
            ui->listRecipes->sortItems();
            updateNbRecipes(items.count());
        }
        else {
            QMessageBox::critical(this, tr("Erreur !"), tr("Une erreur est survenue lors de la récupération de la liste des recettes\nVeuillez contacter le support."),
                                      QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::critical(this, tr("Erreur !"), tr("Une erreur est survenue lors de la récupération de la liste des recettes\nVeuillez contacter le support."),
                                      QMessageBox::Ok);
        qCritical() << "Error while parsing JSON: " + ok.errorString();
    }

    this->exec();
}

void OpenDistant::stateChanged() {
    QStringList catsSelected;
    foreach(QObject *box, ui->scrollCatsContent->children()) {
        QCheckBox *cbox = qobject_cast<QCheckBox*>(box);
        if (cbox) {
            if (cbox->isChecked()) {
                QString catSlct = cbox->text().replace("&", "");
                catsSelected.append(catSlct);
            }
        }
    }
    if (catsSelected.count() > 0) {
        items = recipesByCats[catsSelected[0]];
        if (catsSelected.count() > 1) {
            for (int i = 0; i < catsSelected.count(); ++i) {
                foreach (QString recipe, recipesByCats[catsSelected[i]]) {
                    if (items.indexOf(recipe) == -1) {
                        items.append(recipe);
                    }
                }
            }
        }
    }
    else {
        items = recipes.keys();
    }
    ui->listRecipes->clear();
    ui->listRecipes->addItems(items);
    ui->listRecipes->sortItems();
    updateNbRecipes(items.count());

}

void OpenDistant::on_search_textChanged(const QString &arg1)
{
    QString srch = arg1;
    srch = srch.replace(QRegExp("e", Qt::CaseInsensitive), "[eêëéè]")
               .replace(QRegExp("a", Qt::CaseInsensitive), "[aâäà]")
               .replace(QRegExp("i", Qt::CaseInsensitive), "[iîï]")
               .replace(QRegExp("o", Qt::CaseInsensitive), "[oôö]")
               .replace(QRegExp("u", Qt::CaseInsensitive), "[uûüù]")
               .replace(QRegExp("y", Qt::CaseInsensitive), "[yŷÿ]");
    QRegExp regExp(srch, Qt::CaseInsensitive, QRegExp::Wildcard);
    ui->listRecipes->clear();
    QStringList it = items.filter(regExp);
    ui->listRecipes->addItems(it);
    ui->listRecipes->sortItems();
    updateNbRecipes(it.count());
}

void OpenDistant::updateNbRecipes(int nbRecipes) {
    ui->nbTotal->setText(QString("%1").arg(nbRecipes));
    if (nbRecipes == 1) {
        ui->label_3->setText("recette");
    }
    else {
        ui->label_3->setText("recettes");
    }
}

void OpenDistant::on_buttonBox_rejected()
{
    this->close();
}

void OpenDistant::on_buttonBox_accepted()
{
    QList<QListWidgetItem*> selection = ui->listRecipes->selectedItems();
    if (selection.count() > 0) {
        QListWidgetItem *select = selection[0];
        idRecipeToOpen = recipes[select->text()]["id"].toInt();
        this->close();
    }
    else {
        QMessageBox::critical(this, tr("Aucune recette sélectionnée"), tr("Veuillez sélectionner une recette à ouvrir !"));
    }
}

void OpenDistant::on_listRecipes_doubleClicked()
{
    on_buttonBox_accepted();
}

void OpenDistant::on_pushButton_clicked()
{
    foreach (QCheckBox* cbox, cboxes) {
        cbox->setChecked(false);
    }
}
