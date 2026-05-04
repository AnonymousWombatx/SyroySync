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
#include <QNetworkProxy>
#include "videomodel.h"

YoutubeService::YoutubeService(QObject *parent, QNetworkAccessManager* networkManager, VideoModel* videoModel)
    : QObject(parent), m_networkManager(networkManager), m_apiKey(loadApiKey()), m_videoModel(videoModel)
{
    if (m_videoModel)
        m_videoModel->setVideos(m_videos.values());
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

        qDebug() << "Network error:" << reply->errorString();

        reply->deleteLater();
        return;
    }

    qDebug() << "HTTP status:"
             << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

    qDebug() << "Network error:"
             << reply->error()
             << reply->errorString();

    RequestType type = static_cast<RequestType>(reply -> property("requestType").toInt());

    QByteArray response = reply->readAll();

    // Print ALL response headers
    const auto headers = reply->rawHeaderPairs();
    for (const auto& h : headers)
        qDebug() << "Header:" << h.first << "=" << h.second;

    qDebug() << "RAW JSON RESPONSE:" << response;

    QJsonDocument doc = QJsonDocument::fromJson(response);
    QJsonObject obj = doc.object();

    if (obj.contains("error")) {
        qDebug() << "YouTube API ERROR:" << obj["error"].toObject();
        return;
    }

    QJsonArray items = obj["items"].toArray();

    //Store all videoIds for further requests
    QStringList videoIds;

    switch(type){
    case RequestType::SearchVideos:

        m_videos.clear();

        for (const QJsonValue &item : items){


            QJsonObject obj = item.toObject();
            QJsonObject idObj = obj["id"].toObject();
            QJsonObject snippet = obj["snippet"].toObject();

            //create video structure and load with data
            Video video;
            video.videoId = idObj["videoId"].toString();;
            video.title=snippet["title"].toString();
            video.channel=snippet["channelTitle"].toString();
            video.thumbnail=snippet["thumbnails"].toObject()["medium"].toObject()["url"].toString();

            m_videos[video.videoId]=video;
            videoIds.append(video.videoId);
        }

        qDebug() << "VideoIds count:" << videoIds.size();

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

        m_videoModel->setVideos(m_videos.values());
        qDebug()<<"Additional Data retrieved and videos set in videoModel";
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

            qDebug()<<"youtubeUrlFinished Emitted";
        }

        break;
    case RequestType::GetPlaylistItems:
        break;
    }


    reply->deleteLater();
}

void YoutubeService::searchSnippet(const QString &query, SearchFilter filter)
{

    qDebug()<<"Search Snippet with snippet "<<query<<" with filter "<<filter;

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
    qDebug()<<url;
    QNetworkRequest request(url);

    QNetworkReply* reply=m_networkManager->get(request);

    //add property requestType to request to store type of return data for later use
    if (filter == SearchFilter::Videos)
        reply->setProperty("requestType", static_cast<int>(RequestType::SearchVideos));
    else
        reply->setProperty("requestType", static_cast<int>(RequestType::SearchPlaylists));
    reply -> setProperty("query", query);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
       onReplyFinished(reply);
    });
}

QString YoutubeService::selectedId()
{
    return m_selectedId;
}

void YoutubeService::setSelectedId(QString inputId)
{
    m_selectedId=inputId;
}

void YoutubeService::requestAdditionalData(QStringList &videoIds)
{
    QUrl url("https://www.googleapis.com/youtube/v3/videos");

    QUrlQuery queryParams;
    queryParams.addQueryItem("part", "statistics,contentDetails");
    //pass all ids of the videos
    queryParams.addQueryItem("id", videoIds.join(","));
    queryParams.addQueryItem("key", m_apiKey);

    url.setQuery(queryParams);

    QNetworkRequest request(url);
    QNetworkReply* reply = m_networkManager->get(request);

    reply -> setProperty("requestType", static_cast<int>(RequestType::GetAdditionalData));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {onReplyFinished(reply);});
}


QString YoutubeService::loadApiKey()
{
    QString path = QCoreApplication::applicationDirPath() + "/config.local.json";

    if (!QFile::exists(path)) {
        path = QCoreApplication::applicationDirPath() + "/config.json";
    }

    qDebug() << "Loading config from:" << path;

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

    qDebug()<<"API-Key: "<<apiVal;
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