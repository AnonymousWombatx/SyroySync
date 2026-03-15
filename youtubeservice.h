#ifndef YOUTUBESERVICE_H
#define YOUTUBESERVICE_H

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QObject>

class YoutubeService : public QObject
{
    Q_OBJECT
public:
    explicit YoutubeService(QObject *parent = nullptr, QNetworkAccessManager* m_networkManager = nullptr);

signals:
    void youtubeUrlFinished();

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager* m_networkManager;

};



#endif // YOUTUBESERVICE_H
