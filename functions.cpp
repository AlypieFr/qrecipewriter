/*
 * © 2013-2014 Conseils en Cuisine !
 *
 * CeCWriter et l'ensemble de ses putils est fournit sous la licence Creative Common BY-NC-SA.
 * Toutes les modifications et la redistribution sont autorisés pour une utilisation NON COMMERCIALE.
 * Par ailleurs, les modifications et la reproduction doivent respecter les règles ci-dessous :
 *    - Cette en-tête doit être maintenue.
 *    - Vous devez redistribuer la version modifiée ou non sous licence Creative Common au moins autant
 *      restrictive.
 *    - ConseilsEnCuisine! ne peut être tenu pour responsable des versions modifiées et/ou redistribuées.
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
extern bool cecPrinter;
extern bool cecSearch;
extern bool cecCoupDeCoeur;
extern bool sendAuto;
extern int configActive;

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
            /*if(xml.name() == "addrSite") {
                xml.readNext();
                addrSite = xml.text().toString();
                continue;
            }
            if(xml.name() == "addrPub") {
                xml.readNext();
                addrPub = xml.text().toString();
                continue;
            }*/
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
            /*if(xml.name() == "dirDistPict") {
                xml.readNext();
                dirDistPict = xml.text().toString();
                continue;
            }
            if(xml.name() == "cecPrinter") {
                xml.readNext();
                cecPrinter = xml.text().toString() == "1" ? true: false;
                continue;
            }*/
            if(xml.name() == "sendAuto") {
                xml.readNext();
                sendAuto = xml.text().toString() == "1" ? true: false;
                continue;
            }
            /*if(xml.name() == "cecSearch") {
                xml.readNext();
                cecSearch = xml.text().toString() == "1" ? true: false;
                continue;
            }
            if(xml.name() == "cecCoupDeCoeur") {
                xml.readNext();
                cecCoupDeCoeur = xml.text().toString() == "1" ? true: false;
                continue;
            }*/
            if (xml.name() == "activeServerConfig") {
                xml.readNext();
                configActive = xml.text().toString().toInt();
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
                if(xmlS.name() == "cecPrinter") {
                    xmlS.readNext();
                    cecPrinter = xmlS.text().toString() == "1" ? true: false;
                    continue;
                }
                if(xmlS.name() == "cecSearch") {
                    xmlS.readNext();
                    cecSearch = xmlS.text().toString() == "1" ? true: false;
                    continue;
                }
                if(xmlS.name() == "cecCoupDeCoeur") {
                    xmlS.readNext();
                    cecCoupDeCoeur = xmlS.text().toString() == "1" ? true: false;
                    continue;
                }
            }
        }
    }
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
                           QStringList preparation, QStringList conseils, QString picture, QMap<QString, QString> liens, QString filename, QString coupDeCoeur)
{
    QFile* file = new QFile(dirSav + "/" + filename + ".rct");
    file->remove(".");
    file->open(QIODevice::WriteOnly);
    QXmlStreamWriter writer(file);

    writer.setAutoFormatting(true);
    writer.writeStartDocument("1.0");
    writer.writeTextElement("titre", title);
    writer.writeStartElement("categories");
    int id = 1;
    foreach (QString cat, categories) {
        writer.writeTextElement("cat" + QString::number(id), cat);
        id++;
    }
    writer.writeEndElement();
    writer.writeTextElement("image", picture);
    writer.writeTextElement("tpsPrep", tpsPrep);
    writer.writeTextElement("tpsCuis", tpsCuis);
    writer.writeTextElement("tpsRep", tpsRep);
    writer.writeTextElement("nbPers", nbPers);
    writer.writeTextElement("precision", precision);
    writer.writeTextElement("description", description);
    if (ingredients.size() > 0)
    {
        writer.writeStartElement("ingredients");
        id = 1;
        foreach (QString ingr, ingredients) {
            writer.writeTextElement("ingr" + QString::number(id), ingr);
            id++;
        }
        writer.writeEndElement();
    }
    if (materiel.size() > 0)
    {
        writer.writeStartElement("materiel");
        id = 1;
        foreach (QString mat, materiel) {
            writer.writeTextElement("mat" + QString::number(id), mat);
            id++;
        }
        writer.writeEndElement();
    }
    if (preparation.size() > 0)
    {
        writer.writeStartElement("preparation");
        id = 1;
        foreach (QString prep, preparation) {
            writer.writeTextElement("prep" + QString::number(id), prep);
            id++;
        }
        writer.writeEndElement();
    }
    if (conseils.size() > 0)
    {
        writer.writeStartElement("conseils");
        id = 1;
        foreach (QString cons, conseils) {
            writer.writeTextElement("cons" + QString::number(id), cons);
            id++;
        }
        writer.writeEndElement();
    }
    writer.writeTextElement("coupDeCoeur", coupDeCoeur);
    if (liens.size() > 0)
    {
        writer.writeStartElement("liens");
        for (int k = 1; k <= liens.size(); ++k) {
            writer.writeTextElement("L" + QString::number(k), liens["L" + QString::number(k)]);
        }
        writer.writeEndDocument();
    }
    file->close();
    return true;
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

    //Loading xml file:
    XMLDocument doc;
    doc.LoadFile(fileName.toStdString().c_str());

    //Titre:
    XMLElement* elem = doc.FirstChildElement("titre");
    if (elem)
    {
        result.insert("titre", QStringList(elem->GetText()));
    }

    //Categories:
    QStringList elemsList;
    elem = doc.FirstChildElement("categories");
    if (elem)
    {
        for( XMLNode* ele = elem->FirstChild();
                 ele;
                 ele = ele->NextSibling() )
            {
            elemsList.append(ele->ToElement()->GetText());
        }
        result.insert("categories", elemsList);
    }

    //Image:
    elem = doc.FirstChildElement("image");
    if (elem)
    {
        result.insert("image", QStringList(elem->GetText()));
    }

    //Temps de preparation:
    elem = doc.FirstChildElement("tpsPrep");
    if (elem)
    {
        QString txt = elem->GetText();
        QStringList tps = txt.split("h");
        result.insert("tpsPrep", tps);
    }

    //Temps de cuisson:
    elem = doc.FirstChildElement("tpsCuis");
    if (elem)
    {
        QString txt = elem->GetText();
        QStringList tps = txt.split("h");
        result.insert("tpsCuis", tps);
    }

    //Temps de repos:
    elem = doc.FirstChildElement("tpsRep");
    if (elem)
    {
        QString txt = elem->GetText();
        QStringList tps = txt.split(QRegExp("[jh]"));
        result.insert("tpsRep", tps);
    }

    //Nb personnes:
    elem = doc.FirstChildElement("nbPers");
    if (elem)
    {
        result.insert("nbPers", QStringList(elem->GetText()));
    }

    //Precision:
    elem = doc.FirstChildElement("precision");
    if (elem)
    {
        result.insert("precision", QStringList(elem->GetText()));
    }

    //Description:
    elem = doc.FirstChildElement("description");
    if (elem)
    {
        result.insert("description", QStringList(elem->GetText()));
    }

    //Ingredients:
    elemsList.clear();
    elem = doc.FirstChildElement("ingredients");
    if (elem)
    {
        for( XMLNode* ele = elem->FirstChild();
                 ele;
                 ele = ele->NextSibling() )
            {
            elemsList.append(ele->ToElement()->GetText());
        }
        result.insert("ingredients", elemsList);
    }

    //Materiel:
    elemsList.clear();
    elem = doc.FirstChildElement("materiel");
    if (elem)
    {
        for( XMLNode* ele = elem->FirstChild();
                 ele;
                 ele = ele->NextSibling() )
            {
            elemsList.append(ele->ToElement()->GetText());
        }
        result.insert("materiel", elemsList);
    }

    //Preparation:
    elemsList.clear();
    elem = doc.FirstChildElement("preparation");
    if (elem)
    {
        for( XMLNode* ele = elem->FirstChild();
                 ele;
                 ele = ele->NextSibling() )
            {
            elemsList.append(ele->ToElement()->GetText());
        }
        result.insert("preparation", elemsList);
    }

    //Conseils:
    elemsList.clear();
    elem = doc.FirstChildElement("conseils");
    if (elem)
    {
        for( XMLNode* ele = elem->FirstChild();
                 ele;
                 ele = ele->NextSibling() )
            {
            elemsList.append(ele->ToElement()->GetText());
        }
        result.insert("conseils", elemsList);
    }

    //Coup de coeur:
    elemsList.clear();
    elem = doc.FirstChildElement("coupDeCoeur");
    if (elem)
    {
        result.insert("coupDeCoeur", QStringList(elem->GetText()));
    }

    //Liens:
    elemsList.clear();
    elem = doc.FirstChildElement("liens");
    if (elem)
    {
        for( XMLNode* ele = elem->FirstChild();
                 ele;
                 ele = ele->NextSibling() )
            {
            elemsList.append(ele->ToElement()->GetText());
        }
        result.insert("liens", elemsList);
    }

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
        data.replace(imgBal, centerBegin + "<img src=\"" + dirDistPict + img.split("/").last() + "\" alt=\"Image d'illustration\" width=\"" + largeur + "\" height=\"" + hauteur + "\"" + classDef + " />" + centerEnd + "<br/>");
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
                                    int hRep, int minRep, int nbPersonnes, QString precision,
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
             + description+ "</description></div><div id=\"detail\"><temps><b>Temps de Préparation : "+tpsPrep;
    //Adding temps de repos if used:
    if (tpsRep != "")
    {
        htmlCode = htmlCode + "<br/>Temps de Repos : "+tpsRep;
    }
    //Adding temps de cuisson if used:
    if (tpsCuis != "")
    {
            htmlCode = htmlCode + "<br/>Temps de Cuisson : "+tpsCuis;
    }
    //Adding next code:
    htmlCode = htmlCode + "</b></temps></div><!--more--><br/><ingredients><p><b>Ingrédients (pour " + QString::number(nbPersonnes);
    //Adding word "personne(s)":
    if (nbPersonnes > 1)
        htmlCode = htmlCode + " personnes";
    else
        htmlCode = htmlCode + " personne";
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
        htmlCode = htmlCode + "<p><b>Matériel nécessaire :</b></p>"+materiel;
    }
    //Preparation:
    htmlCode = htmlCode + "</ingredients><preparation><p><b>Préparation :</b></p>"+preparation+"</preparation>";
    //Conseils if used:
    if (conseils != "")
    {
        htmlCode = htmlCode + "<conseils><p><b>Conseils :</b></p>"+conseils+"</conseils>";
    }
    //Adding "Version imprimable" balise, only if the website is Conseils En Cuisine !, because others might not use it:
    if (cecPrinter) {
        htmlCode = htmlCode + "<br/>[VERSION_IMPRIMABLE]";
    }
    //Replace oe by "e dans l'o", because we speak French :
    htmlCode = htmlCode.replace("oe", "&oelig;");

    htmlCode = setPrintTags(htmlCode);

    htmlCode = insertMovies(htmlCode);

    //breaking spaces:
    htmlCode = htmlCode.replace(" :", "&#8239;:").replace(" !", "&#8239;!").replace(" ;", "&#8239;;").replace(" /", "&#8239;/").replace(" =", "&#8239;=");

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
    QString expt = "<b>Préparation : "+tpsPrep+".";
    if(tpsRep !="")
        expt = expt + " Repos : "+tpsRep+".";
    if(tpsCuis !="")
        expt = expt + "<br/> Cuisson : "+tpsCuis+".";
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
