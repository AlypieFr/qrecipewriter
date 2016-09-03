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

#include "insertpicture.h"
#include "ui_insertpicture.h"

extern QString dirPict;
extern bool recPrinter;

InsertPicture::InsertPicture(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertPicture)
{
    ui->setupUi(this);
}

void InsertPicture::init(QString imgFile, QString afficher, int width, int height, QString filename, bool center) {
    ok = false;
    //Icon of keepRation button:
    QColor colorBut = ui->keepRatio->palette().color(QPalette::Background);
    if ((colorBut.red() + colorBut.green() + colorBut.blue()) / 3 < 127.5)
    {
        ui->keepRatio->setIcon(QPixmap(":/images/media_aspect_ratio_blanc.png"));
    }

    //Set width and height;
    if (width == -1 || height == -1) {
        QPixmap* img = new QPixmap(imgFile);
        w = img->width();
        h = img->height();
        delete img;
        img = NULL;
    }
    else {
        w = width;
        h = height;
    }
    ui->largeur->setValue(w);
    ui->hauteur->setValue(h);
    ui->filename->setText(filename);

    //Set conf affichage :
    if (afficher == "all") {
        ui->printAll->setChecked(true);
    }
    else if (afficher == "noprint") {
        ui->noPrint->setChecked(true);
    }
    else if (afficher == "printonly") {
        ui->printOnly->setChecked(true);
    }
    if (center) {
        ui->centerImg->setChecked(true);
    }
    if (!recPrinter) {
        ui->noPrint->setDisabled(true);
        ui->noPrint->setToolTip("Nécessite d'activer CeC printer");
        ui->printOnly->setDisabled(true);
        ui->printOnly->setToolTip("Nécessite d'activer CeC printer");
    }
}

InsertPicture::~InsertPicture()
{
    delete ui;
}

void InsertPicture::on_buttonBox_accepted()
{
    ok = true;
    if (ui->printAll->isChecked()) {
        print = "all";
    }
    else if (ui->printOnly->isChecked()) {
        print = "printonly";
    }
    else if (ui->noPrint->isChecked()) {
        print = "noprint";
    }
    else {
        ok = false;
    }
    if (ui->centerImg->isChecked()) {
        centerImg = true;
    }
    else {
        centerImg = false;
    }
    filename = ui->filename->text();
    if (filename != "") {
        largeur = QString::number(ui->largeur->value());
        hauteur = QString::number(ui->hauteur->value());
        this->close();
    }
    else {
        QMessageBox::critical(this, "Fichier image requis", "Veuillez entrer un nom de fichier image !");
    }
}

void InsertPicture::on_buttonBox_rejected()
{
    this->close();
}

void InsertPicture::on_largeur_valueChanged(int arg1)
{
    if (ui->keepRatio->isChecked() && ui->largeur->hasFocus()) {
        ui->hauteur->setValue((arg1 * h) / w);
    }
}

void InsertPicture::on_hauteur_valueChanged(int arg1)
{
    if (ui->keepRatio->isChecked() && ui->hauteur->hasFocus()) {
        ui->largeur->setValue((arg1 * w) / h);
    }
}

void InsertPicture::on_changePicture_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Choisir une image",
    dirPict,
    "Images : *.jpg, *.JPG (*.jpg *.JPG)");
    if (fileName != "") {
        ui->filename->setText(fileName);
        QPixmap* img = new QPixmap(fileName);
        w = img->width();
        h = img->height();
        delete img;
        img = NULL;
        ui->largeur->setValue(w);
        ui->hauteur->setValue(h);
    }
}
