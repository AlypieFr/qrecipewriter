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
    if (version != "") {
        QStringList lines = version.split("\n");
        version = lines[0].replace("\r", "");
        QStringList lVerNb = version.split(".");
        int i = 0;
        bool isVersionCorrect = true;
        while (i < lVerNb.length() && isVersionCorrect) {
            bool ok;
            lVerNb[i].toInt(&ok);
            if (!ok) {
                isVersionCorrect = false;
            }
            i++;
        }
        if (isVersionCorrect) {
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
                    int rep = QMessageBox::information(_parent, tr("Mise à jour disponible"), QString(tr("Une mise à jour du logicielle est disponible, voulez-vous "))
                                                       + QString(tr("mettre à jour votre version ")) + VERSION + QString(tr(" à la dernière version") + " (" + version + tr(") ?")), QMessageBox::Yes, QMessageBox::No);
                    if (rep == QMessageBox::Yes)
                    {
                        QString adresse = lines[1].left(lines[1].length() - 1);
                        Functions::downloadUpdate(adresse, _parent);
                    }
                }
            }
        }
        else {
            qWarning() << "Error: unable to check version, the newest version from the net was not correct";
        }
    }
    else {
        qWarning() << "Warning: unable to check for updates. May be you are not connecter to the internet.";
    }
}
