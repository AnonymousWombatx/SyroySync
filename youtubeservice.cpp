#include "youtubeservice.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

YoutubeService::YoutubeService(QObject *parent, QNetworkAccessManager* networkManager)
    : QObject(parent), m_networkManager(networkManager)
{
    //Connect Qt signal finished to slot onReplyFinished
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &YoutubeService::onReplyFinished);
}

void YoutubeService::onReplyFinished(QNetworkReply *reply)
{
    //Check for network errors and delete reply after event loop
    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }

    emit youtubeUrlFinished();

    reply->deleteLater();
}