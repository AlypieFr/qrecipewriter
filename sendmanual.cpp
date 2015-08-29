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

#include "sendmanual.h"
#include "ui_sendmanual.h"

extern QString userDir;
extern QString addrSite;
extern QString cmdNav;

SendManual::SendManual(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendManual)
{
    ui->setupUi(this);
}

SendManual::~SendManual()
{
    delete ui;
}

void SendManual::init(QString titre_lu, QString htmlCode_lu, QString excerpt_lu, QStringList imagesToAdd_lu, QStringList categories_lu)
{
    titre = titre_lu;
    htmlCode = htmlCode_lu;
    excerpt = excerpt_lu;
    imagesToAdd = imagesToAdd_lu;
    categories = categories_lu;
    this->setWindowTitle(titre_lu + " - Envoi Manuel");
    ui->titre->clear();
    ui->titre->setText(titre_lu);
    ui->htmlCode->clear();
    ui->htmlCode->setPlainText(htmlCode_lu);
    ui->excerpt->setPlainText(excerpt_lu);
    ui->imagesToAdd->clear();
    foreach (QString img, imagesToAdd_lu) {
        ui->imagesToAdd->appendPlainText(img);
    }
    QStandardItemModel *modelCats = new QStandardItemModel(this);
    ui->categories->setModel(modelCats);
    ui->categories->setTextElideMode(Qt::ElideNone);
    ui->categories->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->categories->setFocusPolicy(Qt::NoFocus);
    foreach (QString cat, categories_lu) {
        modelCats->appendRow(new QStandardItem(cat.replace("&amp;", "&")));
    }
    this->exec();
}

void SendManual::on_openInline_clicked()
{
    QString Program = "\"" + cmdNav + "\" " + addrSite + "/wp-admin/post-new.php";
    QProcess *myProcess = new QProcess();
    myProcess->setProcessChannelMode(QProcess::MergedChannels);
    myProcess->start(Program);
}

void SendManual::on_closeDialog_clicked()
{
    this->close();
}

void SendManual::on_saveInFile_clicked()
{
    QString *ext = new QString("*.txt");
    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer",
    userDir + "/" + titre.replace(" ", "_") + ".txt",
    "Fichier texte (*.txt) (*.txt)", ext);
    if (fileName != "")
    {
        if (!fileName.endsWith(".txt", Qt::CaseInsensitive))
            fileName.append(".txt");
        QFile saveFile (fileName);
        saveFile.open(QFile::WriteOnly);
        QTextStream in (&saveFile);
        in.setCodec("UTF-8"); //Not useful on linux system as it's a default, but Windows has its own defaults....
        QString cats = "";
        foreach (QString cat, categories) {
            cats.append(cat.replace("&amp;", "&") + "\n");
        }
        QString imgToAdd = "";
        foreach (QString img, imagesToAdd) {
            imgToAdd.append(img + "\n");
        }
        in << "TITRE :\n" << titre << "\n\n\nCATEGORIES :\n" << cats << "\n\nIMAGES A AJOUTER :\n" << imgToAdd << "\n\nEXCERPT :\n" << excerpt << "\n\n\nCODE HTML (corps) :\n" << htmlCode << endl;
        saveFile.close();
    }
}

void SendManual::on_copyTitre_clicked()
{
    QClipboard *pressePapiers = QApplication::clipboard();
    pressePapiers->setText(ui->titre->text());
}

void SendManual::on_copyHtmlCode_clicked()
{
    QClipboard *pressePapiers = QApplication::clipboard();
    pressePapiers->setText(ui->htmlCode->toPlainText());
}

void SendManual::on_copyExcerpt_clicked()
{
    QClipboard *pressePapiers = QApplication::clipboard();
    pressePapiers->setText(ui->excerpt->toPlainText());
}
