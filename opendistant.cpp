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
        recipesByCats[cat] = QStringList();
        QCheckBox *catI = new QCheckBox(cat);
        catI->setMaximumHeight(21);
        catI->setMinimumHeight(21);
        catLay->addWidget(catI);
        connect(catI, SIGNAL(stateChanged(int)), this, SLOT(stateChanged()));;
    }
    FileDownloader *fdower = new FileDownloader(addrSite + "/requests/getPostsJson.php?user=" + pseudoWp, "Récupération de la liste des recettes...", parentWidget);
    QByteArray resData = fdower->downloadedData();
    bool ok;
    QVariantMap result = QJsonWrapper::parseJson(resData, &ok).toMap();
    if (!ok) {
        QMessageBox::critical(this, "Erreur !", "Une erreur est survenue lors de la récupération de la liste des recettes\nVeuillez contacter le support.",
                                      QMessageBox::Ok);
    }
    else if (result["success"].toString() == "true") {
        QList<QVariant> recipesRaw = result["recettes"].toList();
        foreach (QVariant recipeRaw, recipesRaw) {
            QMap<QString, QVariant> recipe = recipeRaw.toMap();
            recipe["cats"] = recipe["cats"].toStringList();
            recipes[recipe["title"].toString()] = recipe;
            foreach (QString cat, recipe["cats"].toStringList()) {
                recipesByCats[cat.replace("&", "&&")].append(recipe["title"].toString());
            }
            items.append(recipe["title"].toString());
        }
        ui->listRecipes->addItems(items);
        ui->listRecipes->sortItems();
        updateNbRecipes(items.count());
    }
    else {
        QMessageBox::critical(this, "Erreur !", "Une erreur est survenue lors de la récupération de la liste des recettes\nVeuillez contacter le support.",
                                      QMessageBox::Ok);
    }

    this->exec();
}

void OpenDistant::stateChanged() {
    QStringList catsSelected;
    foreach(QObject *box, ui->scrollCatsContent->children()) {
        QCheckBox *cbox = qobject_cast<QCheckBox*>(box);
        if (cbox) {
            if (cbox->isChecked()) {
                catsSelected.append(cbox->text());
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
        QMessageBox::critical(this, "Aucune recette sélectionnée", "Veuillez sélectionner une recette à ouvrir !");
    }
}

void OpenDistant::on_listRecipes_doubleClicked()
{
    on_buttonBox_accepted();
}
