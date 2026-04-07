#include "youtubeservice.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QCoreApplication>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonArray>

YoutubeService::YoutubeService(QObject *parent, QNetworkAccessManager* networkManager)
    : QObject(parent), m_networkManager(networkManager), m_apiKey(loadApiKey())
{
    connect(m_networkManager, &QNetworkAccessManager::finished,
            this, &YoutubeService::onReplyFinished);
}

//returns value (structure video) of videos
const QList<YoutubeService::Video> YoutubeService::videoResults()
{
    return m_videos.values();
}

void YoutubeService::onReplyFinished(QNetworkReply *reply)
{
    //Check for network errors and delete reply after event loop
    if (reply->error() != QNetworkReply::NoError) {
        reply->deleteLater();
        return;
    }

    RequestType type = static_cast<RequestType>(reply -> property("requestType").toInt());

    QByteArray response = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonObject obj = doc.object();
    QJsonArray items = obj["items"].toArray();
    m_videos.clear();

    //Store all videoIds for further requests
    QStringList videoIds;

    switch(type){
    case RequestType::SearchVideos:

        for (const QJsonValue &item : items){

            QJsonObject obj = item.toObject();
            QJsonObject idObj = obj["id"].toObject();
            QJsonObject snippet = obj["snippet"].toObject();

            //create video structure and load with data
            Video video;
            video.videoId = idObj["videoId"].toString();;
            video.title=snippet["title"].toString();
            video.thumbnail=snippet["thumbnails"].toObject()["medium"].toObject()["url"].toString();

            m_videos[video.videoId]=video;
            videoIds.append(video.videoId);
        }

        requestAdditionalData(videoIds);
        break;

    case RequestType::GetAdditionalData:

        for (const QJsonValue &item: items){
            QJsonObject obj = item.toObject();

            QString videoId = obj["id"].toString();
            if(!m_videos.contains(videoId)) continue;

            QJsonObject stats = obj["statistics"].toObject();
            QJsonObject content = obj["contentDetails"].toObject();

            m_videos[videoId].views = formatViews(stats["viewCount"].toString().toLongLong());
            QString durationISO = content["duration"].toString();
            m_videos[videoId].duration = parseDuration(durationISO);
        }

        emit youtubeUrlFinished();
        break;

    case RequestType::GetVideoById:
        break;
    case RequestType::SearchPlaylists:

        for (const QJsonValue &item:items){
            QJsonObject obj = item.toObject();
            QJsonObject idObj = obj["id"].toObject();
            QJsonObject snippet = obj["snippet"].toObject();

            //Create playlist structure
            Video playlist;
            playlist.videoId = idObj["playlistId"].toString();

            playlist.title=snippet["title"].toString();
            playlist.thumbnail=snippet["thumbnails"].toObject()["medium"].toObject()["url"].toString();

            //Add to List of items
            m_videos[playlist.videoId] = playlist;

            emit youtubeUrlFinished();
        }

        break;
    case RequestType::GetPlaylistItems:
        break;
    }


    reply->deleteLater();
}

void YoutubeService::searchSnippet(const QString &query, SearchFilter filter)
{
    QUrl url("https://www.googleapis.com/youtube/v3/search");

    QUrlQuery queryParams;
    queryParams.addQueryItem("part", "snippet");
    queryParams.addQueryItem("q", query);
    queryParams.addQueryItem("maxResults", "5");
    queryParams.addQueryItem("key", m_apiKey);

    //differ between different output types
    m_filter = filter;
    switch (filter) {
        case SearchFilter::Videos:
            queryParams.addQueryItem("type", "video");
            break;
        case SearchFilter::Playlists:
            queryParams.addQueryItem("type", "playlist");
            break;
    }

    url.setQuery(queryParams);

    QNetworkRequest request(url);

    QNetworkReply* reply=m_networkManager->get(request);

    //add property requestType to request to store type of return data for later use
    if (filter == SearchFilter::Videos)
        reply->setProperty("requestType", static_cast<int>(RequestType::SearchVideos));
    else
        reply->setProperty("requestType", static_cast<int>(RequestType::SearchPlaylists));
    reply -> setProperty("query", query);
}

void YoutubeService::requestAdditionalData(QStringList &videoIds)
{
    QUrl url("https://www.googleapis.com/youtube/v3/videos");

    QUrlQuery queryParams;
    queryParams.addQueryItem("part", "statistics, contentDetails");
    //pass all ids of the videos
    queryParams.addQueryItem("id", videoIds.join(","));
    queryParams.addQueryItem("key", m_apiKey);

    url.setQuery(queryParams);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);

    reply -> setProperty("requestType", static_cast<int>(RequestType::GetAdditionalData));

}


QString YoutubeService::loadApiKey()
{
    QString path = QCoreApplication::applicationDirPath() + "/config.local.json";

    if (!QFile::exists(path)) {
        path = QCoreApplication::applicationDirPath() + "/config.json";
    }

    QFile file(path);

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug()<<"Failed to open config file: "<< path;
        return "";
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject obj = doc.object();

    QString apiVal = obj["youtube_api_key"].toString();

    if (apiVal=="YOUR_API_KEY_HERE" || apiVal.isEmpty()){
        qDebug()<<"Please initialize Google-Api-Key";
        return "";
    }

    return apiVal;
}

QString YoutubeService::parseDuration(const QString &iso)
{
    QRegularExpression re("PT(?:(\\d+)H)?(?:(\\d+)M)?(?:(\\d+)S)?");
    QRegularExpressionMatch match = re.match(iso);

    int h = match.captured(1).isEmpty() ? 0 : match.captured(1).toInt();
    int m = match.captured(2).isEmpty() ? 0 : match.captured(2).toInt();
    int s = match.captured(3).isEmpty() ? 0 : match.captured(3).toInt();

    if (h > 0)
        return QString("%1:%2:%3")
            .arg(h)
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0'));
    else
        return QString("%1:%2")
            .arg(m)
            .arg(s, 2, 10, QChar('0'));
}

QString YoutubeService::formatViews(qint64 views)
{
    //remove leading 0
    auto format = [](double value, const QString& suffix) {
        QString str = QString::number(value, 'f', 1);
        if (str.endsWith(".0"))
            str.chop(2); // remove ".0"
        return str + suffix;
    };

    //format
    if(views>=1'000'000'000)
        return QString::number(views/1'000'000'000.0, 'f', 1) + "B";
    if(views>=1'000'000)
        return QString::number(views/1'000'000.0, 'f', 1) + "M";
    if(views>=1'000)
        return QString::number(views/1'000.0, 'f', 1) + "K";
    return QString::number(views);
}