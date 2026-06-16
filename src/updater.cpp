#include "updater.h"
#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkRequest>
#include <QUrl>

#include <QVersionNumber>

#include <QProcess>

//constructor
Updater::Updater(QObject *parent, QNetworkAccessManager* networkManager)
    : QObject(parent), m_networkManager(networkManager)
{}

//checks for updates asynchronous
void Updater::checkForUpdates()
{
    //url to version.json
    QUrl url("https://raw.githubusercontent.com/AnonymousWombatx/SyroySync/main/version.json");

    // create and send request
    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onReplyFinished(reply);
    });

}

void Updater::updateYtDlp(const QString &ytDlpPath)
{
    auto* process = new QProcess(this);

    connect(process, &QProcess::finished, this, [this,process](int exitCode, QProcess::ExitStatus status) {
        if(status ==QProcess::NormalExit && exitCode ==0)
            emit ytDlpUpdateFinished();
        else
            process->deleteLater();
    });

    process->start(ytDlpPath, {"-U"});
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

    //emit signals
    if (version != m_latestVersion) {
        m_latestVersion = version;
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
