#ifndef QINPUTMULTILINE_H
#define QINPUTMULTILINE_H

#include <QDialog>

namespace Ui {
class QInputMultiline;
}

class QInputMultiline : public QDialog
{
    Q_OBJECT

public:
    explicit QInputMultiline(QWidget *parent = 0, QString title = "Dialog", QString message = "Entrez le texte", QString text = "", bool wordWrap = true, int width=300, int height=400);
    ~QInputMultiline();

private:
    Ui::QInputMultiline *ui;

public:
    QString text;
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
};

#endif // QINPUTMULTILINE_H
