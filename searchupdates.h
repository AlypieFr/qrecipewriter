#ifndef SEARCHUPDATES_H
#define SEARCHUPDATES_H

#include "functions.h"

#include <QWidget>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QDateTime>
#include <QFile>
#include <QObject>

class SearchUpdates : public QWidget
{
    Q_OBJECT
public:
    explicit SearchUpdates(QWidget *parent=0);
    void start();

signals:

private slots:
    void replyFinished(QNetworkReply* pReply);

private:
    QNetworkAccessManager *manager;
    QWidget *_parent;
};

#endif // SEARCHUPDATES_H
