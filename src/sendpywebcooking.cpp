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

#include "sendpywebcooking.h"

extern QHash<int,QHash<QString, QString> > serverConfs;
extern QString cmdNav; //Command to launch navigator
extern int idRecipe;
extern QStringList otherPicts;

SendPyWebCooking::SendPyWebCooking(QWidget *parent) :
    QDialog(parent)
{

}

void SendPyWebCooking::init(QString title_lu, QString mainPicture_lu, QString mainPictureName_lu, QString precision_lu, QString description_lu, QString coupDeCoeur_lu,
                            QList<int> tpsPrep_lu, QList<int> tpsCuis_lu, QList<int> tpsRep_lu, QStringList categories_lu,
                            QStringList ingredients_lu, QStringList material_lu, QStringList instructions_lu, QStringList proposals_lu,
                            int nbPeople_lu, int nbPeopleMax_lu, bool publish_lu, QString user_lu, QString password_lu, int config_lu,
                            QDialog *envoiEnCours_lu) {
    title = title_lu;
    mainPicture = mainPicture_lu;
    mainPictureName = mainPictureName_lu;
    precision = precision_lu;
    description = Functions::insertLinks(description_lu);
    coupDeCoeur = coupDeCoeur_lu;
    tpsPrep = tpsPrep_lu[0] * 60 + tpsPrep_lu[1];
    tpsCuis = tpsCuis_lu[0] * 60 + tpsCuis_lu[1];
    tpsRep = tpsRep_lu[0] * 24 * 60 + tpsRep_lu[1] * 60 + tpsRep_lu[2];
    foreach (QString cat, categories_lu) {
        categories.append(cat.replace("&amp;&amp;", "&").replace("&amp;", "&"));
    }
    buildIngredients(ingredients_lu);
    if (material_lu.length() > 0)
        material = buildMaterial(material_lu);
    instructions = buildInstructions(instructions_lu);
    if (proposals_lu.length() > 0)
        proposals = buildProposal(proposals_lu);
    nbPeople = nbPeople_lu;
    nbPeopleMax = nbPeopleMax_lu;
    publish = publish_lu;
    user = user_lu;
    passwd = password_lu;
    config = config_lu;
    envoiEnCours = envoiEnCours_lu;
    this->sendRecipe();
}

QString SendPyWebCooking::insertPictures(QString item) {
    QRegExp exp ("\\[IMG:(\\w+):(\\d+):(\\d+)(:center)?:([^\\]]+)\\]");
    while (item.contains(exp))
    {
        QString imgBal = exp.cap(0);
        QString className = exp.cap(1);
        QString largeur = exp.cap(2);
        QString hauteur = exp.cap(3);
        bool center = exp.cap(4).length() > 0;
        QString img = exp.cap(5);
        otherPicts.append(img);
        QString img_filename = img.mid(img.lastIndexOf("/")+1);
        item = "[PICT:" + className + ":" + largeur + ":" + hauteur + (center ? ":center" : "") + ":" + img_filename + "]";
    }
    return item;
}



double SendPyWebCooking::getQuantity(QString qte) {
    bool ok;
    double qte_double = qte.toDouble(&ok);
    if (!ok) {
        QRegExp exp("^(\\d+)/(\\d+)$");
        if (exp.exactMatch(qte)) {
            qte_double = exp.cap(1).toDouble() / exp.cap(2).toDouble();
            qte_double = floor(qte_double * pow(10., 2) + .5) / pow(10., 2);
        }
    }
    return qte_double;
}

void SendPyWebCooking::buildIngredients(QStringList ingrsList) {
    int nb_group = 0;
    int nb_ingr = 0;
    int id_ingr = 0;
    int id_group = 0;
    int current_ig_group = -1;
    int current_level = 0;
    QRegExp exp ("(\\d+)\\|(.+)");
    QRegExp exp_comm ("comm\\|(.+)");
    QHash<QString, QStringList> ig_in_grp;
    foreach (QString ingr, ingrsList) {
        if (ingr.contains(exp)) {
            int level = exp.cap(1).toInt() + 1;
            if (level < current_level) {
                current_ig_group = -1;
                nb_ingr = 0;
            }
            current_level = level;
            QString ingr_base = exp.cap(2);
            if (ingr_base.startsWith("ingr#")) {
                //It's an ingredient
                QStringList ingrParts = ingr_base.split("#");
                double qte = getQuantity(ingrParts[1]);
                QString unit = ingrParts[2];
                QString name = ingrParts.mid(3).join("#");
                QVariantMap ingr_obj;
                ingr_obj["quantity"] = qte;
                ingr_obj["unit"] = unit;
                ingr_obj["name"] = Functions::insertLinks(name);
                ingr_obj["nb"] = nb_ingr;
                ingredients[QString::number(id_ingr)] = ingr_obj;

                //Add ingredient to it's group:
                if (current_ig_group == -1) {
                    QVariantMap ig_group;
                    ig_group["level"] = current_level;
                    ig_group["nb"] = nb_group;
                    ig_group["title"] = "";
                    ingredients_groups[QString::number(id_group)] = ig_group;
                    current_ig_group = id_group;
                    id_group++;
                    nb_group++;
                }
                if (!ig_in_grp.contains(QString::number(current_ig_group)))
                    ig_in_grp[QString::number(current_ig_group)] = QStringList();
                ig_in_grp[QString::number(current_ig_group)].append(QString::number(id_ingr));

                id_ingr++;
                nb_ingr++;
            }
            else {
                //It's an ingredient group
                QVariantMap ig_group;
                ig_group["level"] = level;
                ig_group["nb"] = nb_group;
                ig_group["title"] = Functions::insertLinks(ingr_base);
                ingredients_groups[QString::number(id_group)] = ig_group;
                current_ig_group = id_group;
                id_group++;
                nb_group++;
                nb_ingr = 0;
            }
        }
        else if (ingr.contains(exp_comm)) {
            QString comm = exp_comm.cap(1);
            //Create empty ingr group with level 0 (it's a comment!!)
            QVariantMap ig_group;
            ig_group["level"] = 0;
            ig_group["nb"] = nb_group;
            ig_group["title"] = Functions::insertLinks(comm);
            ingredients_groups[QString::number(id_group)] = ig_group;
            id_group++;
            nb_group++;
        }
    }
    foreach (QString id_grp, ig_in_grp.keys()) {
        ingredients_in_groups[id_grp] = ig_in_grp[id_grp];
    }
}

QVariantList SendPyWebCooking::buildMaterial(QStringList mats) {
    QVariantList objs;
    QRegExp exp ("(\\d+|comm)\\|(.+)");
    int nb = 0;
    foreach (QString mat, mats) {
        if (mat.contains(exp)) {
            if (exp.cap(1) != "comm") {
                QStringList mat_parts = exp.cap(2).split("#");
                int qte = mat_parts[1].toInt();
                QString name = mat_parts.mid(2).join("#");
                QVariantMap mat_obj;
                mat_obj["nb"] = nb;
                mat_obj["name"] = Functions::insertLinks(name);
                mat_obj["quantity"] = qte;
                mat_obj["is_comment"] = false;
                objs.append(mat_obj);
            }
            else {
                QVariantMap mat_obj;
                mat_obj["nb"] = nb;
                mat_obj["name"] = Functions::insertLinks(exp.cap(2));
                mat_obj["quantity"] = -1;
                mat_obj["is_comment"] = true;
                objs.append(mat_obj);
            }
            nb++;
        }
    }
    return objs;
}

QVariantList SendPyWebCooking::buildInstructions(QStringList instrsList) {
    QVariantList instrs;
    QRegExp exp ("([\\d\\.]+)\\|(.+)");
    QRegExp exp_comm ("comm\\|(.+)");
    int nb = 0;
    foreach (QString instr, instrsList) {
        if (instr.contains(exp)) {
            QStringList id = exp.cap(1).split(".");
            int level = id.length() - 1;
            QVariantMap instr_obj;
            instr_obj["nb"] = nb;
            instr_obj["level"] = level;
            instr_obj["text_inst"] = Functions::insertLinks(Functions::insertMovies(insertPictures(exp.cap(2))));
            instrs.append(instr_obj);
        }
        else if (instr.contains(exp_comm)) {
            QVariantMap instr_obj;
            instr_obj["nb"] = nb;
            instr_obj["level"] = 0;
            instr_obj["text_inst"] = Functions::insertLinks(Functions::insertMovies(insertPictures(exp_comm.cap(1))));
            instrs.append(instr_obj);
        }
        nb++;
    }
    return instrs;
}

QVariantList SendPyWebCooking::buildProposal(QStringList props) {
    QRegExp exp ("(0|comm)\\|(.+)");
    QVariantList objs;
    int nb = 0;
    foreach (QString prop, props) {
        if (prop.contains(exp)) {
            QVariantMap prop_obj;
            prop_obj["nb"] = nb;
            prop_obj["text_prop"] = Functions::insertLinks(Functions::insertMovies(insertPictures(exp.cap(2))));
            prop_obj["is_comment"] = exp.cap(1) == "comm";
            objs.append(prop_obj);
            nb++;
        }
    }
    return objs;
}

void SendPyWebCooking::handle_result(HttpRequestWorker *worker) {
    QString msg;

    if (worker->error_type == QNetworkReply::NoError) {
        // communication was successful
        //msg = "Success - Response: " + worker->response;
        QJsonParseError *error = new QJsonParseError();
        QJsonDocument jsondoc = QJsonDocument::fromJson(worker->response, error);
        QJsonObject jsonobj = jsondoc.object();
        QVariantMap map = jsonobj.toVariantMap();
        if (error->error == QJsonParseError::NoError) {
            if (map["status"].toInt() == 0) {
                int rep = QMessageBox::information((QWidget*)this->parent(), tr("Envoi terminé"), tr("Envoi terminé avec succès !\nVoulez-vous afficher la recette en ligne ?"), QMessageBox::Yes, QMessageBox::No);
                if (rep == QMessageBox::Yes)
                {
                    QString program = "\"" + cmdNav + "\" " + "http://" + map["url"].toString();
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

void SendPyWebCooking::sendRecipe() {
    //INIT CONNEXION TO WEBSITE:
    HttpRequestInput input;
    if (idRecipe == -1)
        input = HttpRequestInput(serverConfs[config]["addrSite"] + "/api/", "POST", user, passwd);
    else
        input = HttpRequestInput(serverConfs[config]["addrSite"] + "/api/recipe/by-id/" + QString::number(idRecipe), "PUT", user, passwd);

    //ADD DATA:
    input.add_var("title", title);
    input.add_var("precision", precision);
    input.add_var("description", description);
    if (serverConfs[config]["recCoupDeCoeur"] == "1") {
        QHash<QString,QString> coup_de_coeur_conv;
        coup_de_coeur_conv["no_coup_de_coeur"] = "0";
        coup_de_coeur_conv["coup_de_coeur_1"] = "1";
        coup_de_coeur_conv["coup_de_coeur_2"] = "2";
        coup_de_coeur_conv["coup_de_coeur_3"] = "3";
        input.add_var("coup_de_coeur", coup_de_coeur_conv[coupDeCoeur]);
    }
    input.add_var("nb_people", QString::number(nbPeople));
    input.add_var("nb_people_max", QString::number(nbPeopleMax));
    input.add_var("tps_prep", QString::number(tpsPrep));
    input.add_var("tps_cuis", QString::number(tpsCuis));
    input.add_var("tps_rep", QString::number(tpsRep));
    input.add_var("categories", QString(QJsonDocument(QJsonArray::fromStringList(categories)).toJson()));
    input.add_var("ingredients", QString(QJsonDocument(QJsonObject::fromVariantMap(ingredients)).toJson()));
    input.add_var("ingredients_groups", QString(QJsonDocument(QJsonObject::fromVariantMap(ingredients_groups)).toJson()));
    input.add_var("ingredients_in_groups", QString(QJsonDocument(QJsonObject::fromVariantMap(ingredients_in_groups)).toJson()));
    input.add_var("equipments", QString(QJsonDocument(QJsonArray::fromVariantList(material)).toJson()));
    input.add_var("instructions", QString(QJsonDocument(QJsonArray::fromVariantList(instructions)).toJson()));
    input.add_var("proposals", QString(QJsonDocument(QJsonArray::fromVariantList(proposals)).toJson()));
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
