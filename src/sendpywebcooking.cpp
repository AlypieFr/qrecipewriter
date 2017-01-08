#include "sendpywebcooking.h"

extern QHash<int,QHash<QString, QString>> serverConfs;

SendPyWebCooking::SendPyWebCooking(QWidget *parent) :
    QDialog(parent)
{

}

void SendPyWebCooking::init(QString title_lu, QString mainPicture_lu, QString precision_lu, QString description_lu, QString coupDeCoeur_lu,
                            QList<int> tpsPrep_lu, QList<int> tpsCuis_lu, QList<int> tpsRep_lu, QStringList categories_lu,
                            QStringList ingredients_lu, QStringList material_lu, QStringList instructions_lu, QStringList proposals_lu,
                            int nbPeople_lu, int nbPeopleMax_lu, bool publish_lu, QString user_lu, QString password_lu, int config_lu,
                            QDialog *envoiEnCours_lu) {
    title = title_lu;
    mainPicture = mainPicture_lu;
    precision = precision_lu;
    description = description_lu;
    coupDeCoeur = coupDeCoeur_lu;
    tpsPrep = tpsPrep_lu;
    tpsCuis = tpsCuis_lu;
    tpsRep = tpsRep_lu;
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
                ingredients[id_ingr] = ingr_obj;

                //Add ingredient to it's group:
                if (current_ig_group == -1) {
                    QHash<QString, QVariant> ig_group;
                    ig_group["level"] = current_level-1;
                    ig_group["nb"] = nb_group;
                    ig_group["title"] = "";
                    ingredients_groups[id_group] = ig_group;
                    current_ig_group = id_group;
                    id_group++;
                    nb_group++;
                }
                if (!ingredients_in_groups.contains(current_ig_group))
                    ingredients_in_groups[current_ig_group] = QList<int>();
                ingredients_in_groups[current_ig_group].append(id_ingr);

                id_ingr++;
                nb_ingr++;
            }
            else {
                //It's an ingredient group
                QHash<QString, QVariant> ig_group;
                ig_group["level"] = level;
                ig_group["nb"] = nb_group;
                ig_group["name"] = ingr_base;
                ingredients_groups[id_group] = ig_group;
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
            ig_group["name"] = comm;
            ingredients_groups[id_group] = ig_group;
            id_group++;
            nb_group++;
        }
    }
}

QList<QHash<QString,QVariant>> SendPyWebCooking::buildMaterial(QStringList mats) {
    QList<QHash<QString,QVariant>> objs;
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
                objs.append(mat_obj);
            }
            else {
                QHash<QString,QVariant> mat_obj;
                mat_obj["nb"] = nb;
                mat_obj["name"] = exp.cap(2);
                mat_obj["quantity"] = -1;
                mat_obj["isComment"] = true;
                objs.append(mat_obj);
            }
            nb++;
        }
    }
    return objs;
}

QList<QHash<QString,QVariant>> SendPyWebCooking::buildProposal(QStringList props) {
    QRegExp exp ("0\\|(.+)");
    QList<QHash<QString,QVariant>> objs;
    int nb = 0;
    foreach (QString prop, props) {
        if (prop.contains(exp)) {
            QHash<QString,QVariant> prop_obj;
            prop_obj["nb"] = nb;
            prop_obj["text_prop"] = exp.cap(1);
            objs.append(prop_obj);
            nb++;
        }
    }
    return objs;
}

QList<QHash<QString, QVariant> > SendPyWebCooking::buildInstructions(QStringList instrsList) {
    QList<QHash<QString,QVariant>> instrs;
    QRegExp exp ("([\\d\\.]+)\\|(.+)");
    QRegExp exp_comm ("comm\\|(.+)");
    int nb = 0;
    foreach (QString instr, instrsList) {
        if (instr.contains(exp)) {
            QStringList id = exp.cap(1).split(".");
            int level = id.length() - 1;
            nb = id[id.length() - 2].toInt() - 1;
            QHash<QString,QVariant> instr_obj;
            instr_obj["nb"] = nb;
            instr_obj["level"] = level;
            instr_obj["text_inst"] = exp.cap(2);
            instrs.append(instr_obj);
        }
        else if (instr.contains(exp_comm)) {
            nb++;
            QHash<QString,QVariant> instr_obj;
            instr_obj["nb"] = nb;
            instr_obj["level"] = 0;
            instr_obj["text_inst"] = exp_comm.cap(1);
            instrs.append(instr_obj);
        }
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
    HttpRequestInput input(serverConfs[config]["addrSite"] + "/api/", "POST", user, passwd);

    //ADD DATA:
    input.add_file("main_picture", mainPicture, NULL, "image/jpeg");
    input.add_var("title", title);
    input.add_var("precision", precision);
    input.add_var("description", description);
    input.add_var("coup_de_coeur", coupDeCoeur);
    input.add_var("tps_prep", QString::number(tpsPrep));
    QJsonObject json = QJsonObject::fromVariantMap(data);
    QJsonDocument jsonDoc = QJsonDocument(json);

    //SEND POST
    HttpRequestWorker *worker = new HttpRequestWorker(this);
    connect(worker, SIGNAL(on_execution_finished(HttpRequestWorker*)), this, SLOT(handle_result(HttpRequestWorker*)));
    worker->execute(&input);
}
