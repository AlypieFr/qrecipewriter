#include "searchupdates.h"

extern QString updtUrl;
extern QString VERSION;

SearchUpdates::SearchUpdates(QWidget *parent) :
    QWidget (parent)
{
    _parent = parent;
}

void SearchUpdates::start() {
    manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*)));
    QNetworkRequest request(updtUrl);
    manager->get(request);
}

void SearchUpdates::replyFinished(QNetworkReply *pReply)
{
    QString version (pReply->readAll());
    QStringList lines = version.split("\n");
    version = lines[0].replace("\r", "");
    QStringList lVerNb = version.split(".");
    QStringList tVerNb = VERSION.split(".");
    bool hasUpdate = false;
    int niv = 0;
    while (!hasUpdate && niv < qMin(lVerNb.size(), tVerNb.size()))
    {
        if (lVerNb[niv] > tVerNb[niv])
            hasUpdate = true;
        else if(lVerNb[niv] < tVerNb[niv])
            niv = qMin(lVerNb.size(), tVerNb.size()); //We stop the loop
        niv++;
    }
    if (hasUpdate) {
        if (hasUpdate)
        {
            //Propose to user downloading the last version
            int rep = QMessageBox::information(_parent, "Mise à jour disponible", QString("Une mise à jour du logicielle est disponible, voulez-vous ")
                                               + QString("mettre à jour votre version ") + VERSION + QString(" à la dernière version (" + version + ") ?"), QMessageBox::Yes, QMessageBox::No);
            if (rep == QMessageBox::Yes)
            {
                QString adresse = lines[1].left(lines[1].length() - 1);
                Functions::downloadUpdate(adresse, _parent);
            }
        }
    }
}
