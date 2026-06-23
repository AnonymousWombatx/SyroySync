#ifndef YOUTUBESERVICE_H
#define YOUTUBESERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QObject>
#include <QMap>

#include "videomodel.h"
#include "structures.h"

class YoutubeService : public QObject
{
    Q_OBJECT

public:
    explicit YoutubeService(QObject *parent = nullptr, QNetworkAccessManager* networkManager = nullptr, VideoModel* videoModel = nullptr);

    enum class RequestType {
        SearchVideos,
        GetAdditionalData,
        GetVideoById,
        SearchPlaylists,
        GetPlaylistItems
    };

    enum class SearchFilter {
        Videos,
        Playlists
    };

    Q_INVOKABLE void searchSnippet(const QString &query, bool select);
    Q_ENUM(SearchFilter)

    const QList<Video> videoResults();

signals:
    void youtubeUrlFinished();

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager* m_networkManager;

    VideoModel* m_videoModel;

    void requestAdditionalData(QStringList &videoIds);

    const QString m_apiKey;
    QString getApiKey();
    QString loadApiKey();

    //map videoId to video structure containing the data
    QMap<QString, Video> m_videos;

    QString parseDuration(const QString &iso);
    QString formatViews(qint64 views);

    SearchFilter m_filter;
};



#endif // YOUTUBESERVICE_H
