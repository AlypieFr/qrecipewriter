#include "headers/units.h"
#include "ui_units.h"

extern QString confDir;
extern QStringList units;

Units::Units(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Units)
{
    ui->setupUi(this);
}

Units::~Units()
{
    delete ui;
}

void Units::init() {
    ui->units_input->setText(units.join(","));
}

void Units::on_buttonBox_accepted()
{
    units = ui->units_input->text().split(QRegExp("\\s*,\\s*"));
    this->saveUnits();
}

void Units::saveUnits() {
    QString filename = confDir + "/.units";
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << ui->units_input->text() << endl;
    }
    else {
        QMessageBox::critical((QWidget*)this->parent(), tr("Sauvegarde impossible"), tr("Impossible de sauvegarder les unités."));
    }
}
