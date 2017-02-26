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

#ifndef HTTPREQUESTWORKER_H
#define HTTPREQUESTWORKER_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "filenotfoundexception.h"
#include "globalexception.h"


enum HttpRequestVarLayout {NOT_SET, ADDRESS, URL_ENCODED, MULTIPART};


class HttpRequestInputFileElement {

public:
    QString variable_name;
    QString local_filename;
    QString request_filename;
    QString mime_type;

};


class HttpRequestInput {

public:
    QString url_str;
    QString http_method;
    QString username;
    QString password;
    HttpRequestVarLayout var_layout;
    QMap<QString, QString> vars;
    QList<HttpRequestInputFileElement> files;

    HttpRequestInput();
    HttpRequestInput(QString v_url_str, QString v_http_method, QString v_username, QString v_password);
    HttpRequestInput(QString v_url_str, QString v_http_method);
    void initialize();
    void add_var(QString key, QString value);
    void add_file(QString variable_name, QString local_filename, QString request_filename, QString mime_type);

};


class HttpRequestWorker : public QObject {
    Q_OBJECT

public:
    QByteArray response;
    QNetworkReply::NetworkError error_type;
    QString error_str;

    explicit HttpRequestWorker(QObject *parent = 0);

    QString http_attribute_encode(QString attribute_name, QString input);
    void execute(HttpRequestInput *input);

signals:
    void on_execution_finished(HttpRequestWorker *worker);

private:
    QNetworkAccessManager *manager;

private slots:
    void on_manager_finished(QNetworkReply *reply);

};

#endif // HTTPREQUESTWORKER_H
