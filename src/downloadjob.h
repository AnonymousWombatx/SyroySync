#ifndef DOWNLOADJOB_H
#define DOWNLOADJOB_H

#include <QObject>
#include <QProcess>

#include "structures.h"
#include "basedownloader.h"

class DownloadJob : public QObject
{
    Q_OBJECT
    Q_PROPERTY (int progress READ progress NOTIFY progressChanged)
public:
    explicit DownloadJob(BaseDownloader* downloader, DownloadOptions options, QObject* parent);

    enum States {
        downloading,
        converting,
        thumbnailing,
        error,
        paused,
        finished,
        stopped,
        starting //Before downloading, set in constructor
    };

    void start();

    void stop();
    void pause();
    void resume();

    int progress() const;
    QString name() const;
    QString thumbnail() const;
    QString state() const;

signals:
    void progressChanged();
    void stateChanged();

private slots:
    void onReadyReadStandardOutput();

private:

    States m_state;
    States previousState;
    States getState();
    QString stateString(States state) const;

    QProcess* m_process;

    DownloadOptions m_options;

    BaseDownloader* m_downloader;

    int m_progress;

    void setProgress(int progress);
    const int parseProgressLine(const QString& line);

    void setState(States state);
};

#endif // DOWNLOADJOB_H
