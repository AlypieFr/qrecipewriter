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

#include "categories.h"
#include "ui_categories.h"

extern QFile *confCatFile;

Categories::Categories(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Categories)
{
    ui->setupUi(this);
}

Categories::~Categories()
{
    delete ui;
}

void Categories::keyPressEvent(QKeyEvent *e)
{
    //Disable hide dialog by press escape
    if(e->key() != Qt::Key_Escape)
        QDialog::keyPressEvent(e);
}

void Categories::reset()
{
    idCatList = 0;
    catList.clear();

    wid = new QWidget(ui->scroll_categories); //Main widget of the scrollArea

    //Creating first line:
    QLineEdit *line1 = new QLineEdit();
    line1->setMinimumHeight(30);
    line1->setText(tr("Entrée"));
    line1 ->setAlignment(Qt::AlignCenter);
    QHBoxLayout *hLay = new QHBoxLayout(wid);
    hLay->setAlignment(Qt::AlignTop);
    hLay->addWidget(line1);
    hLay->addSpacing(30 + hLay->margin());
    hLay->setMargin(0);
    QWidget *wid1 = new QWidget();
    wid1->setLayout(hLay);
    vLay = new QVBoxLayout(wid);
    vLay->setAlignment(Qt::AlignTop);
    vLay->addWidget(wid1);
    catList.insert(idCatList, line1);
    idCatList++;

    //Creating other lines:
    for (int wi = 0; wi < 7; ++wi) {
        QPushButton* but1 = new QPushButton("-");
        but1->setMinimumSize(30,30);
        but1->setMaximumSize(30,30);
        connect(but1, SIGNAL(released()), &mapper, SLOT(map()));
        mapper.setMapping(but1, idCatList);
        QLineEdit *line2 = new QLineEdit();
        line2 ->setAlignment(Qt::AlignCenter);
        if (wi==0){
            line2->setText(tr("Plat principal"));
        }

        if (wi==1){
            line2->setText(tr("Dessert"));
        }


        if (wi==2){
            line2->setText(tr("Accompagnement"));
        }


        if (wi==3){
            line2->setText(tr("Base"));
        }


        if (wi==4){
            line2->setText(tr("Biscuits & Friandises"));
        }


        if (wi==5){
            line2->setText(tr("Pain & Viennoiserie"));
        }


        if (wi==6){
            line2->setText(tr("Divers"));
        }

        line2->setMinimumHeight(30);
        QWidget *wid2 = new QWidget();
        QHBoxLayout *hLay2 = new QHBoxLayout(wid2);
        hLay2->setAlignment(Qt::AlignTop);
        hLay2->addWidget(line2);
        hLay2->addWidget(but1);
        hLay2->setMargin(0);
        vLay->addWidget(wid2);
        sendW.insert(idCatList, wid2);
        catList.insert(idCatList, line2);
        idCatList++;
    }



    //Creating last line: the "+" button:
    QPushButton* addCat = new QPushButton("+");
    addCat->setMinimumSize(30,30);
    addCat->setMaximumSize(30,30);
    connect(addCat, SIGNAL(released()), this, SLOT(addCatPushed()));
    vLay->addWidget(addCat);
    vLay->setMargin(0);
    //Attribute main widget to scrollArea:
    ui->scroll_categories->setWidget(wid);
}

void Categories::on_annuler_clicked()
{
    close();
}

void Categories::initCats(QStringList cats)
{
    idCatList = 0;
    catList.clear();

    wid = new QWidget(ui->scroll_categories); //Main widget of the scrollArea

    //Creating first line:
    QLineEdit *line1 = new QLineEdit();
    line1->setMinimumHeight(30);
    line1->setText(cats[0].replace("&&", "&"));
    line1 ->setAlignment(Qt::AlignCenter);
    QHBoxLayout *hLay = new QHBoxLayout(wid);
    hLay->setAlignment(Qt::AlignTop);
    hLay->addWidget(line1);
    hLay->addSpacing(30 + hLay->margin());
    hLay->setMargin(0);
    QWidget *wid1 = new QWidget();
    wid1->setLayout(hLay);
    vLay = new QVBoxLayout(wid);
    vLay->setAlignment(Qt::AlignTop);
    vLay->addWidget(wid1);
    catList.insert(idCatList, line1);
    idCatList++;
    for (int el = 1; el < cats.size(); ++el) {
        QPushButton* but1 = new QPushButton("-");
        but1->setMinimumSize(30,30);
        but1->setMaximumSize(30,30);
        connect(but1, SIGNAL(released()), &mapper, SLOT(map()));
        mapper.setMapping(but1, idCatList);
        QLineEdit *line2 = new QLineEdit();
        line2 ->setAlignment(Qt::AlignCenter);
        line2->setText(cats[el].replace("&&", "&"));
        line2->setMinimumHeight(30);
        QWidget *wid2 = new QWidget();
        QHBoxLayout *hLay2 = new QHBoxLayout(wid2);
        hLay2->setAlignment(Qt::AlignTop);
        hLay2->addWidget(line2);
        hLay2->addWidget(but1);
        hLay2->setMargin(0);
        vLay->addWidget(wid2);
        sendW.insert(idCatList, wid2);
        catList.insert(idCatList, line2);
        idCatList++;
    }
    //Creating last line: the "+" button:
    QPushButton* addCat = new QPushButton("+");
    addCat->setMinimumSize(30,30);
    addCat->setMaximumSize(30,30);
    connect(addCat, SIGNAL(released()), this, SLOT(addCatPushed()));
    vLay->addWidget(addCat);
    vLay->setMargin(0);
    //Attribute main widget to scrollArea:
    ui->scroll_categories->setWidget(wid);
}

void Categories::init(QStringList cats)
{
    connect(&mapper, SIGNAL(mapped(int)), this, SLOT(delCatPushed(int)));
    idCatList = 0;
    ui->scroll_categories->setStyleSheet("QScrollArea {border: none;}");
    initCats(cats);
}

namespace Ui {
class Categories;
}

void Categories::delCatPushed(int id)
{
    delete sendW[id];
    sendW.remove(id);
    catList.remove(id);
}

void Categories::addCatPushed()
{
    QPushButton* but1 = new QPushButton("-");
    but1->setMinimumSize(30,30);
    but1->setMaximumSize(30,30);
    connect(but1, SIGNAL(released()), this, SLOT(delCatPushed()));
    QLineEdit *line2 = new QLineEdit();
    line2->setMinimumHeight(30);
    line2->setAlignment(Qt::AlignCenter);
    QWidget *wid2 = new QWidget();
    QHBoxLayout *hLay2 = new QHBoxLayout(wid2);
    hLay2->setAlignment(Qt::AlignTop);
    hLay2->addWidget(line2);
    hLay2->addWidget(but1);
    hLay2->setMargin(0);
    vLay->insertWidget(vLay->count()-1,wid2);
    sendW.insert(idCatList, wid2);
    catList.insert(idCatList, line2);
    idCatList++;
    line2->setFocus();
}

void Categories::on_valeurs_def_clicked()
{
    delete wid;
    reset();
}

void Categories::on_valider_clicked()
{
    QStringList new_cats;
    foreach (QLineEdit* catL, catList.values()) {
        new_cats.append(catL->text());
    }
    Functions::write_categories_file(new_cats);
    this->close();
}
