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

#include "sendwordpress.h"
#include <QTextStream>

extern QString shareDir;
extern QStringList otherPicts;
extern QString cmdNav; //Command to launch navigator
extern QString dirTmp;
extern QHash<int,QHash<QString, QString>> serverConfs;
extern int idRecipe;

SendWordpress::SendWordpress(QWidget *parent) :
    QDialog(parent)
{

}

SendWordpress::~SendWordpress()
{

}

/**
 * @brief SendWordpress::init Initialize send to Wordpress
 * @param htmlCode_lu
 * @param titre_lu
 * @param categories_lu
 * @param tpsPrep
 * @param tpsCuis
 * @param tpsRep
 * @param mainPicture_lu
 * @param excerpt_lu
 * @param coupDeCoeur_lu
 * @param user_lu: username
 * @param passwd_lu: password
 * @param config_lu: id of the server config to use
 * @param publier_lu: publish the recipe (bool)
 * @param envoiEnCours_lu: "Envoi en cours" wainting window already opened (to be closed at the end)
 */
void SendWordpress::init(QString htmlCode_lu, QString titre_lu, QStringList categories_lu, QList<int> tpsPrep, QList<int> tpsCuis,
                         QList<int> tpsRep, QString mainPicture_lu, QString excerpt_lu, QString coupDeCoeur_lu,
                         QString user_lu, QString passwd_lu, int config_lu, bool publier_lu, QDialog *envoiEnCours_lu)
{
    isSending = false;
    QString categ = categories_lu[0];
    isErrorDetailsOpened = false;
    htmlCode = htmlCode_lu;
    titre = titre_lu;
    //categories = categories_lu;
    foreach (QString cat, categories_lu) {
        categories.append(cat.replace("&amp;&amp;", "&amp;"));
    }
    mainPicture = mainPicture_lu;
    excerpt = excerpt_lu;
    envoiEnCours = envoiEnCours_lu;
    user = user_lu;
    passwd = passwd_lu;
    config = config_lu;
    publier = publier_lu;
    tags = "";
    if (serverConfs[config]["recSearch"] == "1" && (categories.size() > 1 || categories[0] != "Base")) {
        tags = makeTags(tpsPrep, tpsCuis, tpsRep);
        if (serverConfs[config]["recCoupDeCoeur"] == "1") {
            tags += ",";
        }
    }
    if (serverConfs[config]["recCoupDeCoeur"] == "1") {
        tags += coupDeCoeur_lu;
    }
    if (tags.isEmpty()) {
        tags = "null";
    }
    this->sendRecipe();
}

QString SendWordpress::makeExcerpt(QStringList descWords, QString tpsPrep, QString tpsCuis, QString tpsRep)
{
    QString descExpt = "";
    for(int i=0; i<qMin(descWords.length(),20); ++i)
        descExpt = descExpt + " " + descWords[i];
    QString expt = "<b>Préparation : "+tpsPrep+".";
    if(tpsRep !="")
        expt = expt + " Repos : "+tpsRep+".";
    if(tpsCuis !="")
        expt = expt + "<br/> Cuisson : "+tpsCuis+".";
    expt = expt + "</b> <br/> " + descExpt + "...";
    return expt;
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

void SendWordpress::sendRecipe()
{
    isSending = true;
    //Save htmlCode to tmpFile:
    QFile htmlFile(dirTmp + "/htmlCode.txt");
    if (htmlFile.exists())
        htmlFile.remove();
    htmlFile.open(QIODevice::WriteOnly);
    QTextStream stream (&htmlFile);
    stream.setCodec("UTF-8"); //Not useful on linux system as it's a default, but Windows has its own defaults....
    stream << user << "\n" << passwd << "\n" << titre << "\n" << excerpt << "\n<htmlCode>\n" << htmlCode << endl;
    htmlFile.close();
    QString cats = categories.join("|");
    QString oPicts = "null";
    if (otherPicts.size() > 0)
        oPicts = otherPicts.join("|");
    QString isPublier = "false";
    if (publier)
        isPublier = "true";
    QString path = QCoreApplication::applicationDirPath();
    //Starting java program to send recipe to the website. Don't worry, the java part
    //is very small and will take only few seconds (depending on the internet connexion
    //only) :
    QString stwPath = "";
    if (QDir(path + "/wordpress").exists()) {
        stwPath = path + "/wordpress";
    }
    else {
        stwPath = shareDir + "/wordpress";
    }
    QString program = "java -jar \"" + stwPath + "/SendToWordpress.jar\" \""
            + serverConfs[config]["addrPub"] + "\" \"" + cats + "\" \"" + tags + "\" \"" + mainPicture + "\" \"" + htmlFile.fileName()
            + "\" \"" + oPicts + "\" \"" + isPublier + "\" \"" + QString::number(idRecipe) + "\"";
    QProcess *myProcess = new QProcess();
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(program);
    myProcess->waitForFinished(-1); //-1 to wait until program has not finished, without any limit of time
    //Uuuh, it's finished, keeping if java program has send successfully or not the recipe:
    resultSend = myProcess->readAll();
    //Delete temp file:
    htmlFile.remove();
    //Take it in account and continue ;) :
    envoiEnCours->close();
    isSending = false;
    QStringList lines = resultSend.split("\n");
    int i = 0; bool messFound = false;
    while (i < lines.size() && !messFound) {
        if (lines[i].replace("\r", "") == "!!!Send recipe SUCCEEDED!!!" || lines[i].replace("\r", "") == "ERROR : send recipe failed.") {
            messFound = true;
        }
        else {
            i++;
        }
    }
    if (i < lines.size() && lines[i].replace("\r", "") == "!!!Send recipe SUCCEEDED!!!")
    {
        int rep = QMessageBox::information((QWidget*)this->parent(), tr("Envoi terminé"), tr("Envoi terminé avec succès !\nVoulez-vous afficher la recette en ligne ?"), QMessageBox::Yes, QMessageBox::No);
        if (rep == QMessageBox::Yes)
        {
            QString program = "\"" + cmdNav + "\" " + serverConfs[config]["addrSite"] + "/?p=" + lines[i+1].replace("\r", "");
            QProcess *myProcess = new QProcess();
            myProcess->setProcessChannelMode(QProcess::MergedChannels);
            myProcess->start(program);
        }
    }
    else
    {
        errorShow = new QDialog((QWidget*)this->parent());
        QVBoxLayout *vlay = new QVBoxLayout();
        mainError = new QLabel();
        verrorShowContent = new QVBoxLayout();
        verrorShowContent->addWidget(mainError);
        vlay->addLayout(verrorShowContent);
        details = new QPushButton(tr("Détails >>"));
        details->setFixedSize(100,30);
        connect(details, SIGNAL(clicked()), this, SLOT(errorDetails_clicked()));
        QPushButton *ok = new QPushButton("Ok");
        ok->setFixedSize(100,30);
        connect(ok, SIGNAL(clicked()), this, SLOT(errorOk_clicked()));
        QSpacerItem *hspacer = new QSpacerItem(10, 30, QSizePolicy::Expanding, QSizePolicy::Minimum);
        QHBoxLayout *butLay = new QHBoxLayout();
        butLay->addWidget(details);
        butLay->addSpacerItem(hspacer);
        butLay->addWidget(ok);
        QSpacerItem *vspacer = new QSpacerItem(10,20, QSizePolicy::Expanding, QSizePolicy::Fixed);
        vlay->addSpacerItem(vspacer);
        vlay->addLayout(butLay);
        errorShow->setLayout(vlay);
        ok->setFocus();
        if (i < lines.size() &&lines[i].replace("\r", "") == "ERROR : send recipe failed.")
        {
            if (lines[i+3].replace("\r", "") == "redstone.xmlrpc.XmlRpcFault: Identifiant ou mot de passe incorrect.")
            {
                mainError->setText(tr("L'envoi a échoué : identifiant ou mot de passe incorrect !"));
            }
            else if (lines[i+3].replace("\r", "") == "redstone.xmlrpc.XmlRpcException: A network error occurred.")
            {
                mainError->setText(tr("L'envoi a échoué : impossible de se connecter au site web. Vérifiez votre connexion internet."));
            }
            else if (lines[i+3].replace("\r", "") == "redstone.xmlrpc.XmlRpcException: The response could not be parsed.")
            {
                mainError->setText(tr("L'envoi a échoué : l'adresse de publication est-elle correcte ?"));
            }
            else if(lines[i+3].replace("\r", "").contains(QRegExp("redstone.xmlrpc.XmlRpcFault: D.+sol.+, vous n.+avez pas l.+autorisation de modifier cet article."))) {
                mainError->setText(tr("L'envoi a échoué : vous n'avez pas l'autorisation de modifier cette recette !"));
            }

            else
            {
                mainError->setText(tr("L'envoi a échoué. Cliquez sur Détails pour en savoir plus."));
            }
            errorShow->setWindowTitle(tr("L'envoi a échoué"));
            errorSize = errorShow->size();
            errorShow->exec();
        }
        else
        {
            mainError->setText(tr("L'envoi a échoué pour une raison inconnue. Cliquez sur Détails pour en savoir plus."));
            errorShow->setWindowTitle(tr("L'envoi a échoué"));
            errorSize = errorShow->size();
            errorShow->exec();
        }
    }
}

void SendWordpress::errorDetails_clicked()
{
    if (!isErrorDetailsOpened)
    {
        showError = new QPlainTextEdit(resultSend);
        showError->setReadOnly(true);
        showError->setMinimumSize(600,200);
        verrorShowContent->addWidget(showError);
        isErrorDetailsOpened = true;
        details->setText(tr("<< Détails"));
    }
    else
    {
        delete showError;
        errorShow->setMaximumSize(errorSize);
        isErrorDetailsOpened = false;
        details->setText(tr("Détails >>"));
    }
}

void SendWordpress::errorOk_clicked()
{
    errorShow->close();
}
