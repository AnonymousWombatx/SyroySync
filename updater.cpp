#include "updater.h"
#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkRequest>
#include <QUrl>

#include <QVersionNumber>

//constructor
Updater::Updater(QObject *parent)
    : QObject{parent}
{
    //Connect Qt signal finished to slot onReplyFinished
    connect(&m_networkManager, &QNetworkAccessManager::finished, this, &Updater::onReplyFinished);

}

//checks for updates asynchronous
void Updater::checkForUpdates()
{
    //url to version.json
    QUrl url("https://raw.githubusercontent.com/AnonymousWombatx/SyroySync/main/version.json");

    // create and send request
    QNetworkRequest request (url);
    m_networkManager.get(request);

}

//Getter functions for QML-variables
QString Updater::latestVersion() const
{
    return m_latestVersion;
}

bool Updater::updateAvailable() const
{
    return m_updateAvailable;
}

//executes when HTTP request finishes (signal QNetworkAccessManager::finished)
void Updater::onReplyFinished(QNetworkReply *reply)
{
    //Check for network errors and delete reply after event loop
    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }

    //read the downloaded file as byte array
    QByteArray data = reply->readAll();

    //parse byte array to JSON
    QJsonDocument doc = QJsonDocument::fromJson(data);

    //extract top-level JSON-object
    QJsonObject obj = doc.object();
    //extract version from JSON
    QString version = obj["version"].toString();

    m_latestVersion = version;

    //emit signals
    if (version != m_latestVersion) {
        emit latestVersionChanged();
    }

    //convert version from string to vector
    QVersionNumber latest = QVersionNumber::fromString(version);
    QVersionNumber current = QVersionNumber::fromString(APP_VERSION);

    if (latest > current) {
        m_updateAvailable = true;
        emit updateAvailableChanged();
    }

    emit updateCheckFinished();

    reply->deleteLater();
}
