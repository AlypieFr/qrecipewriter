/*
 * © 2013-2016 Flo-Art.fr
 *
 * QRecipeWriter et l'ensemble de ses putils est fournit sous la licence Creative Common BY-NC-SA.
 * Toutes les modifications et la redistribution sont autorisés pour une utilisation NON COMMERCIALE.
 * Par ailleurs, les modifications et la reproduction doivent respecter les règles ci-dessous :
 *    - Cette en-tête doit être maintenue.
 *    - Vous devez redistribuer la version modifiée ou non sous licence Creative Common au moins autant
 *      restrictive.
 *    - Flo-Art.fr ne peut être tenu pour responsable des versions modifiées et/ou redistribuées.
 *    - Toute utilisation commerciale partielle ou complète est interdite.
 */

#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl url, QString message, QWidget *parent) :
    QWidget(parent)
{
    wait = new QDialog(parent);
    QHBoxLayout *lay = new QHBoxLayout();
    QLabel *lab = new QLabel(message);
    lab->setAlignment(Qt::AlignHCenter);
    lay->addWidget(lab);
    wait->setLayout(lay);
    wait->setModal(true);
    connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply*)), SLOT(fileDownloaded(QNetworkReply*)));
    QNetworkRequest request(url);
    m_WebCtrl.get(request);
    wait->exec();
}

FileDownloader::~FileDownloader()
{
}

void FileDownloader::fileDownloaded(QNetworkReply *pReply)
{
    m_DownloadedData = pReply->readAll();
    //emit a signal:
    pReply->deleteLater();
    emit downloaded();
    wait->close();
}

QByteArray FileDownloader::downloadedData() const
{
    return m_DownloadedData;
}
