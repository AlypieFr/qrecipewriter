#ifndef INSERTPICTURE_H
#define INSERTPICTURE_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

namespace Ui {
class InsertPicture;
}

class InsertPicture : public QDialog
{
    Q_OBJECT

public:
    explicit InsertPicture(QWidget *parent = 0);
    ~InsertPicture();

private:
    Ui::InsertPicture *ui;

public:
    int w,h;
    bool ok;
    void init(QString imgFile, QString afficher, int width, int height, QString filename);
    QString print, largeur, hauteur, filename;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_largeur_valueChanged(int arg1);
    void on_hauteur_valueChanged(int arg1);
    void on_changePicture_clicked();
};

#endif // INSERTPICTURE_H
