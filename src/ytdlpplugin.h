#ifndef YTDLPPLUGIN_H
#define YTDLPPLUGIN_H

#include <QString>

#include "BaseDownloader.h"
#include "videomodel.h"

#include <QProcess>

class DownloadState : public QObject {
    Q_OBJECT
public:
    enum State {
        Idle,
        RetrievingMetadata,
        DownloadingSingle,
        DownloadingPlaylist,
        Finished,
        Error
    };
    Q_ENUM(State)
};

class YtdlpPlugin : public QObject, public BaseDownloader
{

    Q_OBJECT
    Q_INTERFACES(BaseDownloader)

public:
    YtdlpPlugin(VideoModel* videoModel = nullptr);

    Q_INVOKABLE void getDataFromLink(QString url);

    DownloadState::State state() const { return m_state; }

signals:
    void dataReady ();
    void errorOccured(const QString &message);

private slots:
    void onFinished (int exitCode, QProcess::ExitStatus exitStatus);
    void onErrorOccurred(QProcess::ProcessError error);

private:

    QProcess *m_process;

    QString getPath();
    QString m_path;

    DownloadState::State m_state = DownloadState::Idle;

    void setState(DownloadState::State state);

    VideoModel* m_videoModel;

    QString formatDuration(double seconds);
    QString formatViews(qint64 views);
};



#endif // YTDLPPLUGIN_H
