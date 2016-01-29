#include "qinputmultiline.h"
#include "ui_qinputmultiline.h"

QInputMultiline::QInputMultiline(QWidget *parent, QString title, QString message, QString text, bool wordWrap, int width, int height) :
    QDialog(parent),
    ui(new Ui::QInputMultiline)
{
    ui->setupUi(this);
    this->setWindowTitle(title);
    ui->message->setText(message + " :");
    if (!wordWrap)
        ui->text->setWordWrapMode(QTextOption::NoWrap);
    ui->text->setPlainText(text);
    this->text = "";
    this->setBaseSize(width, height);
}

QInputMultiline::~QInputMultiline()
{
    delete ui;
}

void QInputMultiline::on_buttonBox_accepted()
{
    this->text = ui->text->toPlainText();
    this->close();
}

void QInputMultiline::on_buttonBox_rejected()
{
    this->close();
}
