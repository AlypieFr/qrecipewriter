#include "sendpywebcooking.h"

extern QHash<int,QHash<QString, QString>> serverConfs;

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
    description = description_lu;
    coupDeCoeur = coupDeCoeur_lu;
    tpsPrep = tpsPrep_lu[0] * 60 + tpsPrep_lu[1];
    tpsCuis = tpsCuis_lu[0] * 60 + tpsCuis_lu[1];
    tpsRep = tpsRep_lu[0] * 24 * 60 + tpsRep_lu[1] * 60 + tpsRep_lu[2];
    categories = categories_lu;
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

void SendPyWebCooking::buildIngredients(QStringList ingrsList) {
    QList<QHash<QString,QVariant>> ingrs;
    int nb_group = 0;
    int nb_ingr = 0;
    int id_ingr = 0;
    int id_group = 0;
    int current_ig_group = -1;
    int current_level = 0;
    QRegExp exp ("(\\d+)\\|(.+)");
    QRegExp exp_comm ("comm\\|(.+)");
    QHash<int, QStringList> ig_in_grp;
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
                double qte = ingrParts[1].toDouble();
                QString unit = ingrParts[2];
                QString name = ingrParts.mid(3).join("#");
                QHash<QString, QVariant> ingr_obj;
                ingr_obj["quantity"] = qte;
                ingr_obj["unit"] = unit;
                ingr_obj["name"] = name;
                ingr_obj["nb"] = nb_ingr;
                ingredients[QString::number(id_ingr)] = QString(QJsonDocument(QJsonObject::fromVariantHash(ingr_obj)).toJson());;

                //Add ingredient to it's group:
                if (current_ig_group == -1) {
                    QHash<QString, QVariant> ig_group;
                    ig_group["level"] = current_level;
                    ig_group["nb"] = nb_group;
                    ig_group["title"] = "";
                    ingredients_groups[QString::number(id_group)] = QString(QJsonDocument(QJsonObject::fromVariantHash(ig_group)).toJson());;
                    current_ig_group = id_group;
                    id_group++;
                    nb_group++;
                }
                if (!ig_in_grp.contains(current_ig_group))
                    ig_in_grp[current_ig_group] = QStringList();
                ig_in_grp[current_ig_group].append(QString::number(id_ingr));

                id_ingr++;
                nb_ingr++;
            }
            else {
                //It's an ingredient group
                QHash<QString, QVariant> ig_group;
                ig_group["level"] = level;
                ig_group["nb"] = nb_group;
                ig_group["title"] = ingr_base;
                ingredients_groups[QString::number(id_group)] = QString(QJsonDocument(QJsonObject::fromVariantHash(ig_group)).toJson());
                current_ig_group = id_group;
                id_group++;
                nb_group++;
                nb_ingr = 0;
            }
        }
        else if (ingr.contains(exp_comm)) {
            QString comm = exp_comm.cap(1);
            //Create empty ingr group with level 0 (it's a comment!!)
            QHash<QString, QVariant> ig_group;
            ig_group["level"] = 0;
            ig_group["nb"] = nb_group;
            ig_group["title"] = comm;
            ingredients_groups[QString::number(id_group)] = QString(QJsonDocument(QJsonObject::fromVariantHash(ig_group)).toJson());
            id_group++;
            nb_group++;
        }
    }
    foreach (int id_grp, ig_in_grp.keys()) {
        ingredients_in_groups[QString::number(id_grp)] = QString(QJsonDocument(QJsonArray::fromStringList(ig_in_grp[id_grp])).toJson());
    }
}

QStringList SendPyWebCooking::buildMaterial(QStringList mats) {
    QStringList objs;
    QRegExp exp ("(\\d+|comm)\\|(.+)");
    int nb = 0;
    foreach (QString mat, mats) {
        if (mat.contains(exp)) {
            if (exp.cap(1) != "comm") {
                QStringList mat_parts = exp.cap(2).split("#");
                int qte = mat_parts[1].toInt();
                QString name = mat_parts.mid(2).join("#");
                QHash<QString,QVariant> mat_obj;
                mat_obj["nb"] = nb;
                mat_obj["name"] = name;
                mat_obj["quantity"] = qte;
                mat_obj["isComment"] = false;
                objs.append(QString(QJsonDocument(QJsonObject::fromVariantHash(mat_obj)).toJson()));
            }
            else {
                QHash<QString,QVariant> mat_obj;
                mat_obj["nb"] = nb;
                mat_obj["name"] = exp.cap(2);
                mat_obj["quantity"] = -1;
                mat_obj["isComment"] = true;
                objs.append(QString(QJsonDocument(QJsonObject::fromVariantHash(mat_obj)).toJson()));
            }
            nb++;
        }
    }
    return objs;
}

QStringList SendPyWebCooking::buildProposal(QStringList props) {
    QRegExp exp ("0\\|(.+)");
    QStringList objs;
    int nb = 0;
    foreach (QString prop, props) {
        if (prop.contains(exp)) {
            QHash<QString,QVariant> prop_obj;
            prop_obj["nb"] = nb;
            prop_obj["text_prop"] = exp.cap(1);
            objs.append(QString(QJsonDocument(QJsonObject::fromVariantHash(prop_obj)).toJson()));
            nb++;
        }
    }
    return objs;
}

QStringList SendPyWebCooking::buildInstructions(QStringList instrsList) {
    QStringList instrs;
    QRegExp exp ("([\\d\\.]+)\\|(.+)");
    QRegExp exp_comm ("comm\\|(.+)");
    int nb = 0;
    foreach (QString instr, instrsList) {
        if (instr.contains(exp)) {
            QStringList id = exp.cap(1).split(".");
            int level = id.length() - 1;
            QHash<QString,QVariant> instr_obj;
            instr_obj["nb"] = nb;
            instr_obj["level"] = level;
            instr_obj["text_inst"] = exp.cap(2);
            instrs.append(QString(QJsonDocument(QJsonObject::fromVariantHash(instr_obj)).toJson()));
        }
        else if (instr.contains(exp_comm)) {
            QHash<QString,QVariant> instr_obj;
            instr_obj["nb"] = nb;
            instr_obj["level"] = 0;
            instr_obj["text_inst"] = exp_comm.cap(1);
            instrs.append(QString(QJsonDocument(QJsonObject::fromVariantHash(instr_obj)).toJson()));
        }
        nb++;
    }
    return instrs;
}

void SendPyWebCooking::handle_result(HttpRequestWorker *worker) {
    QString msg;

    if (worker->error_type == QNetworkReply::NoError) {
        // communication was successful
        msg = "Success - Response: " + worker->response;
    }
    else {
        // an error occurred
        msg = "Error: " + worker->error_str;
    }

    qDebug() << msg;
    envoiEnCours->close();
}

void SendPyWebCooking::sendRecipe() {
    //INIT CONNEXION TO WEBSITE:
    qDebug() << serverConfs[config]["addrSite"] + "/api/";
    HttpRequestInput input(serverConfs[config]["addrSite"] + "/api/", "POST", user, passwd);

    //ADD DATA:
    input.add_var("title", title);
    input.add_var("precision", precision);
    input.add_var("description", description);
    input.add_var("coup_de_coeur", coupDeCoeur);
    input.add_var("nb_people", QString::number(nbPeople));
    input.add_var("nb_people_max", QString::number(nbPeopleMax));
    input.add_var("tps_prep", QString::number(tpsPrep));
    input.add_var("tps_cuis", QString::number(tpsCuis));
    input.add_var("tps_rep", QString::number(tpsRep));
    input.add_var("categories", QString(QJsonDocument(QJsonArray::fromStringList(categories)).toJson()));
    input.add_var("ingredients", QString(QJsonDocument(QJsonObject::fromVariantHash(ingredients)).toJson()));
    input.add_var("ingredients_groups", QString(QJsonDocument(QJsonObject::fromVariantHash(ingredients_groups)).toJson()));
    input.add_var("ingredients_in_groups", QString(QJsonDocument(QJsonObject::fromVariantHash(ingredients_in_groups)).toJson()));
    input.add_var("material", QString(QJsonDocument(QJsonArray::fromStringList(material)).toJson()));
    input.add_var("instructions", QString(QJsonDocument(QJsonArray::fromStringList(instructions)).toJson()));
    input.add_var("proposals", QString(QJsonDocument(QJsonArray::fromStringList(proposals)).toJson()));
    input.add_var("publish", publish ? "1" : "0");
    input.add_file("main_picture", mainPicture, mainPictureName, "image/jpg");

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
