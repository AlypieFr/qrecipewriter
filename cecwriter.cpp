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

#include "cecwriter.h"
#include "ui_cecwriter.h"

/**
 * POINTERS TO QDIALOGS USED BY THE PROGRAM
 */
extern void *ptr2opt;
extern void *ptr2cat;
extern void *ptr2apropos;
extern void *ptr2sendManual;
extern void *ptr2sendAutomatic;

/**
 * GLOBAL VARIABLES
 */
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
extern QString correction;
extern bool cancel;
extern bool cecPrinter;
extern bool sendAuto;

extern QString VERSION;
extern QString QTVERSION;

extern QMap<QString, QString> liens;

/**
 * Constructor of the class
 */
CeCWriter::CeCWriter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CeCWriter)
{

    ui->setupUi(this);
}

CeCWriter::~CeCWriter()
{
    delete ui;
}

/**
 * @brief CeCWriter::closeEvent
 * @param event
 * When user close window, we check if he has modified the recipe until the last saving.
 */
void CeCWriter::closeEvent(QCloseEvent *event)
{
    if (!checkHasBeenModified())
        saveSizeAndQuit();
    else
    {
        int rep = QMessageBox::warning(this, "Enregistrer avant de quitter ?", "La recette a été modifiée. Voulez-vous enregistrer la recette avant de quitter le programme ?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if (rep == QMessageBox::Yes)
        {
            on_enregistrer_clicked();
            if (!checkHasBeenModified())
                this->close();
            else
                event->ignore();
        }
        else if (rep == QMessageBox::No)
        {
            saveSizeAndQuit();
        }
        else
        {
            event->ignore();
        }
    }
}

void CeCWriter::saveSizeAndQuit()
{
    QFile saveSize (confDir + ".size");
    if (saveSize.exists())
    {
        saveSize.remove();
    }
    saveSize.open(QFile::WriteOnly);
    QTextStream in (&saveSize);
    in.setCodec("UTF-8"); //Not useful on linux system as it's a default, but Windows has its own defaults....
    in << QString::number(this->isMaximized()) << "\n" << QString::number(this->width()) << "\t" << QString::number(this->height());
    in.flush();
    saveSize.close();
    this->close();
}

/**
 * @brief CeCWriter::changeEvent
 * @param event
 * User can edit the main picture in another software. This image is shown as icon in the corresponding button.
 * So, when the Main Window get active, we update the icon.
 */
void CeCWriter::changeEvent(QEvent *event)
{
    if(event->type() == QEvent::ActivationChange)
    {
        if(this->isActiveWindow())
        {
            if (imgFile != "")
            {
                ui->mainPicture->setIcon(QIcon(imgFile));
            }
        }
    }
}

/**
 * @brief CeCWriter::config
 * Function launched if confFile does not exists (usually if it is the first time the user launch the program)
 */
void CeCWriter::config()
{
    Options* opt = new Options();
    opt->init();
    QMessageBox::information(0, "Configuration introuvable !", "C'est la première fois que vous lancez l'application. Vous allez être invité à configurer le logiciel.", QMessageBox::Ok);
    QDir dirConf (confDir);
    if (!dirConf.exists())
        dirConf.mkpath(".");
    #ifdef Q_OS_LINUX
    QString path = QCoreApplication::applicationDirPath();
    QString Program = "sh "+path+"/checkGimp.bash";
    QProcess *myProcess = new QProcess();
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(Program);
    myProcess->waitForFinished();
    QString test(myProcess->readAllStandardOutput());
    if(test == "yes\n")
    {
        editPict ="gimp";
    }
    #endif
    opt->init();
    opt->exec();
    delete opt;
    QFile configFile(confFile);
    if (configFile.exists())
    {
        init();
        show();
    }
    else
    {
        QApplication::quit();
    }
}

/**
 * @brief CeCWriter::init
 * Function launched at the startup of the program, after config if config must be launched.
 * It initialise variables and components of the program
 */
void CeCWriter::init()
{
    QFile saveFile (confDir + ".size");
    ui->grasButton->setFocusPolicy(Qt::NoFocus);
    ui->italicButton->setFocusPolicy(Qt::NoFocus);
    ui->soulignButton->setFocusPolicy(Qt::NoFocus);
    ui->commButton->setFocusPolicy(Qt::NoFocus);
    ui->lienButton->setFocusPolicy(Qt::NoFocus);
    ui->imgButton->setFocusPolicy(Qt::NoFocus);
    ui->movie->setFocusPolicy(Qt::NoFocus);
    ui->abcButton->setFocusPolicy(Qt::NoFocus);
    ui->ingrListMoins->setFocusPolicy(Qt::NoFocus);
    ui->ingrListPlus->setFocusPolicy(Qt::NoFocus);
    ui->ingrListShow->setFocusPolicy(Qt::NoFocus);
    ui->prepListMoins->setFocusPolicy(Qt::NoFocus);
    ui->prepListPlus->setFocusPolicy(Qt::NoFocus);
    ui->prepListShow->setFocusPolicy(Qt::NoFocus);
    ui->noPrint->setFocusPolicy(Qt::NoFocus);
    ui->printOnly->setFocusPolicy(Qt::NoFocus);
    //Center window in the screen
    isMax = false;
    int w = this->width(), h = this->height();
    if (saveFile.exists())
    {
        saveFile.open(QFile::ReadOnly);
        QTextStream out (&saveFile);
        out.setCodec("UTF-8"); //Not useful on linux system as it's a default, but Windows has its own defaults....
        QString line = out.readLine();
        isMax = (line == "1" ? true : false);
        line = out.readLine();
        QStringList sizes = line.split("\t");
        w = sizes[0].toInt();
        h = sizes[1].toInt();
    }
    if (!isMax)
    {
        int scr_width = QApplication::desktop()->width();
        int scr_height = QApplication::desktop()->height();
        int x = (scr_width - w) /2;
        int y = (scr_height - h) /2;
        this->setGeometry(x, y, w, h);
    }
    //Define variables:
    imgFile = "";
    imgFileName = "";
    ingrEdit = -1;
    matEdit = -1;
    prepEdit = -1;
    consEdit = -1;
    ingrComm = false;
    matComm = false;
    prepComm = false;
    consComm = false;
    addrLnk = "";
    idLien = 1;
    liens.clear();
    saveFileName = "";
    balise = "";
    ui->editPicture->setEnabled(false);
    QColor colorBck = ui->listIngr->palette().color(QPalette::Text);
    if ((colorBck.red() + colorBck.green() + colorBck.blue()) / 3 < 127.5)
        colorEdit = QColor(117, 245, 149);
    else
        colorEdit = QColor(5, 122, 19);
    QColor colorWin = this->palette().color(QPalette::Background);
    if ((colorWin.red() + colorWin.green() + colorWin.blue()) / 3 < 127.5)
    {
        ui->logo->setPixmap(QPixmap(":/images/logo_blanc.png"));
    }
    QColor colorBut = ui->abcButton->palette().color(QPalette::Background);
    if ((colorBut.red() + colorBut.green() + colorBut.blue()) / 3 < 127.5)
    {
        //ui->lienButton->setIcon(QPixmap(":/images/lien_button_blanc.png"));
        //ui->imgButton->setIcon(QPixmap(":/images/img_blanc.png"));
        //ui->abcButton->setIcon(QPixmap(":/images/abc_blanc.png"));
    }
    //Set categories:
    ui->scrollCats->setFrameShape(QFrame::NoFrame); //Remove border
    QWidget *wid = new QWidget();
    QWidget* wLeft = new QWidget(wid);
    QWidget* wRight = new QWidget(wid);
    QVBoxLayout *lLeft = new QVBoxLayout(wLeft);
    lLeft->setMargin(0);
    lLeft->setSpacing(2);
    QVBoxLayout *lRight = new QVBoxLayout(wRight);
    lRight->setMargin(0);
    lRight->setSpacing(2);
    catLay = new QHBoxLayout(wid);
    catLay->setMargin(0);
    if(!confCatFile->exists())
    {
        QCheckBox *platPrincipal = new QCheckBox("Plat principal");
        platPrincipal->setMaximumHeight(21);
        QCheckBox *entree = new QCheckBox("Entrée");
        entree->setMaximumHeight(21);
        QCheckBox *dessert = new QCheckBox("Dessert");
        dessert->setMaximumHeight(21);
        QCheckBox *accompagnement = new QCheckBox("Accompagnement");
        accompagnement->setMaximumHeight(21);
        lLeft->addWidget(platPrincipal);
        lLeft->addWidget(entree);
        lLeft->addWidget(dessert);
        lLeft->addWidget(accompagnement);
        QCheckBox *base = new QCheckBox("Base");
        base->setMaximumHeight(21);
        QCheckBox *biscuits = new QCheckBox("Biscuits && Friandises");
        biscuits->setMaximumHeight(21);
        QCheckBox *pain = new QCheckBox("Pain && Viennoiserie");
        pain->setMaximumHeight(21);
        QCheckBox *divers = new QCheckBox("Divers");
        divers->setMaximumHeight(21);
        lRight->addWidget(base);
        lRight->addWidget(biscuits);
        lRight->addWidget(pain);
        lRight->addWidget(divers);
        catLay->addWidget(wLeft);
        catLay->addWidget(wRight);
        //Fill disctionary:
        categories.insert(platPrincipal, "Plat principal");
        namesCats.append("Plat principal");
        categories.insert(entree, "Entrée");
        namesCats.append("Entrée");
        categories.insert(dessert, "Dessert");
        namesCats.append("Dessert");
        categories.insert(accompagnement, "Accompagnement");
        namesCats.append("Accompagnement");
        categories.insert(base, "Base");
        namesCats.append("Base");
        categories.insert(biscuits, "Biscuits & Friandises");
        namesCats.append("Biscuits & Friandises");
        categories.insert(pain, "Pain & Viennoiserie");
        namesCats.append("Pain & Viennoiseries");
        categories.insert(divers, "Divers");
        namesCats.append("Divers");
    }
    else {
        QStringList theCats = Functions::loadCategories();
        int idC =0;
        foreach (QString cat, theCats) {
            QCheckBox *catI = new QCheckBox(cat.replace("&", "&&"));
            catI->setMaximumHeight(21);
            if (idC < 4 || (theCats.size() > 8 && idC < (theCats.size()+1) / 2))
            {
                lLeft->addWidget(catI);
            }
            else
            {
                lRight->addWidget(catI);
            }
            catLay->addWidget(wLeft);
            catLay->addWidget(wRight);
            //Fill disctionary and list:
            categories.insert(catI, cat);
            namesCats.append(cat);
            idC++;
        }
    }
    ui->scrollCats->setWidget(wid);
    //Load config:
    Functions::loadConfig();
    if (!cecPrinter) {
        ui->noPrint->setToolTip("Ne pas imprimer une partie de texte\n[NÉCESSITE D'ACTIVER CEC PRINTER]");
        ui->printOnly->setToolTip("Imprimer une partie de texte mais ne pas l'afficher\n[NÉCESSITE D'ACTIVER CEC PRINTER]");
    }
    //Test if dictrionary path is given:
    if (corrOrtho == "")
    {
        ui->abcButton->setEnabled(false);
        ui->actionCorrection_orthographique->setEnabled(false);
    }
    //Initialize ident variables:
    idIngr = 0;
    idPrep = "0.0";
    //editPict events:
    ui->editPicture->installEventFilter(this);
    //Set ListView's style:
    //ON INGR:
    model1 = new QStandardItemModel(this);
    ui->listIngr->setIconSize(QSize(15,15));
    ui->listIngr->setModel(model1);
    ui->listIngr->setTextElideMode(Qt::ElideNone);
    ui->listIngr->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listIngr->setItemDelegate(new ListViewDelegate());
    ui->listIngr->installEventFilter(this);
    ui->editIngr->installEventFilter(this);
    //ON MAT:
    model2 = new QStandardItemModel(this);
    ui->listMat->setIconSize(QSize(15,15));
    ui->listMat->setModel(model2);
    ui->listMat->setTextElideMode(Qt::ElideNone);
    ui->listMat->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listMat->setItemDelegate(new ListViewDelegate());
    ui->listMat->installEventFilter(this);
    ui->editMat->installEventFilter(this);
    //ON PREP:
    model3 = new QStandardItemModel(this);
    ui->listPrep->setIconSize(QSize(20,20));
    ui->listPrep->setModel(model3);
    ui->listPrep->setTextElideMode(Qt::ElideNone);
    ui->listPrep->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listPrep->setItemDelegate(new ListViewDelegate());
    ui->listPrep->installEventFilter(this);
    ui->editPrep->installEventFilter(this);
    //ON CONS:
    model4 = new QStandardItemModel(this);
    ui->listCons->setIconSize(QSize(20,20));
    ui->listCons->setModel(model4);
    ui->listCons->setTextElideMode(Qt::ElideNone);
    ui->listCons->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listCons->setItemDelegate(new ListViewDelegate());
    ui->listCons->installEventFilter(this);
    ui->editCons->installEventFilter(this);
    this->installEventFilter(this);
    //Install events on description field:
    ui->description->installEventFilter(this);
}

/**
 * @brief CeCWriter::resetFields
 * reset fields when nouvelle recette or ouvrir une recette existante is triggered
 */
void CeCWriter::resetFields()
{
    ui->titre->clear();
    foreach (QCheckBox* cat, categories.keys()) {
        cat->setChecked(false);
    }
    imgFile = "";
    imgFileName = "";
    ui->mainPicture->setText("Sélectionner une image...");
    ui->mainPicture->setIcon(QPixmap(":/images/image.png"));
    ui->hPrep->setValue(0);
    ui->minPrep->setValue(0);
    ui->hCuis->setValue(0);
    ui->minCuis->setValue(0);
    ui->jRep->setValue(0);
    ui->hRep->setValue(0);
    ui->minRep->setValue(0);
    ui->nbPersonnes->setValue(0);
    ui->precision->clear();
    ui->description->clear();
    ui->editIngr->clear();
    ui->editPrep->clear();
    ui->editMat->clear();
    ui->editCons->clear();
    model1->clear();
    model2->clear();
    model3->clear();
    model4->clear();
    //Reset other variables:
    ingrEdit = -1;
    matEdit = -1;
    prepEdit = -1;
    consEdit = -1;
    ingrComm = false;
    matComm = false;
    prepComm = false;
    consComm = false;
    addrLnk = "";
    idLien = 1;
    saveFileName = "";
    idIngr = 0;
    idPrep = "0.0";
    ui->prepListShow->setText("0");
    ui->ingrListShow->setText("0");
    ui->editPicture->setEnabled(false);
}

/**
 * @brief CeCWriter::resetCats
 * reset categories if they have been changed in the dedicaded interface
 */
void CeCWriter::resetCats()
{
    categories.clear();
    namesCats.clear();
    QWidget *wid = new QWidget();
    QWidget *wLeft = new QWidget(wid);
    QWidget *wRight = new QWidget(wid);
    QVBoxLayout *lLeft = new QVBoxLayout(wLeft);
    lLeft->setMargin(0);
    lLeft->setSpacing(2);
    QVBoxLayout *lRight = new QVBoxLayout(wRight);
    lRight->setMargin(0);
    lRight->setSpacing(2);
    catLay = new QHBoxLayout(wid);
    catLay->setMargin(0);
    QStringList theCats = Functions::loadCategories();
    int idC =0;
    foreach (QString cat, theCats) {
        QCheckBox *catI = new QCheckBox(cat.replace("&", "&&"));
        catI->setMaximumHeight(21);
        if (idC < 4 || (theCats.size() > 8 && idC < (theCats.size()+1) / 2))
        {
            lLeft->addWidget(catI);
        }
        else
        {
            lRight->addWidget(catI);
        }
        catLay->addWidget(wLeft);
        catLay->addWidget(wRight);
        //Fill disctionary:
        categories.insert(catI, cat);
        namesCats.append(cat);
        idC++;
    }
    ui->scrollCats->setWidget(wid);
}

/**
 * @brief CeCWriter::checkHasBeenModified
 * @return bool : true if the recipe has been modified, else false
 */
bool CeCWriter::checkHasBeenModified()
{
    QStringList actualCats;
    foreach (QCheckBox *catCheckBox, categories.keys()) {
        if (catCheckBox->isChecked()) {
            actualCats.append(categories[catCheckBox]);
        }
    }
    if(saveFileName == "" && ui->titre->text() == "" && actualCats.size() == 0 && imgFile == "" && ui->hPrep->value() == 0 && ui->minPrep->value() == 0
            && ui->hCuis->value() == 0 && ui->jRep->value() == 0 && ui->hRep->value() == 0 && ui->minRep->value() == 0
            && ui->nbPersonnes->value() == 0 && ui->precision->text() == "" && ui->description->toPlainText() == ""
            && model1->rowCount() == 0 && model2->rowCount() == 0 && model3->rowCount() == 0 && model4->rowCount() == 0)
    {
        return false;
    }
    else if (saveFileName != "")
    {
        QStringList ingrList;
        for (int i1 = 0; i1 < model1->rowCount(); ++i1) {
            ingrList.append(model1->item(i1)->text());
        }
        QStringList matList;
        for (int i1 = 0; i1 < model2->rowCount(); ++i1) {
            matList.append(model2->item(i1)->text());
        }
        QStringList prepList;
        for (int i1 = 0; i1 < model3->rowCount(); ++i1) {
            prepList.append(model3->item(i1)->text());
        }
        QStringList consList;
        for (int i1 = 0; i1 < model4->rowCount(); ++i1) {
            consList.append(model4->item(i1)->text());
        }
        bool catsIdentical = true;
        foreach (QString cat, actualCats) {
            if (!saveCategories.contains(cat))
            {
                catsIdentical = false;
            }
        }
        if (catsIdentical)
        {
            foreach (QString cat, saveCategories) {
                if (!actualCats.contains(cat))
                {
                    catsIdentical = false;
                }
            }
        }
        if (ui->titre->text() != saveTitre || !catsIdentical || imgFile != saveMainImage || ui->hPrep->value() != saveHPrep
                || ui->minPrep->value() != saveMinPrep || ui->hCuis->value() != saveHCuis || ui->minCuis->value() != saveMinCuis
                || ui->jRep->value() != saveJRep || ui->hRep->value() != saveHRep || ui->minRep->value() != saveMinRep || ui->nbPersonnes->value() != saveNbPersonnes
                || ui->description->toPlainText() != saveDescription || ingrList.join("\n") != saveIngredients
                || matList.join("\n") != saveMateriel || prepList.join("\n") != savePreparation
                || consList.join("\n") != saveConseils)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}

/**
 * @brief CeCWriter::on_actionOptions_triggered
 * Start options dialog when Options (Préférences) is triggered in menu
 */
void CeCWriter::on_actionOptions_triggered()
{

    Options* opt = (Options*) ptr2opt;
    opt->init();
    opt->exec();
    if (corrOrtho == "")
    {
        ui->abcButton->setEnabled(false);
        ui->actionCorrection_orthographique->setEnabled(false);
    }
    else
    {
        ui->abcButton->setEnabled(true);
        ui->actionCorrection_orthographique->setEnabled(true);
    }
    if (!cecPrinter) {
        ui->noPrint->setToolTip("Ne pas imprimer une partie de texte\n[NÉCESSITE D'ACTIVER CEC PRINTER]");
        ui->printOnly->setToolTip("Imprimer une partie de texte mais ne pas l'afficher\n[NÉCESSITE D'ACTIVER CEC PRINTER]");
    }
    else {
        ui->noPrint->setToolTip("Ne pas imprimer une partie de texte");
        ui->printOnly->setToolTip("Imprimer une partie de texte mais ne pas l'afficher");
    }
}

/**
 * @brief CeCWriter::on_action_propos_triggered
 * Start a propos dialog when A propos is triggered in menu
 */
void CeCWriter::on_action_propos_triggered()
{
    Apropos* apropos = (Apropos*) ptr2apropos;
    apropos->init();
    apropos->exec();
}

/**
 * @brief CeCWriter::on_actionGerer_les_categories_triggered
 * Start to choose categories used by the program
 */
void CeCWriter::on_actionGerer_les_categories_triggered()
{
    Categories* cat = (Categories*) ptr2cat;
    cat->init(namesCats);
    cat->exec();
    if (confCatFile->exists())
        resetCats();
}

/**
 * @brief CeCWriter::on_mainPicture_clicked
 * Change main picture of the recipe
 */
void CeCWriter::on_mainPicture_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Choisir une image",
    dirPict,
    "Images : *.jpg, *.JPG (*.jpg *.JPG);;");
    if ( !fileName.isEmpty() )
    {
        imgFile = fileName;
        imgFileName = imgFile.split("/").last();
        ui->mainPicture->setText(imgFileName);
        ui->mainPicture->setIcon(QPixmap(imgFile));
        ui->editPicture->setEnabled(true);
    }
}

/**
 * @brief CeCWriter::format_clicked
 * @param typeF : the type of the tag format (b, u or i)
 * When a tag format button is released (gras, italique, souligné)
 */
void CeCWriter::format_clicked(QString typeF)
{
    if (ui->editIngr->hasFocus()) {
        int selStart = ui->editIngr->selectionStart();
        if (selStart == -1) {
            int pos = ui->editIngr->cursorPosition();
            QString text = ui->editIngr->text();
            text.insert(pos, "<"+ typeF +"></"+ typeF +">");
            ui->editIngr->setText(text);
            ui->editIngr->setCursorPosition(pos+2+typeF.length());
        }
        else {
            QString sel = ui->editIngr->selectedText();
            QString initial = ui->editIngr->text();
            ui->editIngr->setText(initial.left(selStart) + "<"+ typeF +">" + sel
                  + "</"+ typeF +">" + initial.right(initial.length() - (selStart + sel.length())));
            ui->editIngr->setCursorPosition(selStart + sel.length() + 5 + (typeF.length() * 2));
        }
    }
    else if (ui->editMat->hasFocus()) {
        int selStart = ui->editMat->selectionStart();
        if (selStart == -1) {
            int pos = ui->editMat->cursorPosition();
            QString text = ui->editMat->text();
            text.insert(pos, "<"+ typeF +"></"+ typeF +">");
            ui->editMat->setText(text);
            ui->editMat->setCursorPosition(pos+2+typeF.length());
        }
        else {
            QString sel = ui->editMat->selectedText();
            QString initial = ui->editMat->text();
            ui->editMat->setText(initial.left(selStart) + "<"+ typeF +">" + sel
                  + "</"+ typeF +">" + initial.right(initial.length() - (selStart + sel.length())));
            ui->editMat->setCursorPosition(selStart + sel.length() + 5 + (typeF.length() * 2));
        }
    }
    else if (ui->editPrep->hasFocus()) {
        int selStart = ui->editPrep->textCursor().selectionStart();
        int selEnd = ui->editPrep->textCursor().selectionEnd();
        if (selStart == selEnd) {
            int pos = ui->editPrep->textCursor().positionInBlock();
            QString text = ui->editPrep->toPlainText();
            text.insert(pos, "<"+ typeF +"></"+ typeF +">");
            ui->editPrep->setPlainText(text);
            QTextCursor cursor = ui->editPrep->textCursor();
            cursor.setPosition(pos+2+typeF.length());
            ui->editPrep->setTextCursor(cursor);
        }
        else {
            QString sel = ui->editPrep->textCursor().selectedText();
            QString initial = ui->editPrep->toPlainText();
            ui->editPrep->setPlainText(initial.left(selStart) + "<"+ typeF +">" + sel
                  + "</"+ typeF +">" + initial.right(initial.length() - (selStart + sel.length())));
            QTextCursor cursor = ui->editPrep->textCursor();
            cursor.setPosition(selEnd + 5 + (typeF.length() * 2));
            ui->editPrep->setTextCursor(cursor);
        }
    }
    else if (ui->editCons->hasFocus()) {
        int selStart = ui->editCons->textCursor().selectionStart();
        int selEnd = ui->editCons->textCursor().selectionEnd();
        if (selStart == selEnd) {
            int pos = ui->editCons->textCursor().positionInBlock();
            QString text = ui->editCons->toPlainText();
            text.insert(pos, "<"+ typeF +"></"+ typeF +">");
            ui->editCons->setPlainText(text);
            QTextCursor cursor = ui->editCons->textCursor();
            cursor.setPosition(pos+2+typeF.length());
            ui->editCons->setTextCursor(cursor);
        }
        else {
            QString sel = ui->editCons->textCursor().selectedText();
            QString initial = ui->editCons->toPlainText();
            ui->editCons->setPlainText(initial.left(selStart) + "<"+ typeF +">" + sel
                  + "</"+ typeF +">" + initial.right(initial.length() - (selStart + sel.length())));
            QTextCursor cursor = ui->editCons->textCursor();
            cursor.setPosition(selEnd + 5 + (typeF.length() * 2));
            ui->editCons->setTextCursor(cursor);
        }
    }
    else if (ui->description->hasFocus())
    {
        int selStart = ui->description->textCursor().selectionStart();
        int selEnd = ui->description->textCursor().selectionEnd();
        if (selStart == selEnd) {
            int pos = ui->description->textCursor().positionInBlock();
            QString text = ui->description->toPlainText();
            text.insert(pos, "<"+ typeF +"></"+ typeF +">");
            ui->description->setPlainText(text);
            QTextCursor cursor = ui->description->textCursor();
            cursor.setPosition(pos+2+typeF.length());
            ui->description->setTextCursor(cursor);
        }
        else {
            QString sel = ui->description->textCursor().selectedText();
            QString initial = ui->description->toPlainText();
            ui->description->setPlainText(initial.left(selStart) + "<"+ typeF +">" + sel
                  + "</"+ typeF +">" + initial.right(initial.length() - (selStart + sel.length())));
            QTextCursor cursor = ui->description->textCursor();
            cursor.setPosition(selEnd + 5 + (typeF.length() * 2));
            ui->description->setTextCursor(cursor);
        }
    }
}

/**
 * @brief CeCWriter::on_grasButton_clicked
 * On gras button clicked
 */
void CeCWriter::on_grasButton_clicked()
{
    format_clicked("b");
}

/**
 * @brief CeCWriter::on_italicButton_clicked
 * On italique button clicked
 */
void CeCWriter::on_italicButton_clicked()
{
    format_clicked("i");
}

/**
 * @brief CeCWriter::on_soulignButton_clicked
 * On souligné button clicked
 */
void CeCWriter::on_soulignButton_clicked()
{
    format_clicked("u");
}

/**
 * @brief CeCWriter::on_noPrint_clicked
 * On no print button clicked
 */
void CeCWriter::on_noPrint_clicked()
{
    format_clicked("np");
}

/**
 * @brief CeCWriter::on_printOnly_clicked
 * On print only button clicked
 */
void CeCWriter::on_printOnly_clicked()
{
    format_clicked("po");
}

/**
 * @brief CeCWriter::on_commButton_clicked
 * On comment button clicked : button to write comments inside the lists
 */
void CeCWriter::on_commButton_clicked()
{
    if (ui->editIngr->hasFocus()) {
        if (!ingrComm)
        {
            ingrComm = true;
            ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
        }
        else
        {
            ingrComm = false;
            ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
        }
    }
    if (ui->editMat->hasFocus()) {
        if (!matComm)
        {
            matComm = true;
            ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
        }
        else
        {
            matComm = false;
            ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
        }
    }
    if (ui->editPrep->hasFocus()) {
        if (!prepComm)
        {
            prepComm = true;
            ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
        }
        else
        {
            prepComm = false;
            ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
        }
    }
    if (ui->editCons->hasFocus()) {
        if (!consComm)
        {
            consComm = true;
            ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
        }
        else
        {
            consComm = false;
            ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
        }
    }
}

/**
 * @brief CeCWriter::on_lienButton_clicked
 * On lien clicked, if a compatible QLineEdit or QPlainTextEdit has focus and contains selected text,
 * display the dialog for selecting target link, and add then link
 */
void CeCWriter::on_lienButton_clicked()
{
    QString cible = "";
    QString searchLink = "";
    if (ui->editIngr->hasFocus() && ui->editIngr->selectionStart() != -1)
    {
        cible = "editIngr";
        searchLink = ui->editIngr->text().mid(ui->editIngr->selectionStart() - 5, 5 + ui->editIngr->selectedText().length() + 6);
    }
    else if (ui->editMat->hasFocus() && ui->editMat->selectionStart() != -1)
    {
        cible = "editMat";
        searchLink = ui->editMat->text().mid(ui->editMat->selectionStart() - 5, 5 + ui->editMat->selectedText().length() + 6);
    }
    else if (ui->editPrep->hasFocus() && ui->editPrep->textCursor().selectionStart() != ui->editPrep->textCursor().selectionEnd())
    {
        cible = "editPrep";
        searchLink = ui->editPrep->toPlainText().mid(ui->editPrep->textCursor().selectionStart() - 5, 5 + ui->editPrep->textCursor().selectedText().length() + 6);
    }
    else if (ui->editCons->hasFocus() && ui->editCons->textCursor().selectionStart() != ui->editCons->textCursor().selectionEnd())
    {
        cible = "editCons";
        searchLink = ui->editCons->toPlainText().mid(ui->editCons->textCursor().selectionStart() - 5, 5 + ui->editCons->textCursor().selectedText().length() + 6);
    }
    else if (ui->description->hasFocus() && ui->description->textCursor().selectionStart() != ui->description->textCursor().selectionEnd())
    {
        cible = "description";
        searchLink = ui->description->toPlainText().mid(ui->description->textCursor().selectionStart() - 5, 5 + ui->description->textCursor().selectedText().length() + 6);
    }
    if (cible != "")
    {
        QString idL = "L" + QString::number(idLien);
        bool modified = false;
        chooseLink = new QDialog(this);
        chooseLink->setWindowTitle("Insérer un lien");
        QVBoxLayout *vlay = new QVBoxLayout();
        QLabel *txt = new QLabel("Entrez l'adresse du lien :");
        txt->setMinimumHeight(30);
        chooseAddrLnk = new QLineEdit(ui->titre->text());
        chooseAddrLnk->setMinimumHeight(30);

        QRegExp exp ("<L\\d+>.+</L\\d+>");
        if (searchLink.contains(exp))
        {
            QString resT = exp.cap();
            QRegExp sousExp ("<L\\d+>");
            if (resT.contains(sousExp))
                idL = sousExp.cap();
            idL = idL.mid(1, idL.length() - 2);
            if (liens.contains(idL))
            {
                chooseAddrLnk->setText(liens[idL]);
                modified = true;
            }
            else
            {
                chooseAddrLnk->setText("http://");
                chooseAddrLnk->selectAll();
            }
        }
        else
        {
            chooseAddrLnk->setText("http://");
            chooseAddrLnk->selectAll();
        }

        vlay->addWidget(txt);
        vlay->addWidget(chooseAddrLnk);
        QWidget *buttons = new QWidget();
        QPushButton *valider = new QPushButton("Valider");
        connect(valider, SIGNAL(released()), this, SLOT(handleChooseLnkValider()));
        valider->setMinimumSize(100, 30);
        valider->setMaximumSize(100,30);
        QPushButton *annuler = new QPushButton("Annuler");
        connect(annuler, SIGNAL(released()), this, SLOT(handleChooseLnkAnnuler()));
        annuler->setMinimumSize(100, 30);
        annuler->setMaximumSize(100,30);
        QHBoxLayout *butLay = new QHBoxLayout(buttons);
        butLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        butLay->addWidget(valider);
        butLay->addWidget(annuler);
        vlay->addWidget(buttons);
        chooseLink->setLayout(vlay);
        chooseLink->setFixedHeight(chooseLink->sizeHint().height());
        chooseLink->setMinimumWidth(450);
        chooseLink->exec();
        if (addrLnk != "")
        {
            if (!modified)
            {
                liens.insert(idL, addrLnk);
                if (cible == "editIngr")
                {
                    int selStart = ui->editIngr->selectionStart();
                    QString sel = ui->editIngr->selectedText();
                    QString initial = ui->editIngr->text();
                    ui->editIngr->setText(initial.left(selStart) + "<"+ idL +">" + sel
                          + "</"+ idL +">" + initial.right(initial.length() - (selStart + sel.length())));
                    ui->editIngr->setCursorPosition(selStart + sel.length() + 5 + (idL.length() * 2));
                }
                else if (cible == "editMat")
                {
                    int selStart = ui->editMat->selectionStart();
                    QString sel = ui->editMat->selectedText();
                    QString initial = ui->editMat->text();
                    ui->editMat->setText(initial.left(selStart) + "<"+ idL +">" + sel
                          + "</"+ idL +">" + initial.right(initial.length() - (selStart + sel.length())));
                    ui->editMat->setCursorPosition(selStart + sel.length() + 5 + (idL.length() * 2));
                }
                else if (cible == "editPrep")
                {
                    int selStart = ui->editPrep->textCursor().selectionStart();
                    int selEnd = ui->editPrep->textCursor().selectionEnd();
                    QString sel = ui->editPrep->textCursor().selectedText();
                    QString initial = ui->editPrep->toPlainText();
                    ui->editPrep->setPlainText(initial.left(selStart) + "<"+ idL +">" + sel
                          + "</"+ idL +">" + initial.right(initial.length() - (selStart + sel.length())));
                    QTextCursor cursor = ui->editPrep->textCursor();
                    cursor.setPosition(selEnd + 5 + (idL.length() * 2));
                    ui->editPrep->setTextCursor(cursor);
                }
                else if (cible == "editCons")
                {
                    int selStart = ui->editCons->textCursor().selectionStart();
                    int selEnd = ui->editCons->textCursor().selectionEnd();
                    QString sel = ui->editCons->textCursor().selectedText();
                    QString initial = ui->editCons->toPlainText();
                    ui->editCons->setPlainText(initial.left(selStart) + "<"+ idL +">" + sel
                          + "</"+ idL +">" + initial.right(initial.length() - (selStart + sel.length())));
                    QTextCursor cursor = ui->editCons->textCursor();
                    cursor.setPosition(selEnd + 5 + (idL.length() * 2));
                    ui->editCons->setTextCursor(cursor);
                }
                else if (cible == "description")
                {
                    int selStart = ui->description->textCursor().selectionStart();
                    int selEnd = ui->description->textCursor().selectionEnd();
                    QString sel = ui->description->textCursor().selectedText();
                    QString initial = ui->description->toPlainText();
                    ui->description->setPlainText(initial.left(selStart) + "<"+ idL +">" + sel
                          + "</"+ idL +">" + initial.right(initial.length() - (selStart + sel.length())));
                    QTextCursor cursor = ui->description->textCursor();
                    cursor.setPosition(selEnd + 5 + (idL.length() * 2));
                    ui->description->setTextCursor(cursor);
                }
            }
            else
            {
                liens[idL] = addrLnk;
            }
            idLien++;
            addrLnk = "";
        }
    }
}

/**
 * @brief CeCWriter::on_imgButton_clicked
 * On image button clicked, open file choose dialog to choose secondary picture, if a compatible QPlainTextEdit
 * has focus
 */
void CeCWriter::on_imgButton_clicked()
{
    QString cible = "";
    QString afficher = "all";
    int largeur = -1;
    int hauteur = -1;
    QString fileName = "";
    bool center = false;
    QRegExp exp ("^\\[IMG:(\\w+):(\\d+):(\\d+):([^\\]]+)\\]$");
    if (ui->editPrep->hasFocus())
    {
        cible = "editPrep";
        QString selection = ui->editPrep->textCursor().selectedText();
        if (selection.contains(exp))
        {
            afficher = exp.cap(1);
            largeur = exp.cap(2).toInt();
            hauteur = exp.cap(3).toInt();
            fileName = exp.cap(4);
            if (fileName.startsWith("center:")) {
                fileName = fileName.right(fileName.length() - 7);
                center = true;
            }
        }
    }
    else if (ui->editCons->hasFocus())
    {
        cible = "editCons";
        QString selection = ui->editCons->textCursor().selectedText();
        if (selection.contains(exp))
        {
            afficher = exp.cap(1);
            largeur = exp.cap(2).toInt();
            hauteur = exp.cap(3).toInt();
            fileName = exp.cap(4);
        }
    }
    if (cible != "")
    {
        if (fileName == "") {
            fileName = QFileDialog::getOpenFileName(this, "Choisir une image",
            dirPict,
            "Images : *.jpg, *.JPG (*.jpg *.JPG)");
        }
        if (fileName != "")
        {
            InsertPicture* insertPicture = new InsertPicture(this);
            insertPicture->init(fileName, afficher, largeur, hauteur, fileName, center);
            insertPicture->exec();
            bool ok = insertPicture->ok;
            QString largeur = insertPicture->largeur;
            QString hauteur = insertPicture->hauteur;
            QString print = insertPicture->print;
            bool centerImg = insertPicture->centerImg;
            fileName = insertPicture->filename;
            delete insertPicture;
            insertPicture = NULL;
            QString balise = "[IMG:" + print + ":" + largeur + ":" + hauteur + ":" + (centerImg ? "center:" : "") + fileName + "]";
            if (ok) {
                if (cible == "editPrep")
                {
                    int cursPos = ui->editPrep->textCursor().position();
                    int selLen = ui->editPrep->textCursor().selectedText().length();
                    if (selLen > 0) {
                        cursPos = ui->editPrep->textCursor().selectionStart();
                        ui->editPrep->textCursor().removeSelectedText();
                    }
                    QString initial = ui->editPrep->toPlainText();
                    ui->editPrep->setPlainText(initial.left(cursPos) + balise + initial.right(initial.length() - cursPos));
                    QTextCursor cursor = ui->editPrep->textCursor();
                    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, cursPos + balise.length());
                    ui->editPrep->setTextCursor(cursor);
                }
                else if (cible == "editCons")
                {
                    int cursPos = ui->editCons->textCursor().position();
                    int selLen = ui->editCons->textCursor().selectedText().length();
                    if (selLen > 0) {
                        cursPos = ui->editCons->textCursor().selectionStart();
                        ui->editCons->textCursor().removeSelectedText();
                    }
                    QString initial = ui->editCons->toPlainText();
                    ui->editCons->setPlainText(initial.left(cursPos) + balise + initial.right(initial.length() - cursPos));
                    QTextCursor cursor = ui->editCons->textCursor();
                    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, cursPos + balise.length());
                    ui->editCons->setTextCursor(cursor);
                }
            }
        }
    }
}

/**
 * @brief CeCWriter::on_movie_clicked
 */
void CeCWriter::on_movie_clicked()
{
    QString cible = "";
    //Default values :
    int largeur = 640;
    int hauteur = 480;
    int startTime = 0;
    int bordure = 0;
    bool fullscreen = true;
    bool suggestions = true;
    bool commandes = true;
    bool titreVideo = true;
    bool confidentialite = false;
    QString idVideo = "";
    QRegExp exp ("^\\[VIDEO:(\\d+):(\\d+):(\\d+):(\\d+):([01]+):([01]+):([01]+):([01]+):([01]+):([\\w-]+)\\]$");
    if (ui->editPrep->hasFocus())
    {
        QString selection = ui->editPrep->textCursor().selectedText();
        if (selection.contains(exp))
        {
            largeur = exp.cap(1).toInt();
            hauteur = exp.cap(2).toInt();
            startTime = exp.cap(3).toInt();
            bordure = exp.cap(4).toInt();
            fullscreen = (exp.cap(5) == "1" ? true : false);
            suggestions = (exp.cap(6) == "1" ? true : false);
            commandes = (exp.cap(7) == "1" ? true : false);
            titreVideo = (exp.cap(8) == "1" ? true : false);
            confidentialite = (exp.cap(9) == "1" ? true : false);
            idVideo = exp.cap(10);
        }
        cible = "editPrep";
    }
    else if (ui->editCons->hasFocus())
    {
        QString selection = ui->editCons->textCursor().selectedText();
        if (selection.contains(exp))
        {
            largeur = exp.cap(1).toInt();
            hauteur = exp.cap(2).toInt();
            startTime = exp.cap(3).toInt();
            bordure = exp.cap(4).toInt();
            fullscreen = (exp.cap(5) == "1" ? true : false);
            suggestions = (exp.cap(6) == "1" ? true : false);
            commandes = (exp.cap(7) == "1" ? true : false);
            titreVideo = (exp.cap(8) == "1" ? true : false);
            confidentialite = (exp.cap(9) == "1" ? true : false);
            idVideo = exp.cap(10);
        }
        cible = "editCons";
    }
    if (cible != "")
    {
        InsertVideo* insertVideo = new InsertVideo(this);
        insertVideo->init(largeur, hauteur, startTime, bordure, fullscreen, suggestions, commandes, titreVideo, confidentialite, idVideo);
        insertVideo->exec();
        if (insertVideo->ok) {
            QString balise = "[VIDEO:" + QString::number(insertVideo->largeur) + ':' + QString::number(insertVideo->hauteur) + ':' + QString::number(insertVideo->startTime) + ':' + QString::number(insertVideo->bordure) + ':'
                    + QString::number(insertVideo->fullscreen) + ':' + QString::number(insertVideo->suggestions) + ':' + QString::number(insertVideo->commandes)
                    + ':' + QString::number(insertVideo->titreVideo) + ':' + QString::number(insertVideo->confidentialite) + ':' + insertVideo->idVideo + ']';
            if (cible == "editPrep")
            {
                int cursPos = ui->editPrep->textCursor().position();
                int selLen = ui->editPrep->textCursor().selectedText().length();
                if (selLen > 0) {
                    cursPos = ui->editPrep->textCursor().selectionStart();
                    ui->editPrep->textCursor().removeSelectedText();
                }
                ui->editPrep->textCursor().removeSelectedText();
                QString initial = ui->editPrep->toPlainText();
                ui->editPrep->setPlainText(initial.left(cursPos) + balise + initial.right(initial.length() - cursPos));
                QTextCursor cursor = ui->editPrep->textCursor();
                cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, cursPos + balise.length());
                ui->editPrep->setTextCursor(cursor);
            }
            else if (cible == "editCons")
            {
                int cursPos = ui->editCons->textCursor().position();
                int selLen = ui->editCons->textCursor().selectedText().length();
                if (selLen > 0) {
                    cursPos = ui->editCons->textCursor().selectionStart();
                    ui->editCons->textCursor().removeSelectedText();
                }
                QString initial = ui->editCons->toPlainText();
                ui->editCons->setPlainText(initial.left(cursPos) + balise + initial.right(initial.length() - cursPos));
                QTextCursor cursor = ui->editCons->textCursor();
                cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, cursPos + balise.length());
                ui->editCons->setTextCursor(cursor);
            }
        }
        delete insertVideo;
        insertVideo = NULL;
    }
}

/**
 * @brief CeCWriter::on_abcButton_clicked
 * On ABC button clicked, launch spell checking
 */
void CeCWriter::on_abcButton_clicked()
{
    QString dictPath = corrOrtho;
    QString userDict= confDir + "/.userDict.txt";
    QFile fileUserDict (userDict);
    if (!fileUserDict.exists())
    {
        //Create file:
        fileUserDict.open(QFile::WriteOnly);
        fileUserDict.close();
    }
    SpellChecker *spellChecker = new SpellChecker(dictPath, userDict);

    cancel = false;
    //Title correction
    correction = ui->titre->text();
    correction.replace(">","> ");
    SpellCheckDialog *checkTitle = new SpellCheckDialog(spellChecker, this);
    correction.replace("> ",">");
    ui->titre->setText(correction);
    delete(checkTitle);

    //Precision correction
    correction = ui->precision->text();
    correction.replace(">","> ");
    checkTitle = new SpellCheckDialog(spellChecker, this);
    correction.replace("> ",">");
    ui->precision->setText(correction);
    delete(checkTitle);

    //Description correction
    correction = ui->description->toPlainText();
    correction.replace(">","> ");
    if(cancel == false)
    {
        SpellCheckDialog *checkDesc = new SpellCheckDialog(spellChecker, this);
        delete(checkDesc);
    }
    correction.replace("> ",">");
    ui->description->setPlainText(correction);


    //Ingredients correction
    for(int i=0;i<model1->rowCount();++i)
    {
        QString item = model1->item(i)->text();
        correction = item.split("|")[1];
        correction.replace(">","> ");
        if(cancel == false)
        {
            SpellCheckDialog *checkIngr = new SpellCheckDialog(spellChecker, this);
            delete(checkIngr);
        }
        correction.replace("> ",">");
        correction = item.split("|")[0] + "|" + correction;
        model1->item(i)->setText(correction);

    }
    //Materiel correction
    for(int i=0;i<model2->rowCount();++i)
    {
        QString item = model2->item(i)->text();
        correction = item.split("|")[1];
        correction.replace(">","> ");

        if(cancel == false)
        {
            SpellCheckDialog *checkMat = new SpellCheckDialog(spellChecker, this);
            delete(checkMat);
        }
        correction.replace("> ",">");
        correction = item.split("|")[0] + "|" + correction;
        model2->item(i)->setText(correction);

    }
    //Preparation correction
    for(int i=0;i<model3->rowCount();++i)
    {
        QString item = model3->item(i)->text();
        correction = item.split("|")[1];
        correction.replace(">","> ");
        if(cancel == false)
        {
            SpellCheckDialog *checkPrep = new SpellCheckDialog(spellChecker, this);
            delete(checkPrep);
        }
        correction.replace("> ",">");
        correction = item.split("|")[0] + "|" + correction;
        model3->item(i)->setText(correction);

    }
    //Conseils correction
    for(int i=0;i<model4->rowCount();++i)
    {
        QString item = model4->item(i)->text();
        correction = item.split("|")[1];
        correction.replace(">","> ");
        if(cancel == false)
        {
            SpellCheckDialog *checkCon = new SpellCheckDialog(spellChecker, this);
            delete(checkCon);
        }
        correction.replace("> ",">");
        correction = item.split("|")[0] + "|" + correction;
        model4->item(i)->setText(correction);

    }

    if (!cancel)
    {
        QMessageBox::information(
                this,
                tr("Terminé"),
                tr("La correction est terminé."));
    }
}

/**
 * @brief CeCWriter::on_actionCorrection_orthographique_triggered
 * When "Correction orthographique" is triggered in the menu, we do the same as when ABC button was released
 */
void CeCWriter::on_actionCorrection_orthographique_triggered()
{
    on_abcButton_clicked();
}

/**
 * @brief CeCWriter::roundValueToFive
 * Round given value to the nearest number 5-divisibled
 * @param value value to be rounded
 * @return
 */
int CeCWriter::roundValueToFive(double value)
{
    return qRound(value / 5.0) * 5;
}

/**
 * @brief CeCWriter::on_minPrep_editingFinished
 */
void CeCWriter::on_minPrep_editingFinished()
{
    ui->minPrep->setValue(roundValueToFive(ui->minPrep->value()));
}

/**
 * @brief CeCWriter::on_minCuis_editingFinished
 */
void CeCWriter::on_minCuis_editingFinished()
{
    ui->minCuis->setValue(roundValueToFive(ui->minCuis->value()));
}

/**
 * @brief CeCWriter::on_minRep_editingFinished
 */
void CeCWriter::on_minRep_editingFinished()
{
    ui->minRep->setValue(roundValueToFive(ui->minRep->value()));
}

/**
 * @brief CeCWriter::isReadyTosend
 * @return true or false
 * Check if enough information is given for the recipe. If yes, say true, else say false
 */
bool CeCWriter::isReadyTosend()
{
    int id1 = 0;
    bool oneCat = false;
    while (!oneCat && id1 < categories.keys().size())
    {
        if (categories.keys().at(id1)->isChecked())
        {
            oneCat = true;
        }
        id1++;
    }
    if (ui->titre->text() != "" && oneCat && imgFile != "" && (ui->hPrep->value() != 0 || ui->minPrep->value() != 0) && ui->description->toPlainText() != "" && model1->rowCount() > 0 && model3->rowCount() > 0)
    {
        return true;
    }
    else
    {
        QMessageBox::critical(this, "Tout n'est pas prêt !", "L'ensemble des paramètres requis de la recette ne sont pas renseignés. Veullez les renseigner.\nParamètres requis :\n      - Titre\n      - Catégorie(s)\n      - Image principale\n      - Temps de préparation\n      - Nombre de personnes\n      - Description\n      - Ingrédients\n      - Instructions de préparation", QMessageBox::Ok);
        return false;
    }
}

/**
 * @brief CeCWriter::on_actionEnvoyer_triggered
 * When "Envoyer" is triggered in the menu, we do the same as the Envoyer button
 */
void CeCWriter::on_actionEnvoyer_triggered()
{
    on_envoyer_clicked();
}

/**
 * @brief CeCWriter::on_envoyer_clicked
 * Open dialog to choose automatic or manual send, if isReadyTosend returns true
 */
void CeCWriter::on_envoyer_clicked()
{
    //Send only if all required fields ar filled:
    if (isReadyTosend())
    {
        makeHtmlCode();
        if (sendAuto) {
            sendAutomatic();
        }
        else {
            sendManual();
        }
    }
}

void CeCWriter::sendAutomatic()
{
    SendAutomatic *sendAutomatic = (SendAutomatic*) ptr2sendAutomatic;
    QStringList cats = Functions::getSelectedCategories(categories);
    QList<int> tpsPrep, tpsCuis, tpsRep;
    tpsPrep << ui->hPrep->value() << ui->minPrep->value();
    tpsCuis << ui->hCuis->value() << ui->minCuis->value();
    tpsRep << ui->jRep->value() << ui->hRep->value() << ui->minRep->value();
    sendAutomatic->init(htmlCode, ui->titre->text(), cats, tpsPrep, tpsCuis, tpsRep, imgFile, excerpt);
}

void CeCWriter::sendManual()
{
    SendManual* sendManual = (SendManual*) ptr2sendManual;
    QStringList imagesToAdd;
    imagesToAdd.append(imgFile);
    foreach (QString pict, otherPicts) {
        imagesToAdd.append(pict);
    }
    QStringList cats = Functions::getSelectedCategories(categories);
    sendManual->init(ui->titre->text(), htmlCode, excerpt, imagesToAdd, cats);
}

/**
 * @brief CeCWriter::makeHtmlCode
 * Gets all data from the recipe and sends it to Function::generateHtmlCode.
 * Store result in htmlCode
 */
void CeCWriter::makeHtmlCode()
{
    otherPicts.clear();
    QStringList allIngr;
    for (int it = 0; it < model1->rowCount(); ++it) {
        allIngr.append(model1->item(it)->text());
    }
    QString ingredients = "";
    if (allIngr.size() > 0)
        ingredients = Functions::getSimpleListWithSubLists(allIngr);
    QStringList allMat;
    for (int it = 0; it < model2->rowCount(); ++it) {
        allMat.append(model2->item(it)->text());
    }
    QString materiel = "";
    if (allMat.size() != 0)
        materiel = Functions::getSimpleList(allMat);
    QStringList allPrep;
    for (int it = 0; it < model3->rowCount(); ++it) {
        allPrep.append(model3->item(it)->text());
    }
    QString preparation = "";
    if (allPrep.size() > 0)
        preparation = Functions::getNumberedList(allPrep);
    QStringList allCons;
    for (int it = 0; it < model4->rowCount(); ++it) {
        allCons.append(model4->item(it)->text());
    }
    QString conseils = "";
    if (allCons.size() > 0)
        conseils = Functions::getSimpleList(allCons);
    QString description = Functions::insertLinks(ui->description->toPlainText());
    QString descTmp = description;
    QRegExp exp ("<a href=\".+\" target=\".+\">");
    descTmp.replace(exp, "");
    descTmp.replace("</a>", "");
    QStringList descWords;
    descWords = descTmp.split(" ");
    QStringList stringTemps = Functions::makeTimes(ui->hPrep->value(), ui->minPrep->value(),
                                                   ui->hCuis->value(), ui->minCuis->value(),
                                                   ui->jRep->value(), ui->hRep->value(), ui->minRep->value());
    excerpt = Functions::makeExcerpt(descWords, stringTemps[0], stringTemps[1], stringTemps[2]);
    excerpt = excerpt.replace("\n", " ");
    description = description.replace("\n", "</p><p>");
    description = "<p>" + description + "</p>";
    htmlCode = Functions::generateHtmlCode(ui->titre->text(), imgFileName, ui->hPrep->value(), ui->minPrep->value(),
                                                   ui->hCuis->value(), ui->minCuis->value(), ui->jRep->value(), ui->hRep->value(),
                                                   ui->minRep->value(), ui->nbPersonnes->value(), ui->precision->text(),
                                                   description, ingredients, materiel, preparation, conseils);
}

/**
 * @brief CeCWriter::on_actionApercu_triggered
 * When "Aperçu" is triggered in the menu, we do the same as the Apercu button
 */
void CeCWriter::on_actionApercu_triggered()
{
    on_apercu_clicked();
}

/**
 * @brief CeCWriter::on_apercu_clicked
 * If isReadyTosend returns true, open an "Aperçu" of the recipe, locally in the browser
 */
void CeCWriter::on_apercu_clicked()
{
    if (isReadyTosend())
    {
        makeHtmlCode();
        QDate today = QDate::currentDate();
        QString month;
        switch (today.month()) {
        case 1:
            month="jan";
            break;
        case 2:
            month="fév";
            break;
        case 3:
            month="mars";
            break;
        case 4:
            month="avril";
            break;
        case 5:
            month="mai";
            break;
        case 6:
            month="juin";
            break;
        case 7:
            month="juil";
            break;
        case 8:
            month="aoüt";
            break;
        case 9:
            month="sept";
            break;
        case 10:
            month="oct";
            break;
        case 11:
            month="nov";
            break;
        case 12:
            month="déc";
            break;
        default:
            month="";
            break;
        }
        QString date = QString::number(today.day())+" "+month+", "+QString::number(today.year());
        if (today.day() < 10)
            date = "0" + date;
        QString path = QCoreApplication::applicationDirPath();
        QDir tmp (dirTmp + "/apercu");
        bool success = true;
        if (tmp.exists())
            success = Functions::removeDir(dirTmp + "/apercu");
        if (success)
        {
            success = Functions::copyRecursively(path + "/modeleHtml", dirTmp + "/apercu");
            if (success)
            {
                tmp.mkdir("./Photos");
                QFile rctFile (dirTmp + "/apercu/modele_recette.html");
                rctFile.open(QFile::ReadOnly);
                QTextStream in(&rctFile);
                in.setCodec("UTF-8"); //Not useful on linux system as it's a default, but Windows has its own defaults....
                QString page = in.readAll();
                rctFile.close();
                QStringList catsSelected;
                foreach (QCheckBox *cat, categories.keys()) {
                    if (cat->isChecked())
                    {
                        catsSelected.append(categories[cat]);
                    }
                }
                catsSelected.sort();
                QString cats = "<a href=\"#\">"+catsSelected.at(0)+"</a>";
                if (catsSelected.size() > 1) {
                    for (int cat=1; cat<catsSelected.size(); ++cat)
                        cats += ", <a href=\"#\">"+catsSelected.at(cat)+"</a>";
                }
                page = page.replace("[!TITRE!]",ui->titre->text());
                page = page.replace("[!AUTEUR!]",pseudoWp);
                page = page.replace("[!CONTENU!]",htmlCode);
                page = page.replace("[!DATE!]",date);
                page = page.replace("[!CATEGORIES!]",cats);
                QFile indexFile (dirTmp + "/apercu/index.html");
                indexFile.open(QFile::WriteOnly);
                QTextStream in1(&indexFile);
                in1.setCodec("UTF-8"); //Not useful on linux system as it's a default, but Windows has its own defaults....
                in1 << page << endl;
                indexFile.close();
                QFile::copy(imgFile, dirTmp + "/apercu/Photos/" + imgFileName);
                if (otherPicts.size() > 0)
                {
                    foreach (QString img, otherPicts) {
                        QString imgName = img.right(img.length() - img.lastIndexOf("/"));
                        QFile::copy(img, dirTmp + "/apercu/Photos/" + imgName);
                    }
                }
                QString Program = "\"" + cmdNav + "\" " + dirTmp + "/apercu/index.html";
                #ifndef  Q_OS_LINUX
                Program.replace("/", "\\");
                #endif
                QProcess *myProcess = new QProcess();
                myProcess->setProcessChannelMode(QProcess::MergedChannels);
                myProcess->start(Program);
            }
            else
            {
                QMessageBox::critical(this, "Erreur lors de la création de l'aperçu", "Erreur : impossible de copier les fichiers dans le dossier temporaire. Veuillez contacter le support.", QMessageBox::Ok);
            }
        }
        else
        {
            QMessageBox::critical(this, "Erreur lors de la création de l'aperçu", "Erreur : impossible de créer le dossier temporaire. Veuillez contacter le support.", QMessageBox::Ok);
        }
    }
}

/**
 * @brief CeCWriter::on_actionEnregistrer_sous_triggered
 * When "Enregistrer sous" menu is triggered, open dialog for choosing FileName and save recipe with it
 */
void CeCWriter::on_actionEnregistrer_sous_triggered()
{
    doSave = false;
    chooseSaveFileName = new QDialog(this);
    chooseSaveFileName->setWindowTitle("SAUVEGARDER");
    QVBoxLayout *vlay = new QVBoxLayout();
    QLabel *txt = new QLabel("Nom de votre sauvegarde :");
    txt->setMinimumHeight(30);
    chooseSaveFN = new QLineEdit(ui->titre->text());
    chooseSaveFN->setMinimumHeight(30);
    vlay->addWidget(txt);
    vlay->addWidget(chooseSaveFN);
    QWidget *buttons = new QWidget();
    QPushButton *valider = new QPushButton("Valider");
    connect(valider, SIGNAL(released()), this, SLOT(handleSaveValider()));
    valider->setMinimumSize(100, 30);
    valider->setMaximumSize(100,30);
    QPushButton *annuler = new QPushButton("Annuler");
    connect(annuler, SIGNAL(released()), this, SLOT(handleSaveAnnuler()));
    annuler->setMinimumSize(100, 30);
    annuler->setMaximumSize(100,30);
    QHBoxLayout *butLay = new QHBoxLayout(buttons);
    butLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
    butLay->addWidget(valider);
    butLay->addWidget(annuler);
    vlay->addWidget(buttons);
    chooseSaveFileName->setLayout(vlay);
    chooseSaveFileName->setFixedHeight(chooseSaveFileName->sizeHint().height());
    chooseSaveFileName->setMinimumWidth(450);
    chooseSaveFileName->exec();
    if (doSave)
    {
        on_enregistrer_clicked();
    }
}

/**
 * @brief CeCWriter::on_actionEnregistrer_triggered
 * When "Enregistrer" is triggered on the menu, we do the same as the Enregistrer button
 */
void CeCWriter::on_actionEnregistrer_triggered()
{
    on_enregistrer_clicked();
}

/**
 * @brief CeCWriter::on_enregistrer_clicked
 * Do Enregistrer : show dialog for choose fileName only if saveFileName is empty
 */
void CeCWriter::on_enregistrer_clicked()
{
    doSave = true;
    if (saveFileName == "")
    {
        doSave = false;
        chooseSaveFileName = new QDialog(this);
        chooseSaveFileName->setWindowTitle("SAUVEGARDER");
        QVBoxLayout *vlay = new QVBoxLayout();
        QLabel *txt = new QLabel("Nom de votre sauvegarde :");
        txt->setMinimumHeight(30);
        chooseSaveFN = new QLineEdit(ui->titre->text());
        chooseSaveFN->setMinimumHeight(30);
        vlay->addWidget(txt);
        vlay->addWidget(chooseSaveFN);
        QWidget *buttons = new QWidget();
        QPushButton *valider = new QPushButton("Valider");
        connect(valider, SIGNAL(released()), this, SLOT(handleSaveValider()));
        valider->setMinimumSize(100, 30);
        valider->setMaximumSize(100,30);
        QPushButton *annuler = new QPushButton("Annuler");
        connect(annuler, SIGNAL(released()), this, SLOT(handleSaveAnnuler()));
        annuler->setMinimumSize(100, 30);
        annuler->setMaximumSize(100,30);
        QHBoxLayout *butLay = new QHBoxLayout(buttons);
        butLay->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
        butLay->addWidget(valider);
        butLay->addWidget(annuler);
        vlay->addWidget(buttons);
        chooseSaveFileName->setLayout(vlay);
        chooseSaveFileName->setFixedHeight(chooseSaveFileName->sizeHint().height());
        chooseSaveFileName->setMinimumWidth(450);
        chooseSaveFileName->exec();
    }
    if (doSave)
    {
        //Categories:
        QStringList catsSelected;
        foreach (QCheckBox *catCheckBox, categories.keys()) {
            if (catCheckBox->isChecked()) {
                catsSelected.append(categories[catCheckBox]);
            }
        }
        //Temps prep:
        QString tpsPrep = QString::number(ui->hPrep->value()) + "h" + QString::number(ui->minPrep->value());
        QString tpsCuis = QString::number(ui->hCuis->value()) + "h" + QString::number(ui->minCuis->value());
        QString tpsRep = QString::number(ui->jRep->value()) + "j" + QString::number(ui->hRep->value()) + "h" + QString::number(ui->minRep->value());

        QStringList ingrList;
        for (int i1 = 0; i1 < model1->rowCount(); ++i1) {
            ingrList.append(model1->item(i1)->text());
        }
        QStringList matList;
        for (int i1 = 0; i1 < model2->rowCount(); ++i1) {
            matList.append(model2->item(i1)->text());
        }
        QStringList prepList;
        for (int i1 = 0; i1 < model3->rowCount(); ++i1) {
            prepList.append(model3->item(i1)->text());
        }
        QStringList consList;
        for (int i1 = 0; i1 < model4->rowCount(); ++i1) {
            consList.append(model4->item(i1)->text());
        }

        bool success = Functions::saveRecipe(ui->titre->text(), catsSelected, tpsPrep, tpsCuis,
                                             tpsRep, QString::number(ui->nbPersonnes->value()), ui->precision->text(),
                                             ui->description->toPlainText(), ingrList, matList, prepList, consList,
                                             imgFile, liens, saveFileName);
        if (success)
        {
            saveVariables(ingrList.join("\n"), matList.join("\n"), prepList.join("\n"), consList.join("\n"));
            ui->state->setText("Enregistrement terminé !");
            QTimer::singleShot(4000, this, SLOT(refreshState()));
        }
    }
}

/**
 * @brief CeCWriter::saveVariables
 * @param ingr : ingrédients
 * @param mat : matériel
 * @param prep : préparation
 * @param cons : conseils
 * Stock saved values. Thay are compared to current values when user want to exit the recipe
 */
void CeCWriter::saveVariables(QString ingr, QString mat, QString prep, QString cons)
{
    saveTitre = ui->titre->text();
    saveCategories.clear();
    foreach (QCheckBox *catCheckBox, categories.keys()) {
        if (catCheckBox->isChecked()) {
            saveCategories.append(categories[catCheckBox]);
        }
    }
    saveMainImage = imgFile;
    saveHPrep = ui->hPrep->value();
    saveMinPrep = ui->minPrep->value();
    saveHCuis = ui->hCuis->value();
    saveMinCuis = ui->minCuis->value();
    saveJRep = ui->jRep->value();
    saveHRep = ui->hRep->value();
    saveMinRep = ui->minRep->value();
    saveNbPersonnes = ui->nbPersonnes->value();
    savePrecision = ui->precision->text();
    saveDescription = ui->description->toPlainText();
    saveIngredients = ingr;
    saveMateriel = mat;
    savePreparation = prep;
    saveConseils = cons;
}

/**
 * @brief CeCWriter::on_actionOuvrir_une_recette_existante_triggered
 * Open a previously saved recipe
 */
void CeCWriter::on_actionOuvrir_une_recette_existante_triggered()
{
    bool doOpen = true;
    if (checkHasBeenModified())
    {
        int rep = QMessageBox::warning(this, "Enregistrer avant de quitter ?", "La recette a été modifiée. Voulez-vous enregistrer la recette avant de continuer ?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if (rep == QMessageBox::Yes)
        {
            on_enregistrer_clicked();
        }
        else if (rep == QMessageBox::Cancel)
        {
            doOpen = false;
        }
    }
    if (doOpen)
    {
        resetFields();
        QString fileName = QFileDialog::getOpenFileName(this, "Ouvrir une recette",
        dirSav,
        "Recettes : *.rct (*.rct)");
        QStringList ingrdt, matl, prept, consl;
        if ( !fileName.isEmpty() )
        {
            QMap<QString, QStringList> rct = Functions::loadRecipe(fileName);
            if (rct.size() > 0)
            {
                //Load titre:
                if (rct.keys().contains("titre"))
                    ui->titre->setText(rct["titre"].at(0));

                //Load categories:
                if (rct.keys().contains("categories"))
                {
                    foreach (QString cat, rct["categories"]) {
                        foreach (QCheckBox* itemC, categories.keys()) {
                            if (categories[itemC] == cat) {
                                itemC->setChecked(true);
                            }
                        }
                    }
                }

                //Load image:
                if (rct.keys().contains("image"))
                {
                    if(rct["image"].at(0) != "")
                    {
                        imgFile = rct["image"].at(0);
                        if (QFile(imgFile).exists())
                        {
                            imgFileName = imgFile.split("/").last();
                            ui->mainPicture->setText(imgFileName);
                            ui->mainPicture->setIcon(QPixmap(imgFile));
                        }
                        else
                        {
                            imgFile = "";
                        }
                    }
                }

                //Load tpsPrep:
                if (rct.keys().contains("tpsPrep"))
                {
                    ui->hPrep->setValue(rct["tpsPrep"].at(0).toInt());
                    ui->minPrep->setValue(rct["tpsPrep"].at(1).toInt());
                }

                //Load tpsCuis:
                if (rct.keys().contains("tpsCuis"))
                {
                    ui->hCuis->setValue(rct["tpsCuis"].at(0).toInt());
                    ui->minCuis->setValue(rct["tpsCuis"].at(1).toInt());
                }

                //Load tpsRep:
                if (rct.keys().contains("tpsRep"))
                {
                    ui->jRep->setValue(rct["tpsRep"].at(0).toInt());
                    ui->hRep->setValue(rct["tpsRep"].at(1).toInt());
                    ui->minRep->setValue(rct["tpsRep"].at(2).toInt());
                }

                //Load nb personnes:
                if (rct.keys().contains("nbPers"))
                {
                    ui->nbPersonnes->setValue(rct["nbPers"].at(0).toInt());
                }

                //Load precision:
                if (rct.keys().contains("precision"))
                {
                    ui->precision->setText(rct["precision"].at(0));
                }

                if (rct.keys().contains("description"))
                {
                    ui->description->setPlainText(rct["description"].at(0));
                }

                //Load ingredients:
                if (rct.keys().contains("ingredients"))
                {
                    ingrdt = rct["ingredients"];
                    foreach (QString ingr, ingrdt) {
                        model1->appendRow(new QStandardItem(ingr));
                    }
                }

                ui->state->setText("Recette chargée !");
                QTimer::singleShot(4000, this, SLOT(refreshState()));

                //Load materiel:
                if (rct.keys().contains("materiel"))
                {
                    matl = rct["materiel"];
                    foreach (QString mat, matl) {
                        model2->appendRow(new QStandardItem(mat));
                    }
                }

                //Load preparation:
                if (rct.keys().contains("preparation"))
                {
                    prept = rct["preparation"];
                    foreach (QString prep, prept) {
                        model3->appendRow(new QStandardItem(prep));
                    }
                    idPrep = prept.last().split("|")[0].split(".")[0] + ".0";
                }

                //Load conseils:
                if (rct.keys().contains("conseils"))
                {
                    consl = rct["conseils"];
                    foreach (QString cons, consl) {
                        model4->appendRow(new QStandardItem(cons));
                    }
                }

                ui->state->setText("Recette chargée !");
                QTimer::singleShot(4000, this, SLOT(refreshState()));

                ui->state->setText("Recette chargée !");
                QTimer::singleShot(4000, this, SLOT(refreshState()));

                ui->state->setText("Recette chargée !");
                QTimer::singleShot(4000, this, SLOT(refreshState()));

                //Load liens:
                if(rct.keys().contains("liens"))
                {
                    foreach (QString lien, rct["liens"]) {
                        liens.insert("L" + QString::number(idLien), lien);
                        idLien++;
                    }
                }
                fileName = fileName.split("/").last();
                saveFileName = fileName.left(fileName.length() - 4);
                saveVariables(ingrdt.join("\n"), matl.join("\n"), prept.join("\n"), consl.join("\n"));
                if (imgFile != "")
                {
                    ui->editPicture->setEnabled(true);
                }
            }
        }
    }
}

/**
 * @brief CeCWriter::on_actionNouvelle_Recette_triggered
 * Open a new recipe
 */
void CeCWriter::on_actionNouvelle_Recette_triggered()
{
    if (checkHasBeenModified())
    {
        int rep = QMessageBox::warning(this, "Enregistrer avant de quitter ?", "La recette a été modifiée. Voulez-vous enregistrer la recette avant de continuer ?", QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
        if (rep == QMessageBox::Yes)
        {
            on_enregistrer_clicked();
            if (!checkHasBeenModified())
            {
                resetFields();
                ui->titre->setFocus();
            }
        }
        else if (rep == QMessageBox::No)
        {
            resetFields();
            ui->titre->setFocus();
        }
    }
    else
    {
        resetFields();

        ui->titre->setFocus();
    }
}

/**
 * @brief CeCWriter::on_actionAide_triggered
 * When Aide is triggered on menu, we open help in browser
 */
void CeCWriter::on_actionAide_triggered()
{
    QString Program = "\"" + cmdNav + "\" http://QCeCWriter.conseilsencuisine.fr/help.php";
    QProcess *myProcess = new QProcess();
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(Program);
}

/**
 * @brief CeCWriter::openEditor
 * Open saved image in advanced editor, as defined in options
 */
void CeCWriter::openEditor()
{
    QString Program = editPict + " " + imgFile;
    #ifndef  Q_OS_LINUX
    Program.replace("/", "\\");
    #endif
    QProcess *myProcess = new QProcess();
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(Program);
}

/**
 * @brief CeCWriter::on_actionRechercher_une_mise_jour_triggered
 * Search for an update of the program, when it is triggered from the menu
 */
void CeCWriter::on_actionRechercher_une_mise_jour_triggered()
{
    FileDownloader *fdower = new FileDownloader("http://qcecwriter.conseilsencuisine.fr/files/LATEST-" + systExp
                                                + "-QT-" + QTVERSION, "Checking for update..." ,this);
    QByteArray resFile = fdower->downloadedData();
    if (!resFile.isEmpty())
    {
        if (resFile.left(9) != "<!DOCTYPE")
        {
            QString content (resFile);
            QStringList lines = content.split("\n");
            QString latestVersion = lines[0].left(lines[0].length() - 1);
            QStringList lVerNb = latestVersion.split(".");
            QStringList tVerNb = VERSION.split(".");
            bool hasUpdate = false;
            int niv = 0;
            while (!hasUpdate && niv < qMin(lVerNb.size(), tVerNb.size()))
            {
                if (lVerNb[niv] > tVerNb[niv])
                    hasUpdate = true;
                niv++;
            }
            if (!hasUpdate && lVerNb.size() > niv)
                hasUpdate = true;
            if (hasUpdate)
            {
                //Propose to user downloading the last version
                int rep = QMessageBox::information(this, "Mise à jour disponible", QString("Une mise à jour du logicielle est disponible, voulez-vous ")
                                                   + QString("mettre à jour votre version ") + VERSION + QString(" à la dernière version (" + latestVersion + ") ?"), QMessageBox::Yes, QMessageBox::No);
                if (rep == QMessageBox::Yes)
                {
                    QString adresse = lines[1].left(lines[1].length() - 1);
                    QString fileToSave = adresse.split("/").last();
                    QString extOfFile = fileToSave.split(".").last();
                    QString fileToSaveUser = QFileDialog::getSaveFileName(this, "Enregistrer l'archive sous...", userDir + "/" + fileToSave, "Package d'installation  : *."+extOfFile+" (*."+extOfFile+")");
                    if (fileToSaveUser != "")
                    {
                        fdower = new FileDownloader(adresse, "Téléchargement de la mise à jour..." ,this);
                        QByteArray package = fdower->downloadedData();
                        QFile saveFile(fileToSaveUser);
                        saveFile.open(QIODevice::WriteOnly);
                        saveFile.write(package);
                        saveFile.close();
                        QMessageBox::information(this, "Téléchargement terminé !", "Vous pouvez maintenant installer\nle package d'installation téléchargé !", QMessageBox::Ok);
                    }

                }
            }
            else
            {
                QMessageBox::information(this, "Aucune mise à jour disponible", "Vous possédez la dernière version du logiciel !", QMessageBox::Ok);
            }
        }
        else
        {
            QMessageBox::critical(this, "Erreur !", "Une erreur est survenue lors de la recherche de la mise à jour\nVeuillez contacter le support.",
                                          QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::critical(this, "Erreur !", "Impossible de déterminer la dernière version du logiciel.\nVeuillez vérifier votre connexion internet.",
                              QMessageBox::Ok);
    }

}

//Events config:

/**
 * @brief CeCWriter::eventFilter
 * @param object : the object that send the event
 * @param event : the event
 * @return bool : true or false
 * Main event processing function
 */
bool CeCWriter::eventFilter(QObject *object, QEvent *event)
{
    if (object == this && event->type() == QEvent::FocusIn)
    {
        ui->mainPicture->setIcon(QPixmap(imgFile));
    }
    //editPicture:
    if (object == ui->editPicture && ui->editPicture->isEnabled())
    {
        if (event->type() == QEvent::Enter)
        {
            ui->editPicture->setPixmap(QPixmap(":/images/editPict_over.png"));
        }
        else if (event->type() == QEvent::Leave)
        {
            ui->editPicture->setPixmap(QPixmap(":/images/editPict.png"));
        }
        else if (event->type() == QEvent::MouseButtonPress)
        {
            ui->editPicture->setPixmap(QPixmap(":/images/editPict_click.png"));
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            openEditor();
            ui->editPicture->setPixmap(QPixmap(":/images/editPict_over.png"));
        }
    }
    //Focus comm:
    if ((object == ui->description || object == ui->editIngr || object == ui->editMat || object == ui->editPrep || object == ui->editCons) && event->type() == QEvent::FocusOut)
    {
        ui->grasButton->setEnabled(false);
        ui->italicButton->setEnabled(false);
        ui->soulignButton->setEnabled(false);
        ui->noPrint->setEnabled(false);
        ui->printOnly->setEnabled(false);
        if (object != ui->description) {
            ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
            ui->commButton->setEnabled(false);
        }
        if (object == ui->editPrep || object == ui->editCons || object == ui->description) {
            ui->lienButton->setDisabled(true);
            if (object != ui->description) {
                ui->imgButton->setEnabled(false);
                ui->movie->setEnabled(false);
            }
        }
        return false;
    }
    else if (object == ui->editIngr && event->type() == QEvent::FocusIn)
    {
        ui->commButton->setEnabled(true);
        ui->grasButton->setEnabled(true);
        ui->italicButton->setEnabled(true);
        ui->soulignButton->setEnabled(true);
        if (cecPrinter) {
            ui->noPrint->setEnabled(true);
            ui->printOnly->setEnabled(true);
        }
        if (ingrComm)
            ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
        else
            ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
        return false;
    }
    else if (object == ui->editMat && event->type() == QEvent::FocusIn)
    {
        ui->commButton->setEnabled(true);
        ui->grasButton->setEnabled(true);
        ui->italicButton->setEnabled(true);
        ui->soulignButton->setEnabled(true);
        if (cecPrinter) {
            ui->noPrint->setEnabled(true);
            ui->printOnly->setEnabled(true);
        }
        if (matComm)
            ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
        else
            ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
        return false;
    }
    else if (object == ui->editPrep && event->type() == QEvent::FocusIn)
    {
        ui->commButton->setEnabled(true);
        ui->imgButton->setEnabled(true);
        ui->movie->setEnabled(true);
        ui->grasButton->setEnabled(true);
        ui->italicButton->setEnabled(true);
        ui->soulignButton->setEnabled(true);
        if (cecPrinter) {
            ui->noPrint->setEnabled(true);
            ui->printOnly->setEnabled(true);
        }
        if (ui->editPrep->textCursor().selectedText().length() > 0) {
            ui->lienButton->setEnabled(true);
        }
        if (prepComm)
            ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
        else
            ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
        return false;
    }
    else if (object == ui->editCons && event->type() == QEvent::FocusIn)
    {
        ui->commButton->setEnabled(true);
        ui->imgButton->setEnabled(true);
        ui->movie->setEnabled(true);
        ui->grasButton->setEnabled(true);
        ui->italicButton->setEnabled(true);
        ui->soulignButton->setEnabled(true);
        if (cecPrinter) {
            ui->noPrint->setEnabled(true);
            ui->printOnly->setEnabled(true);
        }
        if (ui->editCons->textCursor().selectedText().length() > 0) {
            ui->lienButton->setEnabled(true);
        }
        if (consComm)
            ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
        else
            ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
        return false;
    }
    else if (object == ui->description && event->type() == QEvent::FocusIn) {
        ui->grasButton->setEnabled(true);
        ui->italicButton->setEnabled(true);
        ui->soulignButton->setEnabled(true);
        if (cecPrinter) {
            ui->noPrint->setEnabled(true);
            ui->printOnly->setEnabled(true);
        }
        if (ui->description->textCursor().selectedText().length() > 0) {
            ui->lienButton->setEnabled(true);
        }
        return false;
    }
    //Ingredients:
    else if (object == ui->editIngr && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter)
        {
            this->insertIngredient(ui->editIngr->text());
            ui->editIngr->setText("");
            return true;
        }
        else if (ke->key() == Qt::Key_Escape)
        {
            if (ingrEdit != -1)
            {
                if (model1->item(ingrEdit)->text().split("|").at(1) != "")
                {
                    model1->setItem(ingrEdit, new QStandardItem(model1->item(ingrEdit)->text()));
                    ingrEdit = -1;
                    ui->editIngr->clear();
                }
                else
                {
                    model1->removeRow(ingrEdit);
                    ingrEdit = -1;
                    ui->editIngr->clear();
                }
                idIngr = model1->item(model1->rowCount() - 1)->text().split("|").at(0).toInt();
                ui->ingrListShow->setText(QString::number(idIngr));
                ui->listIngr->setSelectionMode(QAbstractItemView::ExtendedSelection);
                ingrComm = false;
                ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
            }
            return true;
        }
        else if (ke->key() == Qt::Key_Backspace)
        {
            if (ui->editIngr->selectionStart() == -1)
                checkRemoveBalBckpInLineEdit(ui->editIngr);
            else
                checkRemoveBalSlctInLineEdit(ui->editIngr);
            if (balise != "" || balises.size() > 0)
                return true;
            return false;
        }
        else if (ke->key() == Qt::Key_Delete)
        {
            if (ui->editIngr->selectionStart() == -1)
                checkRemoveBalSupprInLineEdit(ui->editIngr);
            else
                checkRemoveBalSlctInLineEdit(ui->editIngr);
            if (balise != "" || balises.size() > 0)
                return true;
            return false;
        }
        return false;
    }
    else if (object == ui->editIngr && event->type() == QEvent::KeyRelease) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Backspace || ke->key() == Qt::Key_Delete)
        {
            if (balise != "")
                deleteBalInLineEdit(ui->editIngr);
            else if (balises.size() > 0)
                deleteBalsInLineEdit(ui->editIngr);
        }
        return false;
    }
    else if (object == ui->listIngr && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Delete || ke->key() == Qt::Key_Backspace)
        {
            supprimerIngr();
            return true;
        }
        return false;
    }

    //Materiel:
    else if (object == ui->editMat && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter)
        {
            this->insertMateriel(ui->editMat->text());
            ui->editMat->setText("");
            return true;
        }
        else if (ke->key() == Qt::Key_Escape)
        {
            if (matEdit != -1)
            {
                if (model2->item(matEdit)->text().split("|").at(1) != "")
                {
                    model2->setItem(matEdit, new QStandardItem(model2->item(matEdit)->text()));
                    matEdit = -1;
                    ui->editMat->clear();
                }
                else
                {
                    model2->removeRow(matEdit);
                    matEdit = -1;
                    ui->editMat->clear();
                }
                matComm = false;
                ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
            }
            ui->listMat->setSelectionMode(QAbstractItemView::ExtendedSelection);
            return true;
        }
        else if (ke->key() == Qt::Key_Backspace)
        {
            if (ui->editMat->selectionStart() == -1)
                checkRemoveBalBckpInLineEdit(ui->editMat);
            else
                checkRemoveBalSlctInLineEdit(ui->editMat);
            if (balise != "" || balises.size() > 0)
                return true;
            return false;
        }
        else if (ke->key() == Qt::Key_Delete)
        {
            if (ui->editMat->selectionStart() == -1)
                checkRemoveBalSupprInLineEdit(ui->editMat);
            else
                checkRemoveBalSlctInLineEdit(ui->editMat);
            if (balise != "" || balises.size() > 0)
                return true;
            return false;
        }
        return false;
    }
    else if (object == ui->editMat && event->type() == QEvent::KeyRelease) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Backspace || ke->key() == Qt::Key_Delete)
        {
            if (balise != "")
                deleteBalInLineEdit(ui->editMat);
            else if (balises.size() > 0)
                deleteBalsInLineEdit(ui->editMat);
        }
        return false;
    }
    else if (object == ui->listMat && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Delete || ke->key() == Qt::Key_Backspace)
        {
            supprimerMat();
            return true;
        }
        return false;
    }

    //Preparation:
    else if (object == ui->editPrep && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter)
        {
            this->insertPreparation(ui->editPrep->toPlainText());
            ui->editPrep->setPlainText("");
            return true;
        }
        else if (ke->key() == Qt::Key_Escape && prepEdit != -1)
        {
            if (model3->item(prepEdit)->text().split("|").at(1) != "")
            {
                model3->setItem(prepEdit, new QStandardItem(model3->item(prepEdit)->text()));
                prepEdit = -1;
                ui->editPrep->clear();
            }
            else
            {
                //Update next puces in loop:
                int nbParts = model3->item(prepEdit)->text().split("|").at(0).split(".").size();
                bool endLoop = false;
                int idP = prepEdit + 1;
                while (idP < model3->rowCount() && !endLoop) {
                    QString itP = model3->item(idP)->text();
                    QStringList partsP = itP.split("|");
                    QString puce = partsP[0];
                    QStringList pP = puce.split(".");
                    if (pP.size() == nbParts)
                    {
                        pP[pP.size() - 2] = QString::number(pP[pP.size() - 2].toInt() - 1);
                        puce = pP.join(".");
                        model3->setItem(idP, new QStandardItem(puce + "|" + partsP[1]));
                    }
                    else if (pP.size() < nbParts)
                    {
                        endLoop = true;
                    }
                    idP++;
                }
                model3->removeRow(prepEdit);
                prepEdit = -1;
                ui->editPrep->clear();
            }
            idPrep = model3->item(model3->rowCount() - 1)->text().split("|").at(0);
            if (idPrep == "comm")
                idPrep = "0.0";
            ui->prepListShow->setText(QString::number(idPrep.split(".").size() - 2));
            ui->listPrep->setSelectionMode(QAbstractItemView::ExtendedSelection);
            prepComm = false;
            ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
            return true;
        }
        else if (ke->key() == Qt::Key_Backspace)
        {
            if (ui->editPrep->textCursor().selectedText() == "")
                checkRemoveBalBckpInPlainTextEdit(ui->editPrep);
            else
                checkRemoveBalSlctInPlainTextEdit(ui->editPrep);
            if (balise != "" || balises.size() > 0)
                return true;
            return false;
        }
        else if (ke->key() == Qt::Key_Delete)
        {
            if (ui->editPrep->textCursor().selectedText() == "")
                checkRemoveBalSupprInPlainTextEdit(ui->editPrep);
            else
                checkRemoveBalSlctInPlainTextEdit(ui->editPrep);
            if (balise != "" || balises.size() > 0)
                return true;
            return false;
        }
        return false;
    }
    else if (object == ui->editPrep && event->type() == QEvent::KeyRelease) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Backspace || ke->key() == Qt::Key_Delete)
        {
            if (balise != "")
                deleteBalInPlainTextEdit(ui->editPrep);
            else if (balises.size() > 0)
                deleteBalsInPlainTextEdit(ui->editPrep);
        }
        return false;
    }
    else if (object == ui->listPrep && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Delete || ke->key() == Qt::Key_Backspace)
        {
            supprimerPrep();
            return true;
        }
        return false;
    }

    //Conseils:
    else if (object == ui->editCons && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Return || ke->key() == Qt::Key_Enter)
        {
            this->insertConseil(ui->editCons->toPlainText());
            ui->editCons->setPlainText("");
            return true;
        }
        else if (ke->key() == Qt::Key_Escape)
        {
            if (consEdit != -1)
            {
                if (model4->item(consEdit)->text().split("|").at(1) != "")
                {
                    model4->setItem(consEdit, new QStandardItem(model4->item(consEdit)->text()));
                    consEdit = -1;
                    ui->editMat->clear();
                }
                else
                {
                    model4->removeRow(consEdit);
                    consEdit = -1;
                    ui->editMat->clear();
                }
                matComm = false;
                ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
            }
            ui->listMat->setSelectionMode(QAbstractItemView::ExtendedSelection);
            return true;
        }
        else if (ke->key() == Qt::Key_Backspace)
        {
            if (ui->editCons->textCursor().selectedText() == "")
                checkRemoveBalBckpInPlainTextEdit(ui->editCons);
            else
                checkRemoveBalSlctInPlainTextEdit(ui->editCons);
            if (balise != "" || balises.size() > 0)
                return true;
            return false;
        }
        else if (ke->key() == Qt::Key_Delete)
        {
            if (ui->editCons->textCursor().selectedText() == "")
                checkRemoveBalSupprInPlainTextEdit(ui->editCons);
            else
                checkRemoveBalSlctInPlainTextEdit(ui->editCons);
            if (balise != "" || balises.size() > 0)
                return true;
            return false;
        }
        return false;
    }
    else if (object == ui->editCons && event->type() == QEvent::KeyRelease) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Backspace || ke->key() == Qt::Key_Delete)
        {
            if (balise != "")
                deleteBalInPlainTextEdit(ui->editCons);
            else if (balises.size() > 0)
                deleteBalsInPlainTextEdit(ui->editCons);
        }
        return true;
    }
    else if (object == ui->listCons && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Delete || ke->key() == Qt::Key_Backspace)
        {
            supprimerCons();
            return true;
        }
        return false;
    }
    //Description:
    else if (object == ui->description && event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Backspace)
        {
            if (ui->description->textCursor().selectedText() == "")
                checkRemoveBalBckpInPlainTextEdit(ui->description);
            else
                checkRemoveBalSlctInPlainTextEdit(ui->description);
            if (balise != "" || balises.size() > 0)
                return true;
            return false;
        }
        else if (ke->key() == Qt::Key_Delete)
        {
            if (ui->description->textCursor().selectedText() == "")
                checkRemoveBalSupprInPlainTextEdit(ui->description);
            else
                checkRemoveBalSlctInPlainTextEdit(ui->description);
            if (balise != "" || balises.size() > 0)
                return true;
            return false;
        }
        return false;
    }
    else if (object == ui->description && event->type() == QEvent::KeyRelease) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Backspace || ke->key() == Qt::Key_Delete)
        {
            if (balise != "")
                deleteBalInPlainTextEdit(ui->description);
            else if (balises.size() > 0)
                deleteBalsInPlainTextEdit(ui->description);
        }
        return true;
    }
    else
        return false;
}

/*
 * Insert items in listview
 *
 */

void CeCWriter::insertIngredient(QString text) {
    QStandardItem* itemNew;
    if (!ingrComm)
        itemNew = new QStandardItem(QString::number(idIngr) + "|" + text);
    else
        itemNew = new QStandardItem("comm|" + text);
    if (ingrEdit == -1)
    {
        model1->appendRow(itemNew);
        ui->listIngr->scrollToBottom();
    }
    else
    {
        model1->setItem(ingrEdit, itemNew);
        if (ingrEdit != model1->rowCount() - 1)
        {
            idIngr = model1->item(model1->rowCount() - 1)->text().split("|").at(0).toInt();
            ui->ingrListShow->setText(QString::number(idIngr));
        }
        ingrEdit = -1;
        ui->listIngr->setSelectionMode(QAbstractItemView::ExtendedSelection);
        ingrComm = false;
        ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
    }
}

void CeCWriter::insertMateriel(QString text) {
    QStandardItem* itemNew;
    if (!matComm)
        itemNew = new QStandardItem("0|" + text);
    else
        itemNew = new QStandardItem("comm|" + text);

    if (matEdit == -1)
    {
        model2->appendRow(itemNew);
        ui->listMat->scrollToBottom();
    }
    else
    {
        model2->setItem(matEdit, itemNew);
        matEdit = -1;
        ui->listMat->setSelectionMode(QAbstractItemView::ExtendedSelection);
        matComm = false;
        ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
    }
}

void CeCWriter::insertPreparation(QString text) {
    QStandardItem* itemNew;
    if (!prepComm)
    {
        QStringList nb = idPrep.split('.');
        nb[nb.size() - 2] = QString::number(nb[nb.size() - 2].toInt() + 1);
        idPrep = nb.join(".");
        itemNew = new QStandardItem(idPrep + "|" + text);
    }
    else
    {
        itemNew = new QStandardItem("comm|" + text);
        idPrep = "0.0";
        ui->prepListShow->setText("0");
    }
    if (prepEdit == -1)
    {
        model3->appendRow(itemNew);
        updatePrepItems();
        ui->listPrep->scrollToBottom();
    }
    else
    {
        model3->setItem(prepEdit, itemNew);
        //Update all puces:
        updatePrepItems();
        //End:
        idPrep = model3->item(model3->rowCount() - 1)->text().split("|").at(0);
        if (idPrep == "comm")
            idPrep = "0.0";
        ui->prepListShow->setText(QString::number(idPrep.split(".").size() - 2));
        prepEdit = -1;
        ui->listPrep->setSelectionMode(QAbstractItemView::ExtendedSelection);
        prepComm = false;
        ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
    }
}

void CeCWriter::insertConseil(QString text) {
    QStandardItem* itemNew;
    if (!consComm)
        itemNew = new QStandardItem("0|" + text);
    else
        itemNew = new QStandardItem("comm|" + text);
    if (consEdit == -1)
    {
        model4->appendRow(itemNew);
        ui->listCons->scrollToBottom();
    }
    else
    {
        model4->setItem(consEdit, itemNew);
        consEdit = -1;
        ui->listCons->setSelectionMode(QAbstractItemView::ExtendedSelection);
        consComm = false;
        ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
    }
}

/**
 * @brief CeCWriter::updatePrepItems
 * Update prep items when one item is edited
 */
void CeCWriter::updatePrepItems()
{
    QList<int> idLi;
    for (int i = 0; i <= 4; ++i) {
        idLi.append(0);
    }
    for (int it = 0; it < model3->rowCount(); ++it) {
        QString txt = model3->item(it)->text();
        QStringList parts = txt.split("|");
        QString puce = parts[0];
        if (puce == "comm")
        {
            for (int i = 0; i<= 4; ++i) {
                idLi[i] = 0;
            }
        }
        else {
            QStringList pp = puce.split(".");
            int idE = pp.size() - 2;
            idLi[idE]++;
            for (int it2 = 0; it2 <= qMax(0, idE); ++it2) {
                pp[it2] = QString::number(idLi[it2]);
            }
            puce = pp.join(".");
            model3->setItem(it, new QStandardItem(puce + "|" + parts[1]));
            if (idE != 4)
            {
                for (int suiv = idE + 1; suiv <= 4; ++suiv) {
                    idLi[suiv] = 0;
                }
            }
        }
    }
}

/*
 * Process events when user double-click on an item of a list
 *
*/
void CeCWriter::on_listIngr_doubleClicked(const QModelIndex &index)
{
    if (ingrEdit == -1)
        modifierIngr(index);
}

void CeCWriter::on_listMat_doubleClicked(const QModelIndex &index)
{
    if (matEdit == -1)
        modifierMat(index);
}

void CeCWriter::on_listPrep_doubleClicked(const QModelIndex &index)
{
    if (prepEdit == -1)
        modifierPrep(index);
}

void CeCWriter::on_listCons_doubleClicked(const QModelIndex &index)
{
    if (consEdit == -1)
        modifierCons(index);
}

/**
 * @brief CeCWriter::modifierIngr
 * @param index : index of the item to be modified on the QListView
 * Things to do when user want to change an Ingredient
 */
void CeCWriter::modifierIngr(const QModelIndex &index)
{
    ingrEdit = index.row();
    QString item = model1->item(ingrEdit)->text();
    QStringList parts = item.split("|");
    if (parts[0] != "comm")
    {
        idIngr = parts[0].toInt();
        ingrComm = false;
        ui->ingrListShow->setText(parts[0]);
        ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
    }
    else
    {
        idIngr = 0;
        ingrComm = true;
        ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
        ui->ingrListShow->setText("0");
    }
    model1->item(ingrEdit)->setBackground(QBrush(colorEdit));
    ui->listIngr->clearSelection();
    ui->listIngr->setSelectionMode(QAbstractItemView::NoSelection);
    ui->editIngr->setText(parts[1]);
    ui->editIngr->setFocus();
}

/**
 * @brief CeCWriter::modifierIngr
 * @param index : index of the item to be modified on the QListView
 * Things to do when user want to change an Materiel
 */
void CeCWriter::modifierMat(const QModelIndex &index)
{
    matEdit = index.row();
    QString text = model2->item(matEdit)->text();
    QStringList parts = text.split("|");
    if (parts[0] == "comm")
    {
        matComm = true;
        ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
    }
    else
    {
        matComm = false;
        ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
    }
    model2->item(matEdit)->setBackground(QBrush(colorEdit));
    ui->listMat->clearSelection();
    ui->listMat->setSelectionMode(QAbstractItemView::NoSelection);
    ui->editMat->setText(parts[1]);
    ui->editMat->setFocus();
}

/**
 * @brief CeCWriter::modifierIngr
 * @param index : index of the item to be modified on the QListView
 * Things to do when user want to change an Preparation
 */
void CeCWriter::modifierPrep(const QModelIndex &index)
{
    prepEdit = index.row();
    QString text = model3->item(prepEdit)->text();
    QStringList parts = text.split("|");
    idPrep = parts[0];
    //Dowgrade number because we make +1 when it is enter:
    if (idPrep != "comm")
    {
        QStringList nbs = idPrep.split(".");
        nbs[nbs.size() - 2] = QString::number(nbs[nbs.size() - 2].toInt() - 1);
        idPrep = nbs.join(".");
        ui->prepListShow->setText(QString::number(idPrep.split(".").size() - 2));
        prepComm = false;
        ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
    }
    else
    {
        idPrep = "0.0";
        prepComm = true;
        ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
        ui->prepListShow->setText("0");
    }
    ui->editPrep->setPlainText(parts[1]);
    ui->editPrep->setFocus();
    QTextCursor curs = ui->editPrep->textCursor();
    int posC = curs.position();
    int lenT = ui->editPrep->toPlainText().length();
    curs.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, lenT-posC);
    ui->editPrep->setTextCursor(curs);
    model3->item(prepEdit)->setBackground(QBrush(colorEdit));
    ui->listPrep->clearSelection();
    ui->listPrep->setSelectionMode(QAbstractItemView::NoSelection);
}

/**
 * @brief CeCWriter::modifierIngr
 * @param index : index of the item to be modified on the QListView
 * Things to do when user want to change an Conseils
 */
void CeCWriter::modifierCons(const QModelIndex &index)
{
    consEdit = index.row();
    QString text = model4->item(consEdit)->text();
    QStringList parts = text.split("|");
    if (parts[0] == "comm")
    {
        consComm = true;
        ui->commButton->setStyleSheet("QPushButton {background-color: red;}");
    }
    else
    {
        consComm = false;
        ui->commButton->setStyleSheet("QPushButton {background-color: none;}");
    }
    ui->editCons->setPlainText(parts[1]);
    ui->editCons->setFocus();
    QTextCursor curs = ui->editCons->textCursor();
    int posC = curs.position();
    int lenT = ui->editCons->toPlainText().length();
    curs.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, lenT-posC);
    ui->editCons->setTextCursor(curs);
    model4->item(consEdit)->setBackground(QBrush(colorEdit));
    ui->listCons->clearSelection();
    ui->listCons->setSelectionMode(QAbstractItemView::NoSelection);
}

/**
 * @brief CeCWriter::modifierIngr
 * Things to do when user want to delete an Ingredient
 */
void CeCWriter::supprimerIngr()
{
    QModelIndexList indexes;
    while((indexes = ui->listIngr->selectionModel()->selectedIndexes()).size()) {
        ui->listIngr->model()->removeRow(indexes.first().row());
    }
}

/**
 * @brief CeCWriter::modifierIngr
 * Things to do when user want to delete a Materiel
 */
void CeCWriter::supprimerMat()
{
    QModelIndexList indexes;
    while((indexes = ui->listMat->selectionModel()->selectedIndexes()).size()) {
        ui->listMat->model()->removeRow(indexes.first().row());
    }
}

/**
 * @brief CeCWriter::modifierIngr
 * Things to do when user want to delete a Preparation
 */
void CeCWriter::supprimerPrep()
{
    QModelIndexList items = ui->listPrep->selectionModel()->selectedIndexes();
    qSort(items);
    int len = items.size();
    if (len > 0)
    {
        //Delete items selected and update next puce items:
        for (int i = len - 1; i >= 0; --i) {
            QString it = model3->item(items.value(i).row())->text();
            QString puce = it.split("|")[0];
            int niv = puce.split(".").size();
            ui->listPrep->model()->removeRow(items.value(i).row());
            int k = items.value(i).row();
            bool continuer = true;
            while (k < model3->rowCount() && continuer) {;
                QString fit = model3->item(k)->text();
                QStringList parts = fit.split("|");
                QString puce = parts[0];
                QStringList puceParts = puce.split(".");
                if (puceParts.size() == niv)
                {
                    puceParts[puceParts.size() - 2] = QString::number(puceParts[puceParts.size() - 2].toInt() - 1);
                    puce = puceParts.join(".");
                    model3->item(k)->setText(puce + "|" + parts[1]);
                }
                else if (puceParts.size() < niv)
                {
                    continuer = false;
                }
                k++;
            }
        }
        //If we was in edition mode, now we are not in edition mode:
        prepEdit = -1;
        //We save last puce:
        if (model3->rowCount() > 0)
            idPrep = model3->item(model3->rowCount() - 1)->text().split("|")[0];
        else
            idPrep = "0.0";
        ui->listPrep->clearSelection();
    }
}

/**
 * @brief CeCWriter::modifierIngr
 * Things to do when user want to delete a Conseils
 */
void CeCWriter::supprimerCons()
{
    QModelIndexList indexes;
    while((indexes = ui->listCons->selectionModel()->selectedIndexes()).size()) {
        ui->listCons->model()->removeRow(indexes.first().row());
    }
}

/**
 * @brief CeCWriter::modifierIngr
 * When user want to increase increment of Ingredient list
 */
void CeCWriter::on_ingrListPlus_clicked()
{
    if (ui->ingrListShow->text().toInt() <= 3)
    {
        idIngr++;
        ui->ingrListShow->setText(QString::number(idIngr));
        ui->editIngr->setFocus();
    }
}

/**
 * @brief CeCWriter::modifierIngr
 * When user want to decrease increment of Ingredient list
 */
void CeCWriter::on_ingrListMoins_clicked()
{
    if (idIngr > 0)
    {
        idIngr--;
        ui->ingrListShow->setText(QString::number(idIngr));
        ui->editIngr->setFocus();
    }
}

/**
 * @brief CeCWriter::modifierIngr
 * When user want to increase increment of Preparation list
 */
void CeCWriter::on_prepListPlus_clicked()
{
    if (ui->prepListShow->text().toInt() <= 3)
    {
        idPrep += ".0";
        ui->prepListShow->setText(QString::number(ui->prepListShow->text().toInt() + 1));
        ui->editPrep->setFocus();
    }
}

/**
 * @brief CeCWriter::modifierIngr
 * When user want to decrease increment of Preparation list
 */
void CeCWriter::on_prepListMoins_clicked()
{
    if (ui->prepListShow->text().toInt() > 0)
    {
        idPrep = idPrep.left(idPrep.size() - 3) + "0";
        ui->prepListShow->setText(QString::number(ui->prepListShow->text().toInt() - 1));
        ui->editPrep->setFocus();
    }
}

/**
 * @brief CeCWriter::modifierIngr
 * Function launched when the Main Window is resized
 */
void CeCWriter::resizeEvent(QResizeEvent* event)
{
    event->accept();
    //Repaint listViews:
    if (model1->rowCount() > 0)
    {
        QList<QString> s1;
        for (int i = 0; i < model1->rowCount(); ++i) {
            s1.append(model1->item(i)->text());
        }
        model1->clear();
        foreach (QString item, s1) {
            model1->appendRow(new QStandardItem(item));
        }
    }
    if (model2->rowCount() > 0)
    {
        QList<QString> s2;
        for (int i = 0; i < model2->rowCount(); ++i) {
            s2.append(model2->item(i)->text());
        }
        model2->clear();
        foreach (QString item, s2) {
            model2->appendRow(new QStandardItem(item));
        }
    }
    if (model3->rowCount() > 0)
    {
        QList<QString> s3;
        for (int i = 0; i < model3->rowCount(); ++i) {
            s3.append(model3->item(i)->text());
        }
        model3->clear();
        foreach (QString item, s3) {
            model3->appendRow(new QStandardItem(item));
        }
    }
    if (model4->rowCount() > 0)
    {
        QList<QString> s4;
        for (int i = 0; i < model4->rowCount(); ++i) {
            s4.append(model4->item(i)->text());
        }
        model4->clear();
        foreach (QString item, s4) {
            model4->appendRow(new QStandardItem(item));
        }
    }

    //Set logo fitting size:
    this->setLogoWindow();
}

//Set logo window depending on size of window:
void CeCWriter::setLogoWindow()
{
    QColor colorWin = this->palette().color(QPalette::Background);
    if (ui->centralWidget->height() >= 890) {
        if ((colorWin.red() + colorWin.green() + colorWin.blue()) / 3 < 127.5)
        {
            ui->logo->setPixmap(QPixmap(":/images/logo-grand_blanc.png"));
        }
        else
        {
            ui->logo->setPixmap(QPixmap(":/images/logo-grand.png"));
        }
    }
    else {
        if ((colorWin.red() + colorWin.green() + colorWin.blue()) / 3 < 127.5)
        {
            ui->logo->setPixmap(QPixmap(":/images/logo_blanc.png"));
        }
        else
        {
            ui->logo->setPixmap(QPixmap(":/images/logo.png"));
        }
    }
}

//Context menus:
void CeCWriter::on_listIngr_customContextMenuRequested(const QPoint &pos)
{
    if (ui->listIngr->selectionModel()->selectedIndexes().size() > 0)
    {
        QStringList cont_menu;
        if (ui->listIngr->selectionModel()->selectedIndexes().size() == 1)
        {
            cont_menu << "Modifier l'élément";
            cont_menu << "Supprimer l'élément";
            cont_menu << "Insérer un élément après";
            cont_menu << "Insérer un élément avant";

            QMenu my_menu;
            for (int i = 0; i < cont_menu.size(); ++i)
            my_menu.addAction(cont_menu.at(i));

            QPoint pt = ui->listIngr->mapToGlobal(pos);
            QAction* sel_item = my_menu.exec(pt);

            if (sel_item)
            {
            // something was chosen, do stuff
                QString s = sel_item->text();
                if (s == cont_menu.at(0))
                {
                    // Modifier l'élément is handled
                    modifierIngr(ui->listIngr->selectionModel()->selectedIndexes().at(0));
                }
                else if (s == cont_menu.at(1))
                {
                    // Supprimer l'élément is handled
                    supprimerIngr();
                }
                else if (s == cont_menu.at(2))
                {
                    //Insérer un élément après is handled
                    ingrEdit = ui->listIngr->selectionModel()->selectedIndexes().at(0).row();
                    int puce = model1->item(ingrEdit)->text().split("|").at(0).toInt();
                    ingrEdit++;
                    model1->insertRow(ingrEdit, new QStandardItem(QString::number(puce) + "|"));
                    modifierIngr(model1->index(ingrEdit, 0));
                }
                else if (s == cont_menu.at(3))
                {
                    //Insérer un élément avant is handled
                    ingrEdit = ui->listIngr->selectionModel()->selectedIndexes().at(0).row();
                    int puce = model1->item(ingrEdit)->text().split("|").at(0).toInt();
                    model1->insertRow(ingrEdit, new QStandardItem(QString::number(puce) + "|"));
                    modifierIngr(model1->index(ingrEdit, 0));
                }

            }
        }
        else
        {
            cont_menu << "Supprimer les éléments";
            QMenu my_menu;
            for (int i = 0; i < cont_menu.size(); ++i)
            my_menu.addAction(cont_menu.at(i));

            QPoint pt = ui->listIngr->mapToGlobal(pos);
            QAction* sel_item = my_menu.exec(pt);

            if (sel_item)
            {
            // something was chosen, do stuff
                QString s = sel_item->text();
                if (s == cont_menu.at(0))
                {
                    //Supprimer les éléments is handled:
                    supprimerIngr();
                }
            }
        }
    }
}

void CeCWriter::on_listMat_customContextMenuRequested(const QPoint &pos)
{
    if (ui->listMat->selectionModel()->selectedIndexes().size() > 0)
    {
        QStringList cont_menu;
        if (ui->listMat->selectionModel()->selectedIndexes().size() == 1)
        {
            cont_menu << "Modifier l'élément";
            cont_menu << "Supprimer l'élément";
            cont_menu << "Insérer un élément après";
            cont_menu << "Insérer un élément avant";

            QMenu my_menu;
            for (int i = 0; i < cont_menu.size(); ++i)
            my_menu.addAction(cont_menu.at(i));

            QPoint pt = ui->listMat->mapToGlobal(pos);
            QAction* sel_item = my_menu.exec(pt);

            if (sel_item)
            {
            // something was chosen, do stuff
                QString s = sel_item->text();
                if (s == cont_menu.at(0))
                {
                    // Modifier l'élément is handled
                    modifierMat(ui->listMat->selectionModel()->selectedIndexes().at(0));
                }
                else if (s == cont_menu.at(1))
                {
                    // Supprimer l'élément is handled
                    supprimerMat();
                }
                else if (s == cont_menu.at(2))
                {
                    //Insérer un élément après is handled
                    matEdit = ui->listMat->selectionModel()->selectedIndexes().at(0).row() + 1;
                    model2->insertRow(matEdit, new QStandardItem("0|"));
                    modifierMat(model2->index(matEdit, 0));
                }
                else if (s == cont_menu.at(3))
                {
                    //Insérer un élément avant is handled
                    matEdit = ui->listMat->selectionModel()->selectedIndexes().at(0).row();
                    model2->insertRow(matEdit, new QStandardItem("0|"));
                    modifierMat(model2->index(matEdit, 0));
                }

            }
        }
        else
        {
            cont_menu << "Supprimer les éléments";
            QMenu my_menu;
            for (int i = 0; i < cont_menu.size(); ++i)
            my_menu.addAction(cont_menu.at(i));

            QPoint pt = ui->listMat->mapToGlobal(pos);
            QAction* sel_item = my_menu.exec(pt);

            if (sel_item)
            {
            // something was chosen, do stuff
                QString s = sel_item->text();
                if (s == cont_menu.at(0))
                {
                    //Supprimer les éléments is handled:
                    supprimerMat();
                }
            }
        }
    }
}

void CeCWriter::on_listPrep_customContextMenuRequested(const QPoint &pos)
{
    if (ui->listPrep->selectionModel()->selectedIndexes().size() > 0)
    {
        QStringList cont_menu;
        if (ui->listPrep->selectionModel()->selectedIndexes().size() == 1)
        {
            cont_menu << "Modifier l'élément";
            cont_menu << "Supprimer l'élément";
            cont_menu << "Insérer un élément après";
            cont_menu << "Insérer un élément avant";

            QMenu my_menu;
            for (int i = 0; i < cont_menu.size(); ++i)
            my_menu.addAction(cont_menu.at(i));

            QPoint pt = ui->listPrep->mapToGlobal(pos);
            QAction* sel_item = my_menu.exec(pt);

            if (sel_item)
            {
            // something was chosen, do stuff
                QString s = sel_item->text();
                if (s == cont_menu.at(0))
                {
                    // Modifier l'élément is handled
                    modifierPrep(ui->listPrep->selectionModel()->selectedIndexes().at(0));
                }
                else if (s == cont_menu.at(1))
                {
                    // Supprimer l'élément is handled
                    supprimerPrep();
                }
                else if (s == cont_menu.at(2))
                {
                    //Insérer un élément après is handled
                    prepEdit = ui->listPrep->selectionModel()->selectedIndexes().at(0).row();
                    QString puce = model3->item(prepEdit)->text().split("|").at(0);
                    QStringList puceParts = puce.split(".");
                    puceParts[puceParts.size() - 2] = QString::number(puceParts[puceParts.size() - 2].toInt() + 1);
                    puce = puceParts.join(".");
                    prepEdit++;
                    model3->insertRow(prepEdit, new QStandardItem(puce + "|"));
                    //Update next puces:
                    int idP = prepEdit + 1;
                    int nbParts = puceParts.size();
                    bool loopEnded = false;
                    while (idP < model3->rowCount() && !loopEnded) {
                        QString itemP = model3->item(idP)->text();
                        QStringList partsT = itemP.split("|");
                        QString puceP = partsT[0];
                        QStringList pPart = puceP.split(".");
                        if (pPart.size() == nbParts)
                        {
                            pPart[pPart.size() - 2] = QString::number(pPart[pPart.size() - 2].toInt() + 1);
                            puceP = pPart.join(".");
                            model3->item(idP)->setText(puceP + "|" + partsT[1]);
                        }
                        else if (pPart.size() < nbParts)
                        {
                            loopEnded = true;
                        }
                        idP++;
                    }
                    modifierPrep(model3->index(prepEdit, 0));
                }
                else if (s == cont_menu.at(3))
                {
                    //Insérer un élément avant is handled
                    prepEdit = ui->listPrep->selectionModel()->selectedIndexes().at(0).row();
                    QString puce = model3->item(prepEdit)->text().split("|").at(0);
                    QStringList puceParts = puce.split(".");
                    model3->insertRow(prepEdit, new QStandardItem(puce + "|"));
                    //Update next puces:
                    int idP = prepEdit + 1;
                    int nbParts = puceParts.size();
                    bool loopEnded = false;
                    while (idP < model3->rowCount() && !loopEnded) {
                        QString itemP = model3->item(idP)->text();
                        QStringList partsT = itemP.split("|");
                        QString puceP = partsT[0];
                        QStringList pPart = puceP.split(".");
                        if (pPart.size() == nbParts)
                        {
                            pPart[pPart.size() - 2] = QString::number(pPart[pPart.size() - 2].toInt() + 1);
                            puceP = pPart.join(".");
                            model3->item(idP)->setText(puceP + "|" + partsT[1]);
                        }
                        else if (pPart.size() < nbParts)
                        {
                            loopEnded = true;
                        }
                        idP++;
                    }
                    modifierPrep(model3->index(prepEdit, 0));
                }

            }
        }
        else
        {
            cont_menu << "Supprimer les éléments";
            QMenu my_menu;
            for (int i = 0; i < cont_menu.size(); ++i)
            my_menu.addAction(cont_menu.at(i));

            QPoint pt = ui->listPrep->mapToGlobal(pos);
            QAction* sel_item = my_menu.exec(pt);

            if (sel_item)
            {
            // something was chosen, do stuff
                QString s = sel_item->text();
                if (s == cont_menu.at(0))
                {
                    //Supprimer les éléments is handled:
                    supprimerPrep();
                }
            }
        }
    }
}

void CeCWriter::on_listCons_customContextMenuRequested(const QPoint &pos)
{
    if (ui->listCons->selectionModel()->selectedIndexes().size() > 0)
    {
        QStringList cont_menu;
        if (ui->listCons->selectionModel()->selectedIndexes().size() == 1)
        {
            cont_menu << "Modifier l'élément";
            cont_menu << "Supprimer l'élément";
            cont_menu << "Insérer un élément après";
            cont_menu << "Insérer un élément avant";

            QMenu my_menu;
            for (int i = 0; i < cont_menu.size(); ++i)
            my_menu.addAction(cont_menu.at(i));

            QPoint pt = ui->listCons->mapToGlobal(pos);
            QAction* sel_item = my_menu.exec(pt);

            if (sel_item)
            {
            // something was chosen, do stuff
                QString s = sel_item->text();
                if (s == cont_menu.at(0))
                {
                    // Modifier l'élément is handled
                    modifierCons(ui->listCons->selectionModel()->selectedIndexes().at(0));
                }
                else if (s == cont_menu.at(1))
                {
                    // Supprimer l'élément is handled
                    supprimerCons();
                }
                else if (s == cont_menu.at(2))
                {
                    //Insérer un élément après is handled
                    consEdit = ui->listCons->selectionModel()->selectedIndexes().at(0).row() + 1;
                    model4->insertRow(consEdit, new QStandardItem("0|"));
                    modifierCons(model4->index(consEdit, 0));
                }
                else if (s == cont_menu.at(3))
                {
                    //Insérer un élément avant is handled
                    consEdit = ui->listCons->selectionModel()->selectedIndexes().at(0).row();
                    model4->insertRow(consEdit, new QStandardItem("0|"));
                    modifierCons(model4->index(consEdit, 0));
                }

            }
        }
        else
        {
            cont_menu << "Supprimer les éléments";
            QMenu my_menu;
            for (int i = 0; i < cont_menu.size(); ++i)
            my_menu.addAction(cont_menu.at(i));

            QPoint pt = ui->listCons->mapToGlobal(pos);
            QAction* sel_item = my_menu.exec(pt);

            if (sel_item)
            {
            // something was chosen, do stuff
                QString s = sel_item->text();
                if (s == cont_menu.at(0))
                {
                    //Supprimer les éléments is handled:
                    supprimerCons();
                }
            }
        }
    }
}

void CeCWriter::on_editIngr_customContextMenuRequested(const QPoint &pos)
{
    QStringList cont_menu;
    cont_menu << "Importer des ingrédients";
    QMenu my_menu;
    for (int i = 0; i < cont_menu.size(); ++i)
        my_menu.addAction(cont_menu.at(i));
    QPoint pt = ui->editIngr->mapToGlobal(pos);
    QAction* sel_item = my_menu.exec(pt);
    if (sel_item)
    {
        QString s = sel_item->text();
        if (s == cont_menu.at(0))
        {
            QInputMultiline* dialg = new QInputMultiline(this, "Importer une liste d'ingrédients", "Entrez les ingrédients (1 par ligne)", "", false);
            dialg->exec();
            QString txtImp = dialg->text;
            if (txtImp != "") {
                QStringList items = txtImp.split("\n");
                ingrEdit = -1;
                int idIngrOrig = idIngr;
                bool ingrCommOrig = ingrComm;
                foreach (QString item, items) {
                    int indent = 0;
                    QString txt = item;
                    QRegExp exp ("^(\t+)(.*)$");
                    ingrComm = false;
                    if (exp.exactMatch(item)) {
                        indent = exp.cap(1).length();
                        txt = exp.cap(2);
                    }
                    else {
                        exp.setPattern("^#(.*)$");
                        if (exp.exactMatch(item)) {
                            txt = exp.cap(1);
                            ingrComm = true;
                        }
                    }
                    idIngr = idIngrOrig + indent;
                    if (txt != "")
                        this->insertIngredient(txt);
                }
                idIngr = idIngrOrig;
                ingrComm = ingrCommOrig;
            }
        }
    }
}

void CeCWriter::on_editMat_customContextMenuRequested(const QPoint &pos)
{
    QStringList cont_menu;
    cont_menu << "Importer du matériel";
    QMenu my_menu;
    for (int i = 0; i < cont_menu.size(); ++i)
        my_menu.addAction(cont_menu.at(i));
    QPoint pt = ui->editMat->mapToGlobal(pos);
    QAction* sel_item = my_menu.exec(pt);
    if (sel_item)
    {
        QString s = sel_item->text();
        if (s == cont_menu.at(0))
        {
            QInputMultiline* dialg = new QInputMultiline(this, "Importer une liste de matériel", "Entrez le matériel (1 par ligne)", "", false);
            dialg->exec();
            QString txtImp = dialg->text;
            if (txtImp != "") {
                QStringList items = txtImp.split("\n");
                matEdit = -1;
                bool matCommOrig = matComm;
                foreach (QString item, items) {
                    QString txt = item;
                    QRegExp exp ("^#(.*)$");
                    matComm = false;
                    if (exp.exactMatch(item)) {
                        txt = exp.cap(1);
                        matComm = true;
                    }
                    if (txt != "")
                        this->insertMateriel(txt);
                }
                matComm = matCommOrig;
            }
        }
    }
}

void CeCWriter::on_editPrep_customContextMenuRequested(const QPoint &pos)
{
    QStringList cont_menu;
    cont_menu << "Importer des instructions de préparation";
    QMenu my_menu;
    for (int i = 0; i < cont_menu.size(); ++i)
        my_menu.addAction(cont_menu.at(i));
    QPoint pt = ui->editPrep->mapToGlobal(pos);
    QAction* sel_item = my_menu.exec(pt);
    if (sel_item)
    {
        QString s = sel_item->text();
        if (s == cont_menu.at(0))
        {
            QInputMultiline* dialg = new QInputMultiline(this, "Importer des instructions de préparation", "Entrez les instructions de préparation (1 par ligne)", "", false);
            dialg->exec();
            QString txtImp = dialg->text;
            if (txtImp != "") {
                QStringList items = txtImp.split("\n");
                prepEdit = -1;
                bool prepCommOrig = prepComm;
                int indent = 0;
                foreach (QString item, items) {
                    QString txt = item;
                    QRegExp exp ("^(\t+)(.*)$");
                    prepComm = false;
                    int newIndent = 0;
                    if (exp.exactMatch(item)) {
                        newIndent = exp.cap(1).length();
                        txt = exp.cap(2);
                        if (newIndent > indent) {
                            for (int it = 0; it < newIndent - indent; ++it) {
                                idPrep += ".0";
                            }
                        }
                    }
                    else {
                        exp.setPattern("^#(.*)$");
                        if (exp.exactMatch(item)) {
                            txt = exp.cap(1);
                            prepComm = true;
                        }
                        else if (newIndent < indent) {
                            for (int it = 0; it < indent - newIndent; ++it) {
                                QStringList idPrepList = idPrep.split(".");
                                    idPrepList.removeAt(qMax(0,idPrepList.length() - 2)); //In reality, idPrepList length will be always >= 2, but doing this avoid a warning.
                                idPrep = idPrepList.join(".");
                            }
                        }
                    }
                    indent = newIndent;
                    //idPrep = idPrepOrig + indent;
                    if (txt != "")
                        this->insertPreparation(txt);
                }
                ui->prepListShow->setText(QString::number(idPrep.split(".").length() - 2));
                prepComm = prepCommOrig;
            }
        }
    }
}

void CeCWriter::on_editCons_customContextMenuRequested(const QPoint &pos)
{
    QStringList cont_menu;
    cont_menu << "Importer des conseils";
    QMenu my_menu;
    for (int i = 0; i < cont_menu.size(); ++i)
        my_menu.addAction(cont_menu.at(i));
    QPoint pt = ui->editCons->mapToGlobal(pos);
    QAction* sel_item = my_menu.exec(pt);
    if (sel_item)
    {
        QString s = sel_item->text();
        if (s == cont_menu.at(0))
        {
            QInputMultiline* dialg = new QInputMultiline(this, "Importer une liste de conseils", "Entrez les conseils (1 par ligne)", "", false);
            dialg->exec();
            QString txtImp = dialg->text;
            if (txtImp != "") {
                QStringList items = txtImp.split("\n");
                consEdit = -1;
                bool matCommOrig = consComm;
                foreach (QString item, items) {
                    QString txt = item;
                    QRegExp exp ("^#(.*)$");
                    consComm = false;
                    if (exp.exactMatch(item)) {
                        txt = exp.cap(1);
                        consComm = true;
                    }
                    if (txt != "")
                        this->insertConseil(txt);
                }
                consComm = matCommOrig;
            }
        }
    }
}

//Dialogs events:

void CeCWriter::handleSaveAnnuler()
{
    chooseSaveFileName->close();
    doSave = false;
}

void CeCWriter::handleSaveValider()
{
    saveFileName = chooseSaveFN->text();
    chooseSaveFileName->close();
    doSave = true;
}

void CeCWriter::handleChooseLnkAnnuler()
{
    chooseLink->close();
}

void CeCWriter::handleChooseLnkValider()
{
    addrLnk = chooseAddrLnk->text();
    chooseLink->close();
}

void CeCWriter::refreshState()
{
    ui->state->setText("Prêt.");
}

/**
/ Search balises and delete them if a part of them is deleted:
**/

//1. ON LINK BALISES:

void CeCWriter::checkRemoveLnkSuppr(QString text, int caretPos)
{
    //Get character that will be deleted:
    QStringList number;
    for (int i = 0; i <= 9; i++) {
        number.append(QString::number(i));
    }
    QString willRmv = text.mid(caretPos, 1);
    QList<int> balDsearch, balFsearch, balDsearchExt, balFsearchExt;
    balDsearch << 0 << 0;
    balFsearch << 0 << 0;
    balDsearchExt << 0 << 0;
    balFsearchExt << 0 << 0;
    if (willRmv == ">") {
        balDsearch[1] = caretPos + 1;
        balDsearch[0] = caretPos - 3;
        balFsearch[1] = caretPos + 1;
        balFsearch[0] = caretPos - 4;
        balDsearchExt[1] = caretPos + 1;
        balDsearchExt[0] = caretPos - 4;
        balFsearchExt[1] = caretPos + 1;
        balFsearchExt[0] = caretPos - 5;
    }
    else if (number.contains(willRmv)) {
        if (number.contains(text.mid(caretPos + 1, 1))) {
            balDsearchExt[0] = caretPos - 2;
            balFsearchExt[0] = caretPos - 3;
            balDsearchExt[1] = caretPos + 3;
            balFsearchExt[1] = caretPos + 3;
        }
        else if (number.contains(text.mid(qMax(0, caretPos - 1), 1))) {
            balDsearchExt[0] = caretPos - 3;
            balFsearchExt[0] = caretPos - 4;
            balDsearchExt[1] = caretPos + 2;
            balFsearchExt[1] = caretPos + 2;
        }
        else {
            balDsearch[0] = caretPos - 2;
            balFsearch[0] = caretPos - 3;
            balDsearch[1] = caretPos + 2;
            balFsearch[1] = caretPos + 2;
        }
    }
    else if (willRmv =="L") {
        balDsearch[1] = caretPos + 3;
        balDsearch[0] = caretPos - 1;
        balFsearch[1] = caretPos + 3;
        balFsearch[0] = caretPos - 2;
        balDsearchExt[1] = caretPos + 4;
        balDsearchExt[0] = caretPos - 1;
        balFsearchExt[1] = caretPos + 4;
        balFsearchExt[0] = caretPos - 2;
    }
    else if (willRmv =="/") {
        balFsearch[1] = caretPos + 4;
        balFsearch[0] = caretPos - 1;
        balFsearchExt[1] = caretPos + 5;
        balFsearchExt[0] = caretPos - 1;
    }
    else if (willRmv == "<") {
        balDsearch[1] = caretPos + 4;
        balDsearch[0] = caretPos;
        balFsearch[1] = caretPos + 5;
        balFsearch[0] = caretPos;
        balDsearchExt[1] = caretPos + 5;
        balDsearchExt[0] = caretPos;
        balFsearchExt[1] = caretPos + 6;
        balFsearchExt[0] = caretPos;
    }
    QRegExp pattD ("<L\\d>");
    QRegExp pattF ("</L\\d>");
    QRegExp pattDExt ("<L\\d{2}>");
    QRegExp pattFExt ("</L\\d{2}>");
    bool balFound = false;
    if (balFsearch[1] > 0 && balFsearch[1] <= text.length() && balFsearch[0] >= 0 && balFsearch[0] < text.length()) {
        QString search = text.mid(balFsearch[0], balFsearch[1] - balFsearch[0]);
        if (search.contains(pattF)) {
            QString cBal = pattF.cap();
            balise = cBal.mid(2, cBal.length() - 3);
            resetPosCaret = balFsearch[0] - 4;
            balFound = true;
        }
    }
    if (balDsearch[1] > 0 && balDsearch[1] <= text.length() && balDsearch[0] >= 0 && balDsearch[0] < text.length() && !balFound) {
        QString search = text.mid(balDsearch[0], balDsearch[1] - balDsearch[0]);
        if (search.contains(pattD)) {
            QString cBal = pattD.cap();
            balise = cBal.mid(1, cBal.length() - 2);
            resetPosCaret = balDsearch[0];
            balFound = true;
        }
    }
    if (balFsearchExt[1] > 0 && balFsearchExt[1] <= text.length() && balFsearchExt[0] >= 0 && balFsearchExt[0] < text.length() && !balFound) {
        QString search = text.mid(balFsearchExt[0], balFsearchExt[1] - balFsearchExt[0]);
        if (search.contains(pattFExt)) {
            QString cBal = pattFExt.cap();
            balise = cBal.mid(2, cBal.length() - 3);
            resetPosCaret = balFsearchExt[0] - 5;
            balFound = true;
        }
    }
    if (balDsearchExt[1] > 0 && balDsearchExt[1] <= text.length() && balDsearchExt[0] >= 0 && balDsearchExt[0] < text.length() && !balFound) {
        QString search = text.mid(balDsearchExt[0], balDsearchExt[1] - balDsearchExt[0]);
        if (search.contains(pattDExt)) {
            QString cBal = pattDExt.cap();
            balise = cBal.mid(1, cBal.length() - 2);
            resetPosCaret = balDsearchExt[0];
        }
    }
    if (balise != "" && liens.contains(balise))
    {
        liens.remove(balise);
    }
}

void CeCWriter::checkRemoveLnkBckp(QString text, int caretPos)
{
    QStringList number;
    for (int i = 0; i <= 9; i++) {
        number.append(QString::number(i));
    }
    QString willRmv = text.mid(caretPos - 1, 1);
    QList<int> balDsearch, balFsearch, balDsearchExt, balFsearchExt;
    balDsearch << 0 << 0;
    balFsearch << 0 << 0;
    balDsearchExt << 0 << 0;
    balFsearchExt << 0 << 0;
    if (willRmv == ">") {
        balDsearch[1] = caretPos;
        balDsearch[0] = caretPos - 4;
        balFsearch[1] = caretPos;
        balFsearch[0] = caretPos - 5;
        balDsearchExt[1] = caretPos;
        balDsearchExt[0] = caretPos - 5;
        balFsearchExt[1] = caretPos;
        balFsearchExt[0] = caretPos - 6;
    }
    else if (number.contains(willRmv)) {
        if (number.contains(text.mid(qMax(0, caretPos - 2), 1))) {
            balDsearchExt[0] = caretPos - 4;
            balFsearchExt[0] = caretPos - 5;
            balDsearchExt[1] = caretPos + 1;
            balFsearchExt[1] = caretPos + 1;
        }
        else if (number.contains(text.mid(caretPos - 1, 1))) {
            balDsearchExt[0] = caretPos - 3;
            balFsearchExt[0] = caretPos - 4;
            balDsearchExt[1] = caretPos + 2;
            balFsearchExt[1] = caretPos + 2;
        }
        else {
            balDsearch[0] = caretPos - 3;
            balFsearch[0] = caretPos - 4;
            balDsearch[1] = caretPos + 1;
            balFsearch[1] = caretPos + 1;
        }
    }
    else if (willRmv == "L") {
        balDsearch[1] = caretPos + 2;
        balDsearch[0] = caretPos - 2;
        balFsearch[1] = caretPos + 2;
        balFsearch[0] = caretPos - 3;
        balDsearchExt[1] = caretPos + 3;
        balDsearchExt[0] = caretPos - 2;
        balFsearchExt[1] = caretPos + 3;
        balFsearchExt[0] = caretPos - 3;
    }
    else if (willRmv == "/") {
        balFsearch[1] = caretPos + 3;
        balFsearch[0] = caretPos - 2;
        balFsearchExt[1] = caretPos + 4;
        balFsearchExt[0] = caretPos - 2;
    }
    else if (willRmv =="<") {
        balDsearch[1] = caretPos + 3;
        balDsearch[0] = caretPos - 1;
        balFsearch[1] = caretPos + 4;
        balFsearch[0] = caretPos - 1;
        balDsearchExt[1] = caretPos + 4;
        balDsearchExt[0] = caretPos - 1;
        balFsearchExt[1] = caretPos + 5;
        balFsearchExt[0] = caretPos - 1;
    }
    QRegExp pattD ("<L\\d>");
    QRegExp pattF ("</L\\d>");
    QRegExp pattDExt ("<L\\d{2}>");
    QRegExp pattFExt ("</L\\d{2}>");
    bool balFound = false;
    if (balFsearch[1] > 0 && balFsearch[0] >= 0 && balFsearch[1] <= text.length()) {
        QString search = text.mid(balFsearch[0], balFsearch[1] - balFsearch[0]);
        if (search.contains(pattF)) {
            QString cBal = pattF.cap();
            balise = cBal.mid(2, cBal.length() - 3);
            resetPosCaret = balFsearch[0] - 4;
            balFound = true;
        }
    }
    if (balDsearch[1] > 0 && balDsearch[0] >= 0 && balDsearch[1] <= text.length() && !balFound) {
        QString search = text.mid(balDsearch[0], balDsearch[1]);
        if (search.contains(pattD)) {
            QString cBal = pattD.cap();
            balise = cBal.mid(1, cBal.length() - 2);
            resetPosCaret = balDsearch[0];
            balFound = true;
        }
    }
    if (balFsearchExt[1] > 0 && balFsearchExt[0] >= 0 && balFsearchExt[1] <= text.length() && !balFound) {
        QString search = text.mid(balFsearchExt[0], balFsearchExt[1]);
        if (search.contains(pattFExt)) {
            QString cBal = pattFExt.cap();
            balise = cBal.mid(2, cBal.length() - 3);
            resetPosCaret = balFsearchExt[0] - 5;
            balFound = true;
        }
    }
    if (balDsearchExt[1] > 0 && balDsearchExt[0] >= 0 && balDsearchExt[1] <= text.length() && !balFound) {
        QString search = text.mid(balDsearchExt[0], balDsearchExt[1]);
        if (search.contains(pattDExt)) {
            QString cBal = pattDExt.cap();
            balise = cBal.mid(1, cBal.length() - 2);
            resetPosCaret = balDsearchExt[0];
        }
    }
    if (balise != "" && liens.contains(balise))
    {
        liens.remove(balise);
    }
}

void CeCWriter::checkRemoveLnkSlct(QString text, QList<int> slct)
{
    resetPosCaret = slct[0];

    //Search <L\d> type balise:
    QString search = text.mid(qMax(0, slct[0] - 3), qMin(text.length(), slct[1] + 3));
    QRegExp exp ("<L\\d>");
    int i = 0;
    while (search.right(search.length() - i).contains(exp)) {
        QString bal = exp.cap();
        if (text.indexOf(bal) < slct[0]) {
            resetPosCaret = text.indexOf(bal);
        }
        balises.append(bal.mid(1, bal.length() - 2));
        i = search.indexOf(bal) + 1;
    }

    //Search <L\d{2} type balise:
    search = text.mid(qMax(0, slct[0] - 4), qMin(text.length(), slct[1] + 4));
    exp.setPattern("<L\\d{2}>");
    i = 0;
    while (search.right(search.length() - i).contains(exp)) {
        QString bal = exp.cap();
        if (text.indexOf(bal) < slct[0]) {
            resetPosCaret = text.indexOf(bal);
        }
        balises.append(bal.mid(1, bal.length() - 2));
        i = search.indexOf(bal) + 1;;
    }

    //Search </L\d> type balise:
    exp.setPattern("</L\\d>");
    i = 0;
    while (search.right(search.length() - i).contains(exp)) {
        QString bal = exp.cap();
        if (text.indexOf(bal) < slct[0]) {
            resetPosCaret = text.indexOf(bal);
        }
        QString nameBal = bal.mid(2, bal.length() - 3);
        if (!balises.contains(nameBal))
            balises.append(nameBal);
        if (text.indexOf("<" + nameBal + ">") < slct[0] && text.contains("<" + nameBal + ">")) {
            resetPosCaret -= 4;
        }
        i = search.indexOf(bal) + 1;
    }

    //Search </L\d{2}> type balise:
    search = text.mid(qMax(0, slct[0] - 5), qMin(text.length(), slct[1] + 5));
    exp.setPattern("</L\\d{2}>");
    i = 0;
    while (search.right(search.length() - i).contains(exp)) {
        QString bal = exp.cap();
        if (text.indexOf(bal) < slct[0]) {
            resetPosCaret = text.indexOf(bal);
        }
        QString nameBal = bal.mid(2, bal.length() - 3);
        if (!balises.contains(nameBal))
            balises.append(nameBal);
        if (text.indexOf("<" + nameBal + ">") < slct[0] && text.contains("<" + nameBal + ">")) {
            resetPosCaret -= 5;
        }
        i = search.indexOf(bal) + 1;
    }
    if (balises.size() > 0)
    {
        foreach (QString bal, balises) {
            if (liens.contains(bal))
                liens.remove(balise);
        }
    }
}

//2. ON FORMAT BALISES:

QString CeCWriter::checkRemoveBalBckp(QString text, int caretPos)
{
    QStringList bals;
    bals << "b" << "u" << "i" << "np" << "po";
    int ind = 0;
    while (ind < bals.length() && balise == "")
    {
        QString willRmv = text.mid(caretPos - 1, 1);
        QList<int> balDsearch, balFsearch;
        balDsearch << 0 << 0;
        balFsearch << 0 << 0;
        if (willRmv == ">") {
            balDsearch[1] = caretPos;
            balDsearch[0] = caretPos - (2 + bals[ind].length());
            balFsearch[1] = caretPos;
            balFsearch[0] = caretPos - (3 + bals[ind].length());
        }
        else if (willRmv == "/") {
            balFsearch[1] = caretPos + (1 + bals[ind].length());
            balFsearch[0] = caretPos - 2;
        }
        else if (willRmv == "<") {
            balDsearch[1] = caretPos + (1 + bals[ind].length());
            balDsearch[0] = caretPos - 1;
            balFsearch[1] = caretPos + (2 + bals[ind].length());
            balFsearch[0] = caretPos - 1;
        }
        else {
            int it = 1;
            foreach (QChar letter, bals[ind]) {
                if (willRmv == letter) {
                    balDsearch[1] = caretPos + 1 + (bals[ind].length() - it);
                    balDsearch[0] = caretPos - (1 + it);
                    balFsearch[1] = caretPos + 1 + (bals[ind].length() - it);
                    balFsearch[0] = caretPos - (2 + it);
                }
                it++;
            }
        }
        QRegExp pattD, pattF;
        pattD.setPattern("<" + bals[ind] + ">");
        pattF.setPattern("</" + bals[ind] + ">");
        bool balFound = false;
        if (balFsearch[1] > 0 && balFsearch[0] >= 0 && balFsearch[1] <= text.length()) {
            QString search = text.mid(balFsearch[0], balFsearch[1] - balFsearch[0]);
            if (search.contains(pattF)) {
                resetPosCaret = balFsearch[0] - (2 + bals[ind].length());
                //Format tags are not always unique... So we must indentify the ones that will be deleted:
                //To do that, we UPPER the tags in question:
                int j = 3;
                bool foundSister = false;
                int goodSister = 0;
                while (balFsearch[0] - j >= 0 && !foundSister)
                {
                    int sisterId = text.left(balFsearch[0]).right(j).indexOf("<" + bals[ind] + ">");
                    //OK. If we consider user will be attentive to what he do, it can be sufficient finding a possible sister tag,
                    //as tags are not supposed to embbed other same tags. But user, in real world might do some errors. So, we
                    //take in account user can made non useful embbed tags that embbed other same tags. So there are some cosins
                    //tags, the same as the sister tag, but that is not the sister tag. An example :
                    //<b>*abcd <b>efgh</b> ijkl</b>
                    //We want to delete the tag with a *. We do not delete the first </b> tag (that's the sister of the cosin!), but the second
                    //</b> tag (that's the sister itself, of course!).
                    int cosinId = text.left(balFsearch[0]).right(j).indexOf("</" + bals[ind] + ">");
                    if (cosinId == 0)
                    {
                        goodSister++;
                    }
                    if (sisterId == 0)
                    {
                        if (goodSister == 0)
                        {
                            foundSister = true;
                            sisterId = balFsearch[0] - j;
                            text = text.left(sisterId) + "<" + bals[ind].toUpper() + ">" + text.left(balFsearch[0]).right(text.left(balFsearch[0]).length() - (sisterId + 2 + bals[ind].length())) + "</" + bals[ind].toUpper() + ">" + text.right(text.length() - balFsearch[1]);
                            balise = bals[ind].toUpper();
                        }
                        else
                        {
                            goodSister--;
                        }
                    }
                    j++;
                }
                balFound = true;
            }
        }
        if (balDsearch[1] > 0 && balDsearch[0] >= 0 && balDsearch[1] <= text.length() && !balFound) {
            QString search = text.mid(balDsearch[0], balDsearch[1] - balDsearch[0]);
            if (search.contains(pattD)) {
                resetPosCaret = balDsearch[0];
                int sisterId = text.right(text.length() - balDsearch[1]).indexOf("</" + bals[ind] + ">") + balDsearch[1];
                int cosinId = text.right(text.length() - balDsearch[1]).indexOf("<" + bals[ind] + ">") + balDsearch[1];
                if (cosinId != -1 + balDsearch[1])
                {
                    bool goodSisterFound = false;
                    while (!goodSisterFound && sisterId > -1 && cosinId > -1)
                    {
                        if (cosinId < sisterId)
                        {
                            int followPossibleSister = text.right(text.length() - (sisterId + 1)).indexOf("</" + bals[ind] + ">");
                            if (followPossibleSister != -1)
                            {
                                sisterId += 1 + followPossibleSister;
                                cosinId = text.right(text.length() - (sisterId + 1)).indexOf("<" + bals[ind] + ">");
                            }
                            else
                            {
                                sisterId = -1;
                            }
                        }
                        else
                        {
                            goodSisterFound = true;
                        }
                    }
                }
                if (sisterId > -1)
                {
                    balise = bals[ind].toUpper();
                    text = text.left(balDsearch[0]) + "<" + balise + ">" + text.left(sisterId).right(text.left(sisterId).length() - balDsearch[1]) + "</" + balise + ">" + text.right(text.length() - (sisterId + 3 + bals[ind].length()));
                }
            }
        }
    ind++;
    }
    return text;
}

QString CeCWriter::checkRemoveBalSuppr(QString text, int caretPos)
{
    QStringList bals;
    bals << "b" << "u" << "i" << "np" << "po";
    int ind = 0;
    while (ind < bals.length() && balise == "")
    {
        QString willRmv = text.mid(caretPos, 1);
        QList<int> balDsearch, balFsearch;
        balDsearch << 0 << 0;
        balFsearch << 0 << 0;
        if (willRmv == ">") {
            balDsearch[1] = caretPos + 1;
            balDsearch[0] = caretPos - (1 + bals[ind].length());
            balFsearch[1] = caretPos + 1;
            balFsearch[0] = caretPos - (2 + bals[ind].length());
        }
        else if (willRmv == "/") {
            balFsearch[1] = caretPos + 2 + bals[ind].length();
            balFsearch[0] = caretPos - 1;
        }
        else if (willRmv == "<") {
            balDsearch[1] = caretPos + 2 + bals[ind].length();
            balDsearch[0] = caretPos;
            balFsearch[1] = caretPos + 3 + bals[ind].length();
            balFsearch[0] = caretPos;
        }
        else {
            int it = 0;
            foreach (QChar letter, bals[ind]) {
                if (willRmv == letter) {
                    balDsearch[1] = caretPos + 1 + (bals[ind].length() - it);
                    balDsearch[0] = caretPos - (1 + it);
                    balFsearch[1] = caretPos + 1 + (bals[ind].length() - it);
                    balFsearch[0] = caretPos - (2 + it);
                }
                it++;
            }
        }
        QRegExp pattD, pattF;
        pattD.setPattern("<" + bals[ind] + ">");
        pattF.setPattern("</" + bals[ind] + ">");
        bool balFound = false;
        if (balFsearch[1] > 0 && balFsearch[1] <= text.length() && balFsearch[0] >= 0 && balFsearch[0] < text.length()) {
            QString search = text.mid(balFsearch[0], balFsearch[1] - balFsearch[0]);
            if (search.contains(pattF)) {
                resetPosCaret = balFsearch[0] - (2 + bals[ind].length());
                int j = 3;
                bool foundSister = false;
                int goodSister = 0;
                while (balFsearch[0] - j >= 0 && !foundSister)
                {
                    int sisterId = text.left(balFsearch[0]).right(j).indexOf("<" + bals[ind] + ">");
                    int cosinId = text.left(balFsearch[0]).right(j).indexOf("</" + bals[ind] + ">");
                    if (cosinId == 0)
                    {
                        goodSister++;
                    }
                    if (sisterId == 0)
                    {
                        if (goodSister == 0)
                        {
                            foundSister = true;
                            sisterId = balFsearch[0] - j;
                            text = text.left(sisterId) + "<" + bals[ind].toUpper() + ">" + text.left(balFsearch[0]).right(text.left(balFsearch[0]).length() - (sisterId + 2 + bals[ind].length())) + "</" + bals[ind].toUpper() + ">" + text.right(text.length() - balFsearch[1]);
                            balise = bals[ind].toUpper();
                        }
                        else
                        {
                            goodSister--;
                        }
                    }
                    j++;
                }
                balFound = true;
            }
        }
        if (balDsearch[1] > 0 && balDsearch[1] <= text.length() && balDsearch[0] >= 0 && balDsearch[0] < text.length() && !balFound) {
            QString search = text.mid(balDsearch[0], balDsearch[1] - balDsearch[0]);
            if (search.contains(pattD)) {
                resetPosCaret = balDsearch[0];
                int sisterId = text.right(text.length() - balDsearch[1]).indexOf("</" + bals[ind] + ">") + balDsearch[1];
                int cosinId = text.right(text.length() - balDsearch[1]).indexOf("<" + bals[ind] + ">") + balDsearch[1];
                if (cosinId != -1 + balDsearch[1])
                {
                    bool goodSisterFound = false;
                    while (!goodSisterFound && sisterId > -1 && cosinId > -1)
                    {
                        if (cosinId < sisterId)
                        {
                            int followPossibleSister = text.right(text.length() - (sisterId + 1)).indexOf("</" + bals[ind] + ">");
                            if (followPossibleSister != -1)
                            {
                                sisterId += 1 + followPossibleSister;
                                cosinId = text.right(text.length() - (sisterId + 1)).indexOf("<" + bals[ind] + ">");
                            }
                            else
                            {
                                sisterId = -1;
                            }
                        }
                        else
                        {
                            goodSisterFound = true;
                        }
                    }
                }
                if (sisterId > -1)
                {
                    balise = bals[ind].toUpper();
                    text = text.left(balDsearch[0]) + "<" + balise + ">" + text.left(sisterId).right(text.left(sisterId).length() - balDsearch[1]) + "</" + balise + ">" + text.right(text.length() - (sisterId + 3 + bals[ind].length()));
                }
            }
        }
    ind++;
    }
    return text;
}

QString CeCWriter::checkRemoveBalSlct(QString text, QList<int> slct)
{
    QStringList bals;
    bals << "b" << "u" << "i" << "np" << "po";
    foreach (QString bal1, bals) {

        //Search <b> type balise:
        QString search = text.mid(qMax(0, slct[0] - (2 + bal1.length())), qMin(text.length(), slct[1] + (2 + bal1.length())));
        QRegExp exp;
        exp.setPattern("<" + bal1 + ">");
        int i = 0;
        while (search.right(search.length() - i).contains(exp)) {
            QString bal = exp.cap();
            if (search.right(search.length() - i).indexOf(bal) + i < slct[0]) {
                resetPosCaret -= qMax(0, slct[0] - (qMax(0, slct[0] - (2 + bal1.length())) + (search.right(search.length() - i).indexOf(bal) + i)));
            }
            QList<int> balDsearch;
            balDsearch << qMax(0, slct[0] - (2 + bal1.length())) + search.indexOf(exp);
            balDsearch << qMax(0, slct[0] - (2 + bal1.length())) + search.indexOf(exp) + (2 + bal1.length());
            int sisterId = text.right(text.length() - balDsearch[1]).indexOf("</" + bal1 + ">") + balDsearch[1];
            int cosinId = text.right(text.length() - balDsearch[1]).indexOf("<" + bal1 + ">") + balDsearch[1];
            if (cosinId != -1 + balDsearch[1])
            {
                bool goodSisterFound = false;
                while (!goodSisterFound && sisterId > -1 && cosinId > -1)
                {
                    if (cosinId < sisterId)
                    {
                        int followPossibleSister = text.right(text.length() - (sisterId + 1)).indexOf("</" + bal1 + ">");
                        if (followPossibleSister != -1)
                        {
                            sisterId += 1 + followPossibleSister;
                            cosinId = text.right(text.length() - (sisterId + 1)).indexOf("<" + bal1 + ">");
                        }
                        else
                        {
                            sisterId = -1;
                        }
                    }
                    else
                    {
                        goodSisterFound = true;
                    }
                }
            }
            if (sisterId > -1)
            {
                if (!balises.contains(bal1.toUpper()))
                    balises.append(bal1.toUpper());
                text = text.left(balDsearch[0]) + "<" + bal1.toUpper() + ">" + text.left(sisterId).right(text.left(sisterId).length() - balDsearch[1]) + "</" + bal1.toUpper() + ">" + text.right(text.length() - (sisterId + (3 + bal1.length())));
            }
            i += search.right(search.length() - i).indexOf(bal) + 1;
        }

        //Search </b> type balise:
        exp.setPattern("</" + bal1 + ">");
        i = 0;
        while (search.right(search.length() - i).contains(exp)) {
            QString bal = exp.cap();
            if (search.right(search.length() - i).indexOf(bal) + i < slct[0]) {
                resetPosCaret -= qMax(0, slct[0] - (qMax(0, slct[0] - (2 + bal1.length())) + (search.right(search.length() - i).indexOf(bal) + i)));
            }
            QList<int> balFsearch;
            balFsearch << qMax(0, slct[0] - (2 + bal1.length())) + search.indexOf(exp);
            balFsearch << qMax(0, slct[0] - (2 + bal1.length())) + search.indexOf(exp) + (3 + bal1.length());
            int j = 3;
            bool foundSister = false;
            int goodSister = 0;
            while (balFsearch[0] - j >= 0 && !foundSister)
            {
                int sisterId = text.left(balFsearch[0]).right(j).indexOf("<" + bal1 + ">");
                int cosinId = text.left(balFsearch[0]).right(j).indexOf("</" + bal1 + ">");
                if (cosinId == 0)
                {
                    goodSister++;
                }
                if (sisterId == 0)
                {
                    if (goodSister == 0)
                    {
                        foundSister = true;
                        sisterId = balFsearch[0] - j;
                        text = text.left(sisterId) + "<" + bal1.toUpper() + ">" + text.left(balFsearch[0]).right(text.left(balFsearch[0]).length() - (sisterId + (2 + bal1.length()))) + "</" + bal1.toUpper() + ">" + text.right(text.length() - balFsearch[1]);
                        if (!balises.contains(bal1.toUpper()))
                            balises.append(bal1.toUpper());
                        if (sisterId < slct[0] - (2 + bal1.length()))
                            resetPosCaret -= (2 + bal1.length());
                        else
                            resetPosCaret -= slct[0] - sisterId;
                    }
                    else
                    {
                        goodSister--;
                    }
                }
                j++;
            }
            i += search.right(search.length() - i).indexOf(bal) + 1;
        }
    }
    return text;
}

//3. CHECK DELETE ON FIELDS:

void CeCWriter::checkRemoveBalSupprInLineEdit(QLineEdit *areaTxt)
{
    //Get character that will be deleted:
    int caretPos = areaTxt->cursorPosition();
    if (caretPos >= 0)
    {
        checkRemoveLnkSuppr(areaTxt->text(), caretPos);
        QString text = checkRemoveBalSuppr(areaTxt->text(), caretPos);
        if (balise != "")
            areaTxt->setText(text);
    }
}

void CeCWriter::checkRemoveBalSupprInPlainTextEdit(QPlainTextEdit *areaTxt)
{
    //Get character that will be deleted:
    int caretPos = areaTxt->textCursor().position();
    if (caretPos >= 0)
    {
        checkRemoveLnkSuppr(areaTxt->toPlainText(), caretPos);
        QString text = checkRemoveBalSuppr(areaTxt->toPlainText(), caretPos);
        if (balise != "")
            areaTxt->setPlainText(text);
    }
}

void CeCWriter::checkRemoveBalBckpInLineEdit(QLineEdit* areaTxt)
{
    //Get character that will be deleted:
    int caretPos = areaTxt->cursorPosition();
    if (caretPos >= 0)
    {
        checkRemoveLnkBckp(areaTxt->text(), caretPos);
        QString text = checkRemoveBalBckp(areaTxt->text(), caretPos);
        if (balise != "")
            areaTxt->setText(text);
    }
}

void CeCWriter::checkRemoveBalBckpInPlainTextEdit(QPlainTextEdit *areaTxt)
{
    //Get character that will be deleted:
    int caretPos = areaTxt->textCursor().position();
    if (caretPos >= 0)
    {
        checkRemoveLnkBckp(areaTxt->toPlainText(), caretPos);
        QString text = checkRemoveBalBckp(areaTxt->toPlainText(), caretPos);
        if (balise != "")
            areaTxt->setPlainText(text);
    }
}

void CeCWriter::checkRemoveBalSlctInLineEdit(QLineEdit* areaTxt)
{
    QString text = areaTxt->text();
    QList<int> slct;
    slct << areaTxt->selectionStart() << areaTxt->selectionStart() + areaTxt->selectedText().length();
    checkRemoveLnkSlct(text, slct);
    QString newText = checkRemoveBalSlct(text, slct);
    if (balises.size() > 0)
    {
        areaTxt->setText(newText);
        areaTxt->setSelection(slct[0], slct[1] - slct[0]);
    }
}

void CeCWriter::checkRemoveBalSlctInPlainTextEdit(QPlainTextEdit* areaTxt)
{
    QString text = areaTxt->toPlainText();
    QList<int> slct;
    slct << areaTxt->textCursor().selectionStart() << areaTxt->textCursor().selectionEnd();
    checkRemoveLnkSlct(text, slct);
    QString newText = checkRemoveBalSlct(text, slct);
    if (balises.size() > 0)
    {
        areaTxt->setPlainText(newText);
        QTextCursor cursor = areaTxt->textCursor();
        cursor.setPosition(slct[0]);
        cursor.setPosition(slct[1], QTextCursor::KeepAnchor);
        areaTxt->setTextCursor(cursor);
    }
}

//4. DO DELETE BALISES ON FIELDS:

void CeCWriter::deleteBalInLineEdit(QLineEdit* areaTxt)
{
    QString initialTxt = areaTxt->text().replace("<" + balise + ">", "");
    initialTxt = initialTxt.replace("</" + balise + ">", "");
    areaTxt->setText(initialTxt);
    areaTxt->setCursorPosition(resetPosCaret);
    balise = "";
}

void CeCWriter::deleteBalInPlainTextEdit(QPlainTextEdit* areaTxt)
{
    QString initialTxt = areaTxt->toPlainText().replace("<" + balise + ">", "");
    initialTxt = initialTxt.replace("</" + balise + ">", "");
    areaTxt->setPlainText(initialTxt);
    QTextCursor cursor = areaTxt->textCursor();
    cursor.setPosition(resetPosCaret);
    areaTxt->setTextCursor(cursor);
    balise = "";
}

void CeCWriter::deleteBalsInLineEdit(QLineEdit* areaTxt)
{
    QList<int> slct;
    slct << areaTxt->selectionStart() << areaTxt->selectionStart() + areaTxt->selectedText().length();
    QString initialTxt = areaTxt->text().mid(0, slct[0]) + "仝" + areaTxt->text().mid(slct[0], slct[1] - slct[0]) + "仝" + areaTxt->text().mid(slct[1], areaTxt->text().length() - slct[1]);
    foreach (QString balLink, balises) {
        initialTxt = initialTxt.replace("<" + balLink + ">", "");
        initialTxt = initialTxt.replace("<仝" + balLink + ">", "仝");
        initialTxt = initialTxt.replace("<" + balLink + "仝>", "仝");
        for (int i = 1; i < balLink.length(); ++i) {
            initialTxt = initialTxt.replace("<" + balLink.mid(0, i) + "仝" + balLink.mid(i, balLink.length())+ ">", "仝");
        }
        initialTxt = initialTxt.replace("</" + balLink + ">", "");
        initialTxt = initialTxt.replace("</仝" + balLink + ">", "仝");
        initialTxt = initialTxt.replace("<仝/" + balLink + ">", "仝");
        initialTxt = initialTxt.replace("</" + balLink + "仝>", "仝");
        for (int i = 1; i < balLink.length(); ++i) {
            initialTxt = initialTxt.replace("</" + balLink.mid(0, i) + "仝" + balLink.mid(i , balLink.length())+ ">", "仝");
        }
    }
    int dbt = initialTxt.indexOf("仝");
    int fin = initialTxt.right(initialTxt.length() - (dbt + 1)).indexOf("仝") + dbt + 2;
    initialTxt = initialTxt.replace(initialTxt.mid(dbt, fin - dbt), "");
    areaTxt->setText(initialTxt);
    areaTxt->setCursorPosition(resetPosCaret);
    balises.clear();
}

void CeCWriter::deleteBalsInPlainTextEdit(QPlainTextEdit* areaTxt)
{
    QList<int> slct;
    slct << areaTxt->textCursor().selectionStart() << areaTxt->textCursor().selectionEnd();
    QString initialTxt = areaTxt->toPlainText().mid(0, slct[0]) + "仝" + areaTxt->toPlainText().mid(slct[0], slct[1] - slct[0]) + "仝" + areaTxt->toPlainText().mid(slct[1], areaTxt->toPlainText().length() - slct[1]);
    foreach (QString balLink, balises) {
        initialTxt = initialTxt.replace("<" + balLink + ">", "");
        initialTxt = initialTxt.replace("<仝" + balLink + ">", "仝");
        initialTxt = initialTxt.replace("<" + balLink + "仝>", "仝");
        for (int i = 1; i < balLink.length() - 1; ++i) {
            initialTxt = initialTxt.replace("<" + balLink.mid(0, i) + "仝>" + balLink.mid(i + 1, balLink.length() - (i + 1)), "仝");
        }
        initialTxt = initialTxt.replace("</" + balLink + ">", "");
        initialTxt = initialTxt.replace("</仝" + balLink + ">", "仝");
        initialTxt = initialTxt.replace("<仝/" + balLink + ">", "仝");
        initialTxt = initialTxt.replace("</" + balLink + "仝>", "仝");
        for (int i = 1; i < balLink.length() - 1; ++i) {
            initialTxt = initialTxt.replace("</" + balLink.mid(0, i) + "仝>" + balLink.mid(i + 1, balLink.length() - (i + 1)), "仝");
        }
    }
    int dbt = initialTxt.indexOf("仝");
    int fin = initialTxt.right(initialTxt.length() - (dbt + 1)).indexOf("仝") + dbt + 2;
    initialTxt = initialTxt.replace(initialTxt.mid(dbt, fin - dbt), "");
    areaTxt->setPlainText(initialTxt);
    QTextCursor cursor = areaTxt->textCursor();
    cursor.setPosition(resetPosCaret);
    areaTxt->setTextCursor(cursor);
    balises.clear();
}

/***
 * On selection changed : (des)activating lien button
 ***/

void CeCWriter::on_editIngr_selectionChanged()
{
    if (ui->editIngr->selectedText().length() > 0) {
        ui->lienButton->setEnabled(true);
    }
    else {
        ui->lienButton->setEnabled(false);
    }
}

void CeCWriter::on_editMat_selectionChanged()
{
    if (ui->editMat->selectedText().length() > 0) {
        ui->lienButton->setEnabled(true);
    }
    else {
        ui->lienButton->setEnabled(false);
    }
}

void CeCWriter::on_editPrep_selectionChanged()
{
    if (ui->editPrep->textCursor().selectedText().length() > 0) {
        ui->lienButton->setEnabled(true);
    }
    else {
        ui->lienButton->setEnabled(false);
    }
}

void CeCWriter::on_editCons_selectionChanged()
{
    if (ui->editCons->textCursor().selectedText().length() > 0) {
        ui->lienButton->setEnabled(true);
    }
    else {
        ui->lienButton->setEnabled(false);
    }
}

void CeCWriter::on_description_selectionChanged()
{
    if (ui->description->textCursor().selectedText().length() > 0) {
        ui->lienButton->setEnabled(true);
    }
    else {
        ui->lienButton->setEnabled(false);
    }
}
