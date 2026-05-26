#include "ytdlpplugin.h"

#include <QCoreApplication>
#include <QFile>
#include <QProcess>

YtdlpPlugin::YtdlpPlugin(VideoModel* videoModel)
    : m_path(getPath()), m_videoModel(videoModel)
{
    m_process = new QProcess(this);

    connect(m_process, &QProcess::finished, this, &YtdlpPlugin::onFinished);
    connect (m_process, &QProcess::errorOccurred, this, &YtdlpPlugin::onErrorOccurred);

}

void YtdlpPlugin::getDataFromLink(QString url)
{
    qDebug()<<"Retrieving Data from Link with Yt-Dlp for: "<<url;
    if (m_process->state() != QProcess::NotRunning) {
        emit errorOccured("Process already running");
        return;
    }

    setState(DownloadState::RetrievingMetadata);

    QStringList args = {
        "--dump-json",
        url
    };

    m_process->start(m_path, args);
    qDebug()<<"Yt-Dlp Process started";
}

void YtdlpPlugin::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    //alternatively CrashExit
    if (exitStatus!=QProcess::NormalExit || exitCode!=0) {

        //returns all data available from the standard error of the process
        QString err = QString::fromUtf8(m_process->readAllStandardError());
        setState(DownloadState::Error);
        emit errorOccured(err.trimmed());
        return;

    }

    //returns all data available from the standard output of the process
    QByteArray output = m_process -> readAllStandardOutput();
    QJsonParseError parseError;
    //parses valid document if parsing succeeds, otherwise error details parseError
    QJsonDocument doc = QJsonDocument::fromJson(output, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        //Returns the human-readable message appropriate to the reported JSON parsing error
        QString err =  "Failed to parse metadata: " + parseError.errorString();
        setState(DownloadState::Error);
        emit errorOccured(err.trimmed());
        return;
    }

    QJsonObject obj = doc.object();

    qDebug().noquote() << "Process finished\n\n" << QJsonDocument(obj).toJson(QJsonDocument::Indented);

    VideoModel::Video video;
    video.videoId = obj["id"].toString();
    video.title = obj["title"].toString();
    video.channel = obj["uploader"].toString();
    video.thumbnail = obj["thumbnail"].toString();
    video.duration = formatDuration(obj["duration"].toDouble());
    video.views = formatViews(obj["view_count"].toInt());

    m_videoModel->setVideos({video});

    setState(DownloadState::Idle);
    emit dataReady();
}

void YtdlpPlugin::onErrorOccurred(QProcess::ProcessError error)
{
    if (error==QProcess::FailedToStart) {
        setState(DownloadState::Error);
        emit errorOccured("process failed to start, please check downloader path");
    }
}

QString YtdlpPlugin::getPath()
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


    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
    if (doc.isNull()) {
        qWarning() << "JSON parse error:" << err.errorString();
        return "";
    }

    QJsonObject obj = doc.object();

    QString pathVal = obj["dl_path"].toString();

    if (pathVal=="DOWNLOADER_PATH" || pathVal.isEmpty()){
        qDebug()<<"Please initialize Path of the Downloader";
        return "";
    }

    return pathVal;
}

void YtdlpPlugin::setState(DownloadState::State state)
{
    m_state=state;
}

QString YtdlpPlugin::formatDuration(double seconds)
{
    int total = static_cast<int>(seconds);
    int h = total / 3600;
    int m = (total % 3600) / 60;
    int s = total % 60;

    if (h > 0)
        return QString("%1:%2:%3").arg(h)
            .arg(m, 2, 10, QChar('0'))
            .arg(s, 2, 10, QChar('0'));
    return QString("%1:%2").arg(m).arg(s, 2, 10, QChar('0'));
}

QString YtdlpPlugin::formatViews(qint64 views)
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
