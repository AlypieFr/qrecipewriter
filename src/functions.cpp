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

#include "functions.h"

extern QString userDir;
extern QString confDir;
extern QString confFile;
extern QFile *confCatFile;

extern QStringList otherPicts;
extern QString dirPict;
extern QString dirTmp;
extern QString cmdNav;
extern QString dirSav;
extern QString pseudoWp;
extern QString dirDistPict;
extern QString addrSite;
extern QString addrPub;
extern QString systExp;
extern QString editPict;
extern QString corrOrtho;
extern bool richSnippets;
extern bool recPrinter;
extern bool recSearch;
extern bool recCoupDeCoeur;
extern bool sendAuto;
extern int configActive;
extern bool openLastDir_sauvegarde;
extern bool openLastDir_Img;
extern bool checkF7beforeSend;
extern bool autoCheckUpdt;

extern QMap<QString, QString> liens;

/**
 * @brief Functions::Functions
 * Constructor
 */
Functions::Functions()
{
}

/**
 * @brief Functions::loadConfig
 * Load config file
 */
void Functions::loadConfig()
{
    //A verifier chemin du fichier quand il sera placé

    QFile* file = new QFile(confFile);

    //A modifier pour prendre en compte le premier lancement du logiciel
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(NULL,
                              "QXSRExample::parseXML",
                              "Couldn't open .config",
                              QMessageBox::Ok);
        return;
    }
    QXmlStreamReader xml(file);
    /*Parse more than once the file in case the order is modified*/
    while(!xml.atEnd() &&
            !xml.hasError()) {

        QXmlStreamReader::TokenType token = xml.readNext();
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        if(token == QXmlStreamReader::StartElement) {
            if(xml.name() == "dirPict") {
                xml.readNext();
                dirPict=xml.text().toString();
                continue;
            }
            if(xml.name() == "dirSav") {
                xml.readNext();
                dirSav=xml.text().toString();
                continue;
            }
            if(xml.name() == "dirTmp") {
                xml.readNext();
                dirTmp=xml.text().toString();
                continue;
            }
            if(xml.name() == "cmdNav") {
                xml.readNext();
                cmdNav = xml.text().toString();
                continue;
            }
            if(xml.name() == "pseudoWp") {
                xml.readNext();
                pseudoWp = xml.text().toString();
                continue;
            }
            if(xml.name() == "corrOrtho") {
                xml.readNext();
                corrOrtho = xml.text().toString();
                continue;
            }
            if(xml.name() == "systExp") {
                xml.readNext();
                systExp = xml.text().toString();
                continue;
            }
            if(xml.name() == "editPict") {
                xml.readNext();
                editPict = xml.text().toString();
                continue;
            }
            if(xml.name() == "sendAuto") {
                xml.readNext();
                sendAuto = xml.text().toString() == "1";
                continue;
            }
            if (xml.name() == "activeServerConfig") {
                xml.readNext();
                configActive = xml.text().toString().toInt();
                continue;
            }
            if (xml.name() == "openLastDir_Sauvegarde") {
                xml.readNext();
                openLastDir_sauvegarde = xml.text().toString() == "1";
                continue;
            }
            if (xml.name() == "openLastDir_img") {
                xml.readNext();
                openLastDir_Img = xml.text().toString() == "1";
                continue;
            }
            if (xml.name() == "autoCheckUpdt") {
                xml.readNext();
                autoCheckUpdt = xml.text().toString() == "1";
                continue;
            }
            if (xml.name() == "checkF7beforeSend") {
                xml.readNext();
                checkF7beforeSend = xml.text().toString() == "1";
                continue;
            }
        }
    }

    file = new QFile(confFile + ".serv" + QString::number(configActive));
    if (file->exists()) {
        file->open(QIODevice::ReadOnly);
        QXmlStreamReader xmlS(file);
        while(!xmlS.atEnd() &&
                !xmlS.hasError()) {

            QXmlStreamReader::TokenType token = xmlS.readNext();
            if(token == QXmlStreamReader::StartDocument) {
                continue;
            }
            if(token == QXmlStreamReader::StartElement) {
                if(xmlS.name() == "addrSite") {
                    xmlS.readNext();
                    addrSite = xmlS.text().toString();
                    continue;
                }
                if(xmlS.name() == "addrPub") {
                    xmlS.readNext();
                    addrPub = xmlS.text().toString();
                    continue;
                }
                if(xmlS.name() == "dirDistPict") {
                    xmlS.readNext();
                    dirDistPict = xmlS.text().toString();
                    continue;
                }
                if(xmlS.name() == "richSnippets") {
                    xmlS.readNext();
                    richSnippets = xmlS.text().toString() == "1" ? true: false;
                    continue;
                }
                if(xmlS.name() == "recPrinter") {
                    xmlS.readNext();
                    recPrinter = xmlS.text().toString() == "1" ? true: false;
                    continue;
                }
                if(xmlS.name() == "recSearch") {
                    xmlS.readNext();
                    recSearch = xmlS.text().toString() == "1" ? true: false;
                    continue;
                }
                if(xmlS.name() == "recCoupDeCoeur") {
                    xmlS.readNext();
                    recCoupDeCoeur = xmlS.text().toString() == "1" ? true: false;
                    continue;
                }
            }
        }
    }
}

bool Functions::saveRecipeToFile(QString title, QStringList categories, QString tpsPrep, QString tpsCuis, QString tpsRep,
                                 QString nbPers, QString precision, QString description, QStringList ingredients,
                                 QStringList materiel, QStringList preparation, QStringList conseils, QString picture,
                                 QMap<QString, QString> liens, QFile *file, QString coupDeCoeur, int idRecipe) {
    file->open(QIODevice::WriteOnly);
    QStringList liensList;
    foreach (QString key, liens.keys()) {
        qDebug() << key;
        liensList.append(key + "|" + liens[key]);
    }
    QMap<QString, QVariant> data;
    data.insert("title", title);
    data.insert("categories", categories);
    data.insert("tpsPrep", tpsPrep);
    data.insert("tpsCuis", tpsCuis);
    data.insert("tpsRep", tpsRep);
    data.insert("nbPers", nbPers);
    data.insert("precision", precision);
    data.insert("description", description);
    data.insert("ingredients", ingredients);
    data.insert("materiel", materiel);
    data.insert("preparation", preparation);
    data.insert("conseils", conseils);
    data.insert("picture", picture);
    data.insert("liens", liensList);
    data.insert("coupDeCoeur", coupDeCoeur);
    data.insert("idRecipe", idRecipe);

    QDataStream out(file);
    out << data;

    file->close();
    return true;
}

/**
 * @brief Functions::saveRecipe
 * @param title : titre
 * @param categories : list of categories
 * @param tpsPrep : temps de préparation
 * @param tpsCuis : temps de cuisson
 * @param tpsRep : temps de repos
 * @param nbPers : nombres de personnes
 * @param precision : précision
 * @param description
 * @param ingredients
 * @param materiel
 * @param preparation
 * @param conseils
 * @param picture
 * @param liens
 * @param filename : file into the recipe will be saved
 * @return true if save success
 * Do save of recipe on the file
 */
bool Functions::saveRecipe(QString title, QStringList categories, QString tpsPrep,
                           QString tpsCuis, QString tpsRep, QString nbPers, QString precision,
                           QString description, QStringList ingredients, QStringList materiel,
                           QStringList preparation, QStringList conseils, QString picture, QMap<QString, QString> liens, QString filename, QString coupDeCoeur, int idRecipe)
{
    QFile* file = new QFile(filename + ".rct");
    file->remove(".");
    return saveRecipeToFile(title, categories, tpsPrep, tpsCuis, tpsRep, nbPers, precision, description, ingredients, materiel, preparation, conseils, picture, liens, file, coupDeCoeur, idRecipe);
}

QString Functions::removeSpecialChars(QString str) {
    return str.replace("&#8239;", " ").replace(QRegExp("<span[^>]*>"), "").replace("</span>", "");
}

QStringList Functions::addCommentLines(QString txt) {
    QStringList res = txt.split(QRegExp("<br\\s?/?>"));
    QRegExp notEmpty = QRegExp("\\S+");
    QRegExp notOnlyTags = QRegExp(">[^<]+<");
    bool finished = false;
    int i = 0;
    while (!finished) {
        bool removeLine = false;
        if (res[i].contains(notEmpty)) {
            QString test = res[i];
            while(test.startsWith(" ")) {
                test = test.mid(1);
            }
            while(test.endsWith(" ")) {
                test = test.left(test.length() - 1);
            }
            if (test.startsWith("<") && test.endsWith(">") && !test.contains(notOnlyTags)) {
                removeLine = true;
            }
            else {
                res[i] = "comm|" + res[i];
                i++;
            }
        }
        else {
            removeLine = true;
        }
        if (removeLine){
            res.removeAt(i);
        }
        if (i >= res.count()) {
            finished = true;
        }
    }
    return res;
}

QStringList Functions::makeSimpleList(QString text) {
    QStringList list;
    QString next = text;
    int nextUl = -1, nextFUl = -1, nextLi = -1;
    while (next != "") {
        nextUl = next.indexOf("<ul>");
        nextFUl = next.indexOf("</ul>");
        nextLi = next.indexOf("<li>");
        if (nextUl == -1) {
            nextUl = 1000000;
        }
        if (nextFUl == -1) {
            nextFUl = 1000000;
        }
        if (nextLi == -1) {
            nextLi = 1000000;
        }
        int min1 = qMin(nextUl, nextFUl);
        int min = qMin(min1,nextLi);
        if (min < 1000000) {
            if (min == nextLi) {
                next = next.mid(nextLi + 4);
                int endLi = next.indexOf("</li>");
                list.append("0|" + next.left(endLi));
                next = next.mid(endLi + 5);
            }
            else if (min == nextUl) {
                if (nextUl > 0) {
                    list.append(addCommentLines(next.left(nextUl)));
                }
                next = next.mid(nextUl + 4);
            }
            else if(min == nextFUl) {
                next = next.mid(nextFUl + 5);
            }
        }
        else {
            list.append(addCommentLines(next));
            next = "";
        }
    }
    return list;
}

QStringList Functions::makeSimpleListWithSubLists(QString text) {
    QStringList list;
    int niv = 0;
    QString next = text;
    int nextUl = -1, nextFUl = -1, nextLi = -1;
    while (next != "") {
        nextUl = next.indexOf("<ul>");
        nextFUl = next.indexOf("</ul>");
        nextLi = next.indexOf("<li>");
        if (nextUl == -1) {
            nextUl = 1000000;
        }
        if (nextFUl == -1) {
            nextFUl = 1000000;
        }
        if (nextLi == -1) {
            nextLi = 1000000;
        }
        int min1 = qMin(nextUl, nextFUl);
        int min = qMin(min1,nextLi);
        if (min < 1000000) {
            if (min == nextUl) {
                if (nextUl > 0) {
                    list.append(addCommentLines(next.left(nextUl)));
                }
                niv++;
                next = next.mid(nextUl + 4);
            }
            else if(min == nextFUl) {
                niv--;
                next = next.mid(nextFUl + 5);
            }
            else if(min == nextLi) {
                next = next.mid(nextLi + 4);
                int endLi = next.indexOf("</li>");
                list.append(QString::number(niv) + "|" + removeSpecialChars(next.left(endLi)));
                next = next.mid(endLi + 5);
            }
        }
        else {
            list.append(addCommentLines(next));
            next = "";
        }
    }
    return list;
}

QStringList Functions::makeNumberedList(QString text) {
    QStringList list;
    QString bal = "0.0";
    QString next = text;
    int nextOl = -1, nextFOl = -1, nextLi = -1;
    while (next != "") {
        nextOl = next.indexOf("<ol>");
        nextFOl = next.indexOf("</ol>");
        nextLi = next.indexOf("<li>");
        if (nextOl == -1) {
            nextOl = 1000000;
        }
        if (nextFOl == -1) {
            nextFOl = 1000000;
        }
        if (nextLi == -1) {
            nextLi = 1000000;
        }
        int min1 = qMin(nextOl, nextFOl);
        int min = qMin(min1,nextLi);
        if (min < 1000000) {
            if (nextOl == min) {
                if (nextOl > 0) {
                    list.append(addCommentLines(next.left(nextOl)));
                    bal = "0";
                }
                bal += ".0";
                next = next.mid(nextOl + 4);
            }
            else if(nextFOl == min) {
                QStringList balParts = bal.split(".");
                balParts.removeAt(qMax(balParts.length() - 2, 0)); //balParts.length()-2 always > 0, but qMax suppress the warning in compilation
                bal = balParts.join(".");
                next = next.mid(nextFOl + 5);
            }
            else if(nextLi == min) {
                QStringList balParts = bal.split(".");
                balParts[balParts.length() - 2] = QString::number(balParts[balParts.length() - 2].toInt() + 1);
                bal = balParts.join(".");
                next = next.mid(nextLi + 4);
                int endLi = next.indexOf("</li>");
                list.append(bal + "|" + next.left(endLi));
                next = next.mid(endLi + 5);
            }
        }
        else {
            list.append(addCommentLines(next));
            next = "";
        }
    }
    return list;
}

bool Functions::saveRecipeFromDist(QString title, QStringList categories, QString content, QString picture,
                                   QString coupDeCoeur, QFile *file, int idRecipe) {
    for (int i = 0; i < categories.count(); ++i) {
        categories[i] = categories[i].replace("&amp;", "&&");
    }
    content = content.replace("\n", "").replace("\r", "");
    QString description = "", tpsPrep = "0h0", tpsCuis = "0h0", tpsRep = "0j0h0", nbPers, precision;
    QStringList  ingredients, materiel, preparation, conseils;
    QMap<QString, QString> liens;
    int nbLien = 1;

    //Get description:
    QRegExp lienExp = QRegExp("<a [^>]*href=[\"']([^\"']+)[\"'][^>]*>([^<]+)</a>");
    QRegExp descExp = QRegExp("<description>(.+)</description>");
    if (content.contains(descExp)) {
        description = descExp.cap(1);
        description = description.replace("<p>", "").replace("</p>", "\n");
        description = description.replace(QRegExp("<br\\s?/?>"), "\n");
        while (description.contains(lienExp)) {
            QString balise = "L" + QString::number(nbLien);
            liens[balise] = lienExp.cap(1);
            description = description.replace(lienExp.cap(0), "<" + balise + ">" + lienExp.cap(2) + "</" + balise + ">");
            nbLien++;
        }
        while(description.right(1) == "\n") {
            description = description.left(description.length() - 1);
        }
    }

    //Get ingredients and materiel:
    QRegExp ingrExp = QRegExp("<ingredients>(.+)</ingredients>");
    if (content.contains(ingrExp)) {
        QString txt = removeSpecialChars(ingrExp.cap(1));
        while (txt.contains(lienExp)) {
            QString balise = "L" + QString::number(nbLien);
            liens[balise] = lienExp.cap(1);
            txt = txt.replace(lienExp.cap(0), "<" + balise + ">" + lienExp.cap(2) + "</" + balise + ">");
            nbLien++;
        }
        QRegExp header = QRegExp(QObject::tr("Ingrédients") + " \\(" + QObject::tr("pour") + " (\\d+) "+ QObject::tr("personnes") + "?([^:]+):");
        QRegExp header2 = QRegExp(QObject::tr("Ingrédients") + " \\(" + QObject::tr("pour") + " (\\d+ " + QObject::tr("à") + " \\d+) " + QObject::tr("personnes") + "?([^:]+):");
        QRegExp header3 = QRegExp(QObject::tr("Ingrédients") + " \\(" + QObject::tr("pour") + " (\\d+-\\d+) " + QObject::tr("personnes") + "?([^:]+):");
        QString txtPrecision;
        bool isHeader = false;
        int endHeader = -1;
        if (txt.contains(header)) {
            nbPers = header.cap(1);
            endHeader = txt.indexOf(header.cap(0)) + header.cap(0).length();
            txtPrecision = header.cap(2);
            isHeader = true;
        }
        else if (txt.contains(header2)) {
            nbPers = header2.cap(1);
            endHeader = txt.indexOf(header2.cap(0)) + header2.cap(0).length();
            txtPrecision = header2.cap(2);
            isHeader = true;
        }
        if (txt.contains(header3)) {
            nbPers = header3.cap(1).replace("-", " à ");
            endHeader = txt.indexOf(header3.cap(0)) + header3.cap(0).length();
            txtPrecision = header3.cap(2);
            isHeader = true;
        }
        if (isHeader) {
            QRegExp precis = QRegExp("\\(([^\\)]+)\\)");
            if (txtPrecision.contains(precis)) {
                precision = precis.cap(1);
            }
        }
        int begin = endHeader;
        if (begin > -1) {
            int isMat = txt.indexOf("<p><b>" + QObject::tr("Matériel nécessaire"));
            if (isMat == -1) {
                isMat = txt.indexOf(QObject::tr("Matériel nécessaire"));
            }
            if (isMat > -1) {
                ingredients = makeSimpleListWithSubLists(txt.mid(begin, isMat - begin));
            }
            else {
                ingredients = makeSimpleListWithSubLists(txt.mid(begin));
            }
        }
        //Get materiel:
        QRegExp matExp = QRegExp(QObject::tr("Matériel nécessaire") + "(.+)");
        if (txt.contains(matExp)) {
            QString mat = matExp.cap(1);
            int begin = mat.indexOf("<ul>");
            if (begin > -1) {
                materiel = makeSimpleList(mat.mid(begin));
            }
        }
    }

    //Get preparation :
    QRegExp prepExp = QRegExp("<preparation>(.+)</preparation>");
    if (content.contains(prepExp)) {
        QString txt = removeSpecialChars(prepExp.cap(1));
        while (txt.contains(lienExp)) {
            QString balise = "L" + QString::number(nbLien);
            liens[balise] = lienExp.cap(1);
            txt = txt.replace(lienExp.cap(0), "<" + balise + ">" + lienExp.cap(2) + "</" + balise + ">");
            nbLien++;
        }
        txt.replace("<ul>", "<ol>").replace("</ul>", "</ol>").replace("<ol class=\"lbold\">", "<ol>").replace("<ol class=lbold>", "<ol>");
        int begin = -1;
        QRegExp header = QRegExp(QObject::tr("Pr[ée]paration[^:]*:"), Qt::CaseInsensitive);
        if (txt.contains(header)) {
            begin = txt.indexOf(header.cap(0)) + header.cap(0).length();
        }
        if (begin > -1) {
            preparation = makeNumberedList(txt.mid(begin));
        }
    }

    //Get conseils:
    QRegExp consExp = QRegExp("<conseils>(.+)</conseils>");
    if (content.contains(consExp)) {
        QString txt = removeSpecialChars(consExp.cap(1));
        while (txt.contains(lienExp)) {
            QString balise = "L" + QString::number(nbLien);
            liens[balise] = lienExp.cap(1);
            txt = txt.replace(lienExp.cap(0), "<" + balise + ">" + lienExp.cap(2) + "</" + balise + ">");
            nbLien++;
        }
        int begin = -1;
        QRegExp header = QRegExp(QObject::tr("Conseils") + "[^:]*:", Qt::CaseInsensitive);
        if (txt.contains(header)) {
            begin = txt.indexOf(header.cap(0)) + header.cap(0).length();
        }
        if (begin > -1) {
            conseils = makeSimpleList(txt.mid(begin));
        }
    }

    //Get times:
    QRegExp  timesExp = QRegExp("<temps>(.+)</temps>");
    QRegExp minExp = QRegExp("(\\d+)\\s?min");
    QRegExp hExp = QRegExp("(\\d+)\\s?h");
    QRegExp jExp = QRegExp(QObject::tr("(\\d+)\\s?j"));
    if (content.contains(timesExp)) {
        QString txt = timesExp.cap(1);
        QRegExp  prepTime = QRegExp(QObject::tr("Temps de Préparation") + "[^:]*:\\s?([^<]+)", Qt::CaseInsensitive);
        if (txt.contains(prepTime)) {
            QString pTime = prepTime.cap(1);
            QString h = "0", min = "0";
            if (pTime.contains(minExp)) {
                min = minExp.cap(1);
            }
            if (pTime.contains(hExp)) {
                h = hExp.cap(1);
            }
            tpsPrep = h + "h" + min;
        }
        else {
            tpsPrep = "0h0";
        }
        QRegExp cuisTime = QRegExp(QObject::tr("Temps de Cuisson") + "[^:]*:\\s?([^<]+)", Qt::CaseInsensitive);
        if (txt.contains(cuisTime)) {
            QString cTime = cuisTime.cap(1);
            QString h = "0", min = "0";
            if (cTime.contains(minExp)) {
                min = minExp.cap(1);
            }
            if (cTime.contains(hExp)) {
                h = hExp.cap(1);
            }
            tpsCuis = h + "h" + min;
        }
        else {
            tpsCuis = "0h0";
        }
        QRegExp repTime = QRegExp(QObject::tr("Temps de Repos") + "[^:]*:\\s?([^<]+)", Qt::CaseInsensitive);
        if (txt.contains(repTime)) {
            QString rTime = repTime.cap(1);
            QString j = "0", h = "0", min = "0";
            if (rTime.contains(minExp)) {
                min = minExp.cap(1);
            }
            if (rTime.contains(hExp)) {
                h = hExp.cap(1);
            }
            if (rTime.contains(jExp)) {
                j = jExp.cap(1);
            }
            tpsRep = j + "j" + h + "h" + min;
        }
        else {
            tpsRep = "0j0h0";
        }
    }
    else {
        tpsPrep = "0h0";
        tpsCuis = "0h0";
        tpsRep = "0j0h0";
    }

    return saveRecipeToFile(title, categories, tpsPrep, tpsCuis, tpsRep, nbPers, precision, description, ingredients, materiel, preparation, conseils, picture, liens, file, coupDeCoeur, idRecipe);
}

/**
 * @brief Functions::loadRecipe
 * @param fileName : file to be opened
 * @return items of the recipe in a QMap
 * Open the file given into filename
 */
QMap<QString, QStringList> Functions::loadRecipe(QString fileName)
{
    //Creating Map:
    QMap<QString, QStringList> result;

    QMap<QString, QVariant> map;

    QFile* file = new QFile(fileName);

    QDataStream in(file);

    if (!file->open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not read the file:" << fileName << "Error string:" << file->errorString();
        return result;
    }

    in >> map;

    result["titre"] = QStringList(map["title"].toString());
    result["categories"] = map["categories"].toStringList();
    result["image"] = QStringList(map["picture"].toString());
    result["tpsPrep"] = map["tpsPrep"].toString().split("h");
    result["tpsCuis"] = map["tpsCuis"].toString().split("h");
    result["tpsRep"] = map["tpsRep"].toString().split(QRegExp("[jh]"));
    result["nbPers"] = QStringList(map["nbPers"].toString());
    result["precision"] = QStringList(map["precision"].toString());
    result["description"] = QStringList(map["description"].toString());
    result["ingredients"] = map["ingredients"].toStringList();
    result["materiel"] = map["materiel"].toStringList();
    result["preparation"] = map["preparation"].toStringList();
    result["conseils"] = map["conseils"].toStringList();
    result["coupDeCoeur"] = QStringList(map["coupDeCoeur"].toString());
    result["idRecipe"] = QStringList(map["idRecipe"].toString());
    result["liens"] = map["liens"].toStringList();

    return result;
}

/**
 * @brief Functions::loadCategories
 * @return list of categories
 * Load categories from the cat config file
 */
QStringList Functions::loadCategories()
{
    //Loading xml file:
    XMLDocument doc;
    doc.LoadFile(confCatFile->fileName().toStdString().c_str());

    //Load categories:
    QStringList categories;
    XMLElement* elem = doc.FirstChildElement("categories");
    if (elem)
    {
        for( XMLNode* ele = elem->FirstChild();
                 ele;
                 ele = ele->NextSibling() )
            {
            categories.append(ele->ToElement()->GetText());
        }
    }
    return categories;
}

/**
 * @brief Functions::getSimpleListWithSubLists
 * @param items : elements of the list
 * @return htmlCode corresponding to the list
 * Transform a list into HtmlCode, in the case of simple list (without numbers), that can include sublists (Ingrédents)
 */
QString Functions::getSimpleListWithSubLists(QStringList items)
{
    QString htmlCode = "";
    int nivList = -1;
    foreach (QString item, items) {
        QStringList parts = item.split("|");
        if (parts[0] == "comm")
        {
            for (int var = -1; var < nivList; ++var) {
                htmlCode.append("</ul>\n");
            }
            htmlCode.append(insertLinks(insertPictures(parts[1])) + "<br/>\n");
            nivList = -1;
        }
        else
        {
            int puce = parts[0].toInt();
            if (puce == nivList)
            {
                htmlCode.append("<li>" + insertLinks(insertPictures(parts[1])) + "</li>\n");
                nivList = puce;
            }
            else if (puce > nivList)
            {
                for (int var = nivList; var < puce; ++var) {
                    htmlCode.append("<ul>\n");
                }
                htmlCode.append("<li>" + insertLinks(insertPictures(parts[1])) + "</li>\n");
                nivList = puce;
            }
            else if (puce < nivList)
            {
                for (int var = nivList; var > puce; --var) {
                    htmlCode.append("</ul>\n");
                }
                htmlCode.append("<li>" + insertLinks(insertPictures(parts[1])) + "</li>\n");
                nivList = puce;
            }
        }
    }
    if (nivList > -1)
    {
        for (int var = nivList; var > -1; --var) {
            htmlCode.append("</ul>\n");
        }
    }
    return htmlCode;
}

/**
 * @brief Functions::getSimpleListWithSubLists
 * @param items : elements of the list
 * @return htmlCode corresponding to the list
 * Transform a list into HtmlCode, in the case of simple list (without numbers), that can't include sublists (Matériel, Conseils)
 */
QString Functions::getSimpleList(QStringList items)
{
    QString htmlCode = "";
    bool inList = false;
    foreach (QString item, items) {
        QStringList parts = item.split("|");
        if (parts[0] == "comm")
        {
            if (inList)
            {
                htmlCode.append("</ul>\n" + insertLinks(insertPictures(parts[1])) + "<br/>\n");
                inList = false;
            }
            else
            {
                htmlCode.append(insertLinks(insertPictures(parts[1])) + "<br/>\n");
            }
        }
        else {
            if (inList)
            {
                htmlCode.append("<li>" + insertLinks(insertPictures(parts[1])) + "</li>\n");
            }
            else
            {
                htmlCode.append("<ul>\n<li>" + insertLinks(insertPictures(parts[1])) + "</li>\n");
                inList = true;
            }
        }
    }
    if (inList)
        htmlCode += "</ul>\n";
    return htmlCode;
}

/**
 * @brief Functions::getSimpleListWithSubLists
 * @param items : elements of the list
 * @return htmlCode corresponding to the list
 * Transform a list into HtmlCode, in the case of numbered lists
 */
QString Functions::getNumberedList(QStringList items)
{
    QString htmlCode = "";
    int nivList = -1;
    foreach (QString item, items) {
        QStringList parts = item.split("|");
        if (parts[0] == "comm")
        {
            for (int var = 0; var < nivList; ++var) {
                htmlCode.append("</span></ul>\n");
            }
            htmlCode.append("</ol>\n");
            htmlCode.append(insertLinks(insertPictures(parts[1])) + "<br/>\n");
            nivList = -1;
        }
        else
        {
            int puce = parts[0].split(".").size() - 2;
            if (puce == nivList)
            {
                if (puce == 0)
                {
                    htmlCode.append("<li><span>" + insertLinks(insertPictures(parts[1])) + "</span></li>\n");
                }
                else
                {
                    htmlCode.append("<li>" + insertLinks(insertPictures(parts[1])) + "</li>\n");
                }
                nivList = puce;
            }
            else if (puce > nivList)
            {
                if (nivList == -1)
                {
                    htmlCode.append("<ol class=\"lbold\">\n");
                    nivList = 0;
                }
                for (int var = nivList; var < puce; ++var) {
                    htmlCode.append("<ul><span style=\"font-weight:normal; color:black\">\n");
                }
                if (puce == 0)
                {
                    htmlCode.append("<li><span>" + insertLinks(insertPictures(parts[1])) + "</span></li>\n");
                }
                else
                {
                    htmlCode.append("<li>" + insertLinks(insertPictures(parts[1])) + "</li>\n");
                }
                nivList = puce;
            }
            else if (puce < nivList)
            {
                for (int var = nivList; var > puce; --var) {
                    htmlCode.append("</span></ul>\n");
                }
                if (puce == 0)
                {
                    htmlCode.append("<li><span>" + insertLinks(insertPictures(parts[1])) + "</span></li>\n");
                }
                else
                {
                    htmlCode.append("<li>" + insertLinks(insertPictures(parts[1])) + "</li>\n");
                }
                nivList = puce;
            }
        }
    }
    if (nivList > -1)
    {
        for (int var = nivList; var > 0; --var) {
            htmlCode.append("</span></ul>\n");
        }
        htmlCode.append("</ol>\n");
    }
    return htmlCode;
}

/**
 * @brief Functions::insertLinks
 * @param data : data containing the link
 * @return htmlCode of the link
 * Transform link with tags links to HTML tags links
 */
QString Functions::insertLinks(QString data)
{
    QRegExp exp ("<L\\d+>.+</L\\d+>");
    while (data.contains(exp))
    {
        QString lien =  exp.cap();
        QRegExp droit ("<L\\d+>");
        if (lien.contains(droit))
        {
            QString balise = droit.cap();
            balise = balise.mid(1, balise.length() - 2);
            if (liens.contains(balise))
            {
                data.replace("<" + balise + ">", "<a href=\"" + liens[balise] + "\" target=\"blank\">");
                data.replace("</" + balise + ">", "</a>");
            }
            else {
                QMessageBox::critical(NULL, QObject::tr("Lien introuvable"), QObject::tr("Le lien") + " " + balise + QObject::tr(" est introuvable. Veuillez corriger la balise. Si ceci est un bug, merci de nous le rapporter."));
                break;
            }
        }
    }
    return data;
}

/**
 * @brief Functions::insertPictures
 * @param data
 * @return htmlCode of the picture
 * Transform picture tags to HTML pictures tags, and add corresponding pictures to otherPictures list
 */
QString Functions::insertPictures(QString data)
{
    QRegExp exp ("\\[IMG:(\\w+):(\\d+):(\\d+):([^\\]]+)\\]");
    while (data.contains(exp))
    {
        QString imgBal = exp.cap(0);
        QString className = exp.cap(1);
        QString largeur = exp.cap(2);
        QString hauteur = exp.cap(3);
        QString img = exp.cap(4);
        bool center = false;
        if (img.startsWith("center:")) {
            img = img.right(img.length() - 7);
            center = true;
        }
        QString classDef = "";
        if (className != "all") {
            classDef = " class=\"" + className + "\"";
        }
        QString centerBegin = center ? "<center>" : "";
        QString centerEnd = center ? "</center>" : "";
        data.replace(imgBal, centerBegin + "<img src=\"" + dirDistPict + img.split("/").last() + "\" alt=\"" + QObject::tr("Image d'illustration") + "\" width=\"" + largeur + "\" height=\"" + hauteur + "\"" + classDef + " />" + centerEnd + "<br/>");
        if (!otherPicts.contains(img))
            otherPicts.append(img);
    }
    return data;
}

QString Functions::insertMovies(QString data)
{
    QRegExp exp ("\\[VIDEO:(\\d+):(\\d+):(\\d+):(\\d+):([01]+):([01]+):([01]+):([01]+):([01]+):([\\w-]+)\\]");
    while (data.contains(exp)) {
        QString balise = exp.cap(0);
        QString largeur = exp.cap(1);
        QString hauteur = exp.cap(2);
        QString startTime = exp.cap(3);
        QString bordure = exp.cap(4);
        bool fullscreen = (exp.cap(5) == "1" ? true : false);
        bool suggestions = (exp.cap(6) == "1" ? true : false);
        bool commandes = (exp.cap(7) == "1" ? true : false);
        bool titreVideo = (exp.cap(8) == "1" ? true : false);
        bool confidentialite = (exp.cap(9) == "1" ? true : false);
        QString idVideo = exp.cap(10);

        QStringList params;
        if (!suggestions) {
            params.append("rel=0");
        }
        if (!commandes) {
            params.append("controls=0");
        }
        if (!titreVideo) {
            params.append("showinfo=0");
        }
        QString url = "https://www.youtube";
        if (confidentialite) {
            url += "-nocookie";
        }
        url += ".com/embed/" + idVideo;
        params.append("start=" + startTime);
        if (params.size() > 0) {
            QString stringParams = params.join("&amp;");
            url += "?";
            url += stringParams;
        }

        QString html = "<iframe class=\"noprint\" width=\"" + largeur + "\" height=\"" + hauteur + "\" src=\"" + url + "\" frameborder=\"" + bordure + "\"";
        if (fullscreen) {
            html += " allowfullscreen";
        }
        html +="></iframe>";
        data.replace(balise, html);
    }
    return data;
}

/**
 * @brief Functions::makeTimes
 * @param hPrep
 * @param minPrep
 * @param hCuis
 * @param minCuis
 * @param jRep
 * @param hRep
 * @param minRep
 * @return HTML code of the times :
 *      First element : temps de préparation
 *      Second element : temps de cuisson
 *      Third element : temps de repos
 * Transform each time to HTML code
 */
QStringList Functions::makeTimes(int hPrep, int minPrep, int hCuis, int minCuis, int jRep, int hRep, int minRep)
{
    QString tpsPrep = "";
    if(hPrep != 0)
        tpsPrep = QString::number(hPrep) + " h ";
    if(minPrep != 0)
        tpsPrep = tpsPrep + QString::number(minPrep) + " min";
    QString tpsRep = "";
    if(jRep != 0)
        tpsRep = QString::number(jRep) + " j ";
    if(hRep != 0)
        tpsRep = tpsRep + QString::number(hRep) + " h ";
    if(minRep !=0)
        tpsRep = tpsRep + QString::number(minRep) + " min";
    QString tpsCuis = "";
    if(hCuis != 0)
        tpsCuis = QString::number(hCuis) + " h ";
    if(minCuis != 0)
        tpsCuis = tpsCuis + QString::number(minCuis) + " min";
    QStringList res;
    res << tpsPrep << tpsCuis << tpsRep;
    return res;
}

/**
 * @brief Replace user tags NP and OP with html span tags
 * @param text with user tags
 * @return text with html span tags
 */
QString Functions::setPrintTags(QString text)
{
    text = text.replace("<np>", "<span class=\"noprint\">");
    text = text.replace("<po>", "<span class=\"printonly\">");
    text = text.replace("</np>", "</span>");
    text = text.replace("</po>", "</span>");
    return text;
}

QString Functions::makeRichSnippets(QString title, QString mainPicture, int hPrep, int minPrep, int hCuis, int minCuis, int jRep, int hRep, int minRep,
                                    int nbPersonnes, QString precision, QString description, QStringList ingredients, QStringList preparation)
{
    QString photo = addrSite;
    if (!photo.endsWith("/")) {
        photo += "/";
    }
    photo += dirDistPict;

    QRegExp exp ("<a href=\"[^\"]+\" target=\"[^\"]+\">");
    description.replace(exp, "");
    description.replace("</a>", "");
    description.replace("\"", "\\\"");
    description.replace(";-)", "");
    description.replace(":-)", "");
    description.replace(":-(", "");
    description.replace(";-(", "");
    description.replace(";-D", "");
    description.replace(":-D", "");

    QString prepTime = hPrep == 0 ? "PT" : "PT" + QString::number(hPrep) + "H";
    prepTime += QString::number(minPrep) + "M";
    QString cookTime = hCuis == 0 ? "PT" : "PT" + QString::number(hCuis) + "H";
    cookTime += QString::number(minCuis) + "M";

    int minTotal = hPrep * 60 + minPrep + hCuis * 60 + minCuis + jRep * 24 * 60 + hRep * 60 + minRep;
    int hTotal = minTotal / 60;
    minTotal = minTotal - (60 * hTotal);
    QString totalTime = hTotal == 0 ? "PT" : "PT" + QString::number(hTotal) + "H";
    totalTime += QString::number(minTotal) + "M";

    QRegExp baliseB ("<[A-Za-z0-9]{1,3}>");
    QRegExp baliseE ("</[A-Za-z0-9]{1,3}>");

    description.replace(baliseB, "").replace(baliseE, "");

    QStringList ingrs;
    int i = 1;
    foreach (QString ingr, ingredients) {
        bool addItem = true;
        if (i < ingredients.size()) {
            int index = ingr.split("|")[0].toInt();
            if (ingredients[i].split("|")[0].toInt() > index) {
                addItem = false;
            }
        }
        if (addItem) {
            ingrs.append(ingr.split("|")[1].replace(baliseB, "").replace(baliseE, ""));
        }
        i++;
    }

    QStringList preps;
    foreach (QString prep, preparation) {
        preps.append(prep.replace("0|", " ").replace(baliseB, "").replace(baliseE, ""));
    }

    QString snippet = "<script type=\"application/ld+json\">\n\
        {\n\
              \"@context\": \"http://schema.org/\",\n\
              \"@type\": \"Recipe\",\n\
              \"name\": \"" + title.replace("\"", "\\\"") + "\",\n\
              \"image\": \"" + photo + mainPicture + "\",\n\
              \"author\": {\n\
                \"@type\":\"Person\",\n\
                \"name\":\"" + pseudoWp + "\"\n\
              },\n\
              \"datePublished\": \"" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + "\",\n\
              \"description\": \"" + description + "\",\n\
              \"prepTime\": \"" + prepTime + "\",\n\
              \"cookTime\": \"" + cookTime + "\",\n\
              \"totalTime\": \"" + totalTime + "\",\n\
              \"recipeYield\": \"pour " + QString::number(nbPersonnes) + " personnes";
    if (precision != "") {
        snippet += " (" + precision + ")";
    }
    snippet += "\",\n";
    snippet += "              \"ingredients\": \n              [  \"" + ingrs.join("\",\n                \"") + "\"\n              ],\n";
    snippet += "              \"recipeInstructions\": \"" + preps.join("\\n") + "\"\n        }\n</script>\n\n";
    return snippet;
}

/**
 * @brief Functions::generateHtmlCode
 * @param titre
 * @param mainPicture
 * @param hPrep
 * @param minPrep
 * @param hCuis
 * @param minCuis
 * @param jRep
 * @param hRep
 * @param minRep
 * @param nbPersonnes
 * @param precision
 * @param description
 * @param ingredients
 * @param materiel
 * @param preparation
 * @param conseils
 * @return HTML Code
 * Make htmlCode with given parameters of the recipe
 */
QString Functions::generateHtmlCode(QString titre, QString mainPicture, int hPrep, int minPrep, int hCuis, int minCuis, int jRep,
                                    int hRep, int minRep, int nbPersonnes, int nbPersonnes2, QString precision,
                                    QString description, QString ingredients, QString materiel, QString preparation,
                                    QString conseils)
{
    QString htmlCode = "";
    description = "<p>"+description.replace("\n", "<br/>\n")+"</p>";

    QStringList temps = makeTimes(hPrep, minPrep, hCuis, minCuis, jRep, hRep, minRep);

    QString tpsPrep = temps[0];
    QString tpsCuis = temps[1];
    QString tpsRep = temps[2];

    //Generation of the complete code :
    htmlCode = "<div id='masquer'><div><a href=\"" + dirDistPict + mainPicture + "\"><photo><img style=\"float: left; "
             + "margin-right: 6px;\" class=shadow alt=\"Image d'illustation de la recette\" title=\""
             + titre+"\" src=\"" + dirDistPict + mainPicture + "\" width=\"254\" height=\"190\"></photo></a></div><description>"
             + description+ "</description></div><div id=\"detail\"><temps><b>" + QObject::tr("Temps de Préparation :") + " " +tpsPrep;
    //Adding temps de repos if used:
    if (tpsRep != "")
    {
        htmlCode = htmlCode + "<br/>" + QObject::tr("Temps de Repos :") + " " + tpsRep;
    }
    //Adding temps de cuisson if used:
    if (tpsCuis != "")
    {
            htmlCode = htmlCode + "<br/>" + QObject::tr("Temps de Cuisson :") + " " + tpsCuis;
    }
    //Adding next code:
    htmlCode = htmlCode + "</b></temps></div><!--more--><br/><ingredients><p><b>" + QObject::tr("Ingrédients") + " (" + QObject::tr("pour") + " " + QString::number(nbPersonnes);
    //Adding word "personne(s)":
    if (nbPersonnes2 > 0  && nbPersonnes2 > nbPersonnes) {
        htmlCode = htmlCode + QObject::tr(" à ") + QString::number(nbPersonnes2) + QObject::tr(" personnes");
    }
    else if (nbPersonnes > 1)
        htmlCode = htmlCode + QObject::tr(" personnes");
    else
        htmlCode = htmlCode + QObject::tr(" personne");
    //Adding precision if used:
    if (precision != "")
    {
         htmlCode = htmlCode + " ("+precision+")";
    }
    //Ingredients:
    htmlCode = htmlCode + ") :</b></p>"+ingredients;
    //Materiel if used:
    if (materiel != "")
    {
        htmlCode = htmlCode + "<p><b>" + QObject::tr("Matériel nécessaire :") + "</b></p>"+materiel;
    }
    //Preparation:
    htmlCode = htmlCode + "</ingredients><preparation><p><b>" + QObject::tr("Préparation :") + "</b></p>"+preparation+"</preparation>";
    //Conseils if used:
    if (conseils != "")
    {
        htmlCode = htmlCode + "<conseils><p><b>" + QObject::tr("Conseils :") + "</b></p>"+conseils+"</conseils>";
    }
    //Adding "Version imprimable" balise, only if the website is Cool Cooking, because others might not use it:
    if (recPrinter) {
        htmlCode = htmlCode + "<br/>[VERSION_IMPRIMABLE]";
    }
    //Replace oe by "e dans l'o", because we speak French :
    htmlCode = htmlCode.replace("oe", "&oelig;");

    htmlCode = setPrintTags(htmlCode);

    htmlCode = insertMovies(htmlCode);

    //breaking spaces:
    htmlCode = htmlCode.replace(" :", "&#8239;:").replace(" !", "&#8239;!").replace(" ;", "&#8239;;").replace(" =", "&#8239;=");

    return htmlCode;
}

/**
 * @brief Functions::getSelectedCategories
 * @param categories : contains categories checkbox and corresponding names
 * @return list of selected categories
 * Get selected categories
 */
QStringList Functions::getSelectedCategories(QMap<QCheckBox *, QString> categories)
{
    QStringList res;
    foreach (QCheckBox* box, categories.keys()) {
        if (box->isChecked()) {
            res.append(categories[box].replace("&", "&amp;"));
        }
    }
    return res;
}

/**
 * @brief Functions::makeExcerpt
 * @param descWords : words that compose the description, without any tag
 * @param tpsPrep
 * @param tpsCuis
 * @param tpsRep
 * @return Html code for the excerpt
 * Generate html code for the excerpt
 */
QString Functions::makeExcerpt(QStringList descWords, QString tpsPrep, QString tpsCuis, QString tpsRep)
{
    QString descExpt = "";
    for(int i=0; i<qMin(descWords.length(),20); ++i)
        descExpt = descExpt + " " + descWords[i];
    QString expt = "<b>" + QObject::tr("Préparation :") + " " + tpsPrep+".";
    if(tpsRep !="")
        expt = expt + QObject::tr(" Repos :") + " " + tpsRep+".";
    if(tpsCuis !="")
        expt = expt + "<br/> " + QObject::tr("Cuisson :") + " " + tpsCuis+".";
    expt = expt + "</b> <br/> " + descExpt + "...";
    return expt;
}


/**
 * @brief Functions::copyRecursively
 * @param srcFilePath : dossier source
 * @param tgtFilePath : dossier de destination
 * @return true if copy succeed, false else
 * Copy recursively a directory to another directory
 */
bool Functions::copyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath
                    = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                    = tgtFilePath + QLatin1Char('/') + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}

/**
 * @brief Functions::removeDir
 * @param dirName : directory to remove
 * @return true if remove succeed, false else
 * Remove a directory recursively
 */
bool Functions::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}

bool Functions::downloadPicture(QString url, QString fileName, QWidget *parent) {
    FileDownloader *fdower = new FileDownloader(url, QObject::tr("Récupération de l'image d'illustration de la recette..."), parent);
             QByteArray resData = fdower->downloadedData();
    resData = fdower->downloadedData();
    QString imgFileSave = fileName;
    QFile file(imgFileSave);
    file.open(QFile::WriteOnly);
    file.write(resData);
    file.close();
    return true;
}

bool Functions::saveLastDir(QString action, QString dir) {
    QString filename = confDir;
    if (action == "backup") {
        filename += "/.dirBckp";
    }
    else if(action == "img") {
        filename += "/.dirImg";
    }
    else {
        filename = "";
    }
    if (filename != "") {
        QFile file(filename);
        file.remove(".");
        file.open(QFile::WriteOnly);
        QTextStream stream (&file);
        stream.setCodec("UTF-8"); //Not useful on linux system as it's a default, but Windows has its own defaults....
        stream << dir << endl;
        file.close();
        return true;
    }
    return false;
}

QString Functions::getLastDir(QString action) {
    QString filename = confDir;
    if (action == "backup") {
        filename += "/.dirBckp";
    }
    else if(action == "img") {
        filename += "/.dirImg";
    }
    else {
        filename = "";
    }
    if (filename != "") {
        QFile file(filename);
        if (file.exists()) {
            file.open(QFile::ReadOnly);
            QTextStream stream(&file);
            stream.setCodec("UTF-8");
            QString dir = stream.readLine();
            file.close();
            return dir;
        }
        return NULL;
    }
    return NULL;
}

void Functions::downloadUpdate(QString adresse, QWidget *parent) {
    QString fileToSave = adresse.split("/").last();
    QString extOfFile = fileToSave.split(".").last();
    QString fileToSaveUser = QFileDialog::getSaveFileName(parent, QObject::tr("Enregistrer l'archive sous..."), userDir + "/" + fileToSave, QObject::tr("Package d'installation  :") + " *."+extOfFile+" (*."+extOfFile+")");
    if (fileToSaveUser != "")
    {
        FileDownloader *fdower = new FileDownloader(adresse, QObject::tr("Téléchargement de la mise à jour...") ,parent);
        QByteArray package = fdower->downloadedData();
        QFile saveFile(fileToSaveUser);
        saveFile.open(QIODevice::WriteOnly);
        saveFile.write(package);
        saveFile.close();
        QMessageBox::information(parent, QObject::tr("Téléchargement terminé !"), QObject::tr("Vous pouvez maintenant installer\nle package d'installation téléchargé !"), QMessageBox::Ok);
    }

}
