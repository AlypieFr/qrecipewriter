#ifndef INSERTVIDEO_H
#define INSERTVIDEO_H

#include <QCloseEvent>
#include <QDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QRegExp>
#include <qmath.h>

namespace Ui {
class InsertVideo;
}

class InsertVideo : public QDialog
{
    Q_OBJECT

public:
    explicit InsertVideo(QWidget *parent = 0);
    ~InsertVideo();
    void init(int largeur, int hauteur, int startTime, int bordure, bool fullscreen, bool suggestions, bool commandes, bool titreVideo, bool confidentialite, QString idVideo);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_importFromHtml_clicked();

private:
    Ui::InsertVideo *ui;
    bool checkUrl(QString url);

public:
    bool ok;
    QString urlVideo, idVideo;
    int largeur, hauteur, startTime, bordure;
    bool fullscreen, suggestions, commandes, titreVideo, confidentialite;
};

#endif // INSERTVIDEO_H
