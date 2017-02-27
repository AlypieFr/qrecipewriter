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
    if (serverConfs[config]["recCoupDeCoeur"] == "1") {
        tags += coupDeCoeur_lu;
    }

    tpsPrep = tpsPrep_lu[0] * 60 + tpsPrep_lu[1];
    tpsCuis = tpsCuis_lu[0] * 60 + tpsCuis_lu[1];
    tpsRep = tpsRep_lu[0] * 24 * 60 + tpsRep_lu[1] * 60 + tpsRep_lu[2];

    this->sendRecipe();
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
        else if (worker->error_type == QNetworkReply::ContentNotFoundError) {
            QMessageBox::critical((QWidget*)this->parent(), tr("Une erreur est survenue"), "Impossible de se connecter. Le plugin Wordpress est-il installé et activé sur votre blog ?");
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
    input.add_var("tps_prep", QString::number(tpsPrep));
    input.add_var("tps_cuis", QString::number(tpsCuis));
    input.add_var("tps_rep", QString::number(tpsRep));

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
