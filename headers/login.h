#ifndef LOGIN_H
#define LOGIN_H

#include <QDebug>
#include <QDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();

private slots:
    void on_valider_clicked();

    void on_annuler_clicked();

public:
    void init(bool showPublish=true);
    bool getAccepted();
    QString getUsername();
    QString getPassword();
    bool getPublish();

private:
    Ui::Login *ui;
    bool accepted;
    QString username;
    QString password;
    bool publish;
};

#endif // LOGIN_H
