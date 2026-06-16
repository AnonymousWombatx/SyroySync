#ifndef YTDLPPLUGIN_H
#define YTDLPPLUGIN_H

#include <QString>

#include "basedownloader.h"
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

    Q_INVOKABLE void getDataFromLink(QString url) override;

    DownloadState::State state() const { return m_state; }

    QString executablePath() const override;
    QStringList downloadArguments(const DownloadOptions& options) const override;

signals:
    void dataReady ();
    void errorOccured(const QString &message);

private slots:
    void onFinished (int exitCode, QProcess::ExitStatus exitStatus);
    void onErrorOccurred(QProcess::ProcessError error);

private:

    //Process for searching url
    QProcess *m_process = nullptr;

    QString getPath();
    QString getFfmpeg();
    QString m_path;
    QString m_ffmpeg;

    DownloadState::State m_state = DownloadState::Idle;

    void setState(DownloadState::State state);

    VideoModel* m_videoModel = nullptr;

    QString formatDuration(double seconds);
    QString formatViews(qint64 views);

    QString tempUrl;
};



#endif // YTDLPPLUGIN_H
