#include "sendwordpress.h"

extern QHash<int,QHash<QString, QString>> serverConfs;
extern QString cmdNav; //Command to launch navigator
extern int idRecipe;
extern QStringList otherPicts;

SendWordpress::SendWordpress(QWidget *parent) :
    QDialog(parent)
{

}

void SendWordpress::init(QString htmlCode_lu, QString title_lu, QString mainPicture_lu, QString mainPictureName_lu,
                         QString excerpt_lu, QString coupDeCoeur_lu,
                         QList<int> tpsPrep_lu, QList<int> tpsCuis_lu, QList<int> tpsRep_lu, QStringList categories_lu,
                         bool publish_lu, QString user_lu, QString password_lu, int config_lu,
                         QDialog *envoiEnCours_lu) {
    htmlCode = htmlCode_lu;
    title = title_lu;
    mainPicture = mainPicture_lu;
    mainPictureName = mainPictureName_lu;
    excerpt = excerpt_lu;
    foreach (QString cat, categories_lu) {
        categories.append(cat.replace("&amp;&amp;", "&").replace("&amp;", "&"));
    }
    publish = publish_lu;
    user = user_lu;
    passwd = password_lu;
    config = config_lu;
    envoiEnCours = envoiEnCours_lu;

    tags = "";
    if (serverConfs[config]["recSearch"] == "1" && (categories.size() > 1 || categories[0] != "Base")) {
        tags = makeTags(tpsPrep_lu, tpsCuis_lu, tpsRep_lu);
        if (serverConfs[config]["recCoupDeCoeur"] == "1") {
            tags += ",";
        }
    }
    if (serverConfs[config]["recCoupDeCoeur"] == "1") {
        tags += coupDeCoeur_lu;
    }
    this->sendRecipe();
}

QString SendWordpress::makeTags(QList<int> tpsPrep, QList<int> tpsCuis, QList<int> tpsRep)
{
    int tPrep = tpsPrep[0] * 60 + tpsPrep[1];
    int tCuis = tpsCuis[0] * 60 + tpsCuis[1];
    int tRepos = tpsRep[0] * 24 * 60 + tpsRep[1] * 60 + tpsRep[2];
    int tTotal = tPrep + tCuis + tRepos;
    QString tags = "tpsPrep=" + QString::number(tPrep) + ",tpsCuis="
            + QString::number(tCuis) + ",tpsRepos="
            + QString::number(tRepos) + ",tpsTotal="
            + QString::number(tTotal);
    return tags;
}


void SendWordpress::handle_result(HttpRequestWorker *worker) {
    if (worker->error_type == QNetworkReply::NoError) {
        QJsonParseError *error = new QJsonParseError();
        QJsonDocument jsondoc = QJsonDocument::fromJson(worker->response, error);
        QJsonObject jsonobj = jsondoc.object();
        QVariantMap map = jsonobj.toVariantMap();
        if (error->error == QJsonParseError::NoError) {
            if (map["success"].toInt() == 1) {
                int rep = QMessageBox::information((QWidget*)this->parent(), tr("Envoi terminé"), tr("Envoi terminé avec succès !\nVoulez-vous afficher la recette en ligne ?"), QMessageBox::Yes, QMessageBox::No);
                if (rep == QMessageBox::Yes)
                {
                    QString program = "\"" + cmdNav + "\" " + map["url"].toString();
                    QProcess *myProcess = new QProcess();
                    myProcess->setProcessChannelMode(QProcess::MergedChannels);
                    myProcess->start(program);
                }
            }
            else {
                QString message = map["message"].toString();
                if (!message.isNull() && !message.isEmpty()) {
                    QMessageBox::critical((QWidget*)this->parent(), tr("Une erreur est survenue"), message);
                }
                else {
                    QMessageBox::critical((QWidget*)this->parent(), tr("Une erreur est survenue"),
                                          tr("Une erreur inconnue s'est produite. Veuillez rapporter le bug."));
                }
            }
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

    envoiEnCours->close();
}

void SendWordpress::sendRecipe() {
    //INIT CONNEXION TO WEBSITE:
    HttpRequestInput input;
    input = HttpRequestInput(serverConfs[config]["addrSite"] + "/wp-json/qrecipewriter/v1/posts/", "POST", user, passwd);
    if (idRecipe == -1) {
        input.add_var("author", user);
    }
    else {
        input.add_var("id", QString::number(idRecipe));
    }

    //ADD DATA:
    input.add_var("title", title);
    input.add_var("content", htmlCode);
    input.add_var("excerpt", excerpt);
    input.add_var("categories", categories.join(","));
    input.add_var("tags", tags);
    input.add_var("thumbnail", mainPicture);
    input.add_var("published", publish ? "1" : "0");

    if (!mainPicture.startsWith("http"))
        input.add_file("main_picture", mainPicture, mainPictureName, "image/jpg");
    if (otherPicts.length() > 0) {
        foreach (QString opict, otherPicts) {
            QString opict_name = opict.mid(opict.lastIndexOf("/") + 1);
            QString ext = opict.mid(opict.lastIndexOf(".") + 1);
            QString format = "jpg";
            if (ext.toLower() == "png") {
                format = "png";
            }
            input.add_file("other_pictures", opict, opict_name, "image/" + format);
        }
    }

    //SEND POST
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_result(HttpRequestWorker*)));
    try {
        worker->execute(&input);
    }
    catch (const FileNotFoundException &e) {
        QMessageBox::critical((QWidget*)parent(), tr("Fichier non trouvé"), tr("Fichier image non trouvé : ") + e.getMessage());
        envoiEnCours->close();
    }
    catch (const GlobalException &e) {
        QMessageBox::critical((QWidget*)parent(), tr("Une erreur est survenue"), e.getMessage().isEmpty() ? tr("Une erreur inconnue est survenue. Veuillez rapporter le bug.") : e.getMessage());
        envoiEnCours->close();
    }
}
