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

extern QHash<int,QHash<QString, QString>> serverConfs;
extern QStringList namesCats;

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
    Login *login = new Login((QWidget*)this->parent());
    login->init(false);
    if (login->getAccepted()) {
        user = login->getUsername();
        passwd = login->getPassword();
        config = login->getConfig();

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

        this->open(config, serverConfs[config]["typeServer"]);
    }
}

void OpenDistant::addRecipe(QVariantMap recipe) {
    QStringList catsOld = recipe["categories"].toStringList();
    QStringList catsNew;
    foreach (QString catO, catsOld) {
        catsNew.append(catO.replace("&", ""));
    }
    recipe["categories"] = catsNew;
    recipes[recipe["title"].toString()] = recipe;
    foreach (QString cat, recipe["categories"].toStringList()) {
        recipesByCats[cat].append(recipe["title"].toString());
    }
    items.append(recipe["title"].toString());
}

void OpenDistant::refreshRecipesList() {
    ui->listRecipes->clear();
    ui->listRecipes->addItems(items);
    ui->listRecipes->sortItems();
    updateNbRecipes(items.count());
}

void OpenDistant::open(int config, QString typeServer) {
    openInProgress = new QDialog((QWidget*)this->parent());
    openInProgress->setModal(true);
    QLabel *lab = new QLabel("<b>" + tr("Récupération des recettes...") + "</b>");
    lab->setAlignment(Qt::AlignCenter);
    openInProgress->setWindowTitle("QRecipeWriter");
    QHBoxLayout *layEnvoiEnCours = new QHBoxLayout();
    layEnvoiEnCours->addWidget(lab);
    openInProgress->setLayout(layEnvoiEnCours);
    openInProgress->setFixedSize(300,50);
    openInProgress->setModal(false);
    HttpRequestInput input;
    if (typeServer == "pywebcooking") {
        input = HttpRequestInput(serverConfs[config]["addrSite"] + "/api/", "GET", user, passwd);
    }
    else { // IS wordpress
         input = HttpRequestInput(serverConfs[config]["addrSite"] + "/wp-json/qrecipewriter/v1/posts/" + user, "GET");
    }
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_result(HttpRequestWorker*)));
    worker->execute(&input);
    openInProgress->exec();
}

void OpenDistant::handle_result(HttpRequestWorker *worker) {
    openInProgress->close();
    if (worker->error_type == QNetworkReply::NoError) {
        // communication was successful
        QJsonParseError *error = new QJsonParseError();
        QJsonDocument jsondoc = QJsonDocument::fromJson(worker->response, error);
        if (error->error == QJsonParseError::NoError) {
            QJsonArray jsonobj = jsondoc.array();
            QVariantList recipes_r = jsonobj.toVariantList();
            foreach (QVariant recipe_raw, recipes_r) {
                QVariantMap recipe = recipe_raw.toMap();
                this->addRecipe(recipe);
            }
            this->refreshRecipesList();
            this->exec();
        }
        else {
            qDebug() << worker->response;
            qCritical() << "Error while parsing JSON: " + error->errorString();
            QMessageBox::critical((QWidget*)this->parent(), tr("Une erreur est survenue"), tr("Impossible de lire la réponse du serveur. Merci de rapporter le bug."));
        }
    }
    else {
        // an error occurred
        if (worker->error_type == QNetworkReply::ContentOperationNotPermittedError) {
            QMessageBox::critical((QWidget*)this->parent(), tr("Une erreur est survenue"), "Votre identificant ou vote mot de passe est incorrect");
        }
        else {
            QMessageBox::critical((QWidget*)this->parent(), tr("Une erreur est survenue"), worker->error_str);
        }
    }
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
