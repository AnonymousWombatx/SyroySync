#ifndef YOUTUBESERVICE_H
#define YOUTUBESERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QObject>
#include <QMap>

class YoutubeService : public QObject
{
    Q_OBJECT
public:
    explicit YoutubeService(QObject *parent = nullptr, QNetworkAccessManager* networkManager = nullptr);

    enum class RequestType {
        SearchVideos,
        GetAdditionalData,
        GetVideoById,
        SearchPlaylists,
        GetPlaylistItems
    };

    struct Video {
        QString videoId;
        QString title;
        QString channel;
        QString thumbnail;
        QString duration;
        QString views;
    };

    const QList<Video> videoResults();

signals:
    void youtubeUrlFinished();

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager* m_networkManager;

    void searchSnippet(const QString& query);
    void requestAdditionalData(QStringList &videoIds);

    const QString m_apiKey;
    QString getApiKey();
    QString loadApiKey();

    //map videoId to video structure containing the data
    QMap<QString, Video> m_videos;

    QString parseDuration(const QString &iso);
    QString formatViews(qint64 views);

};



#endif // YOUTUBESERVICE_H
