#include "filedownloader.h"

#include <QDebug>

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
