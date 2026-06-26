#include "ytdlpplugin.h"

#include <QCoreApplication>
#include <QFile>
#include <QProcess>
#include <QDir>

#include <QJsonArray>
#include <QTextDocumentFragment>

YtdlpPlugin::YtdlpPlugin(VideoModel* videoModel)
    : m_path(QDir(QCoreApplication::applicationDirPath()).filePath("tools/yt-dlp.exe")),
    m_videoModel(videoModel),
    m_ffmpeg(QDir(QCoreApplication::applicationDirPath()).filePath("tools/"))
{
    m_process = new QProcess(this);

    connect(m_process, &QProcess::finished, this, &YtdlpPlugin::onFinished);
    connect (m_process, &QProcess::errorOccurred, this, &YtdlpPlugin::onErrorOccurred);
}

void YtdlpPlugin::getDataFromLink(QString url)
{
    tempUrl = url;
    bool isPlaylist = url.contains("list=") || url.contains("playlist");

    qDebug()<<"Retrieving Data from Link with Yt-Dlp for: "<<url;
    if (m_process->state() != QProcess::NotRunning) {
        emit errorOccured("Process already running");
        return;
    }

    setState(DownloadState::RetrievingMetadata);

    QStringList args;
    if(isPlaylist) {
        args<<"--dump-single-json";
        args<<"--flat-playlist";
    }
    else {
        args << "--dump-json";
    }

    args<<url;

    m_process->start(m_path, args);
}

QString YtdlpPlugin::executablePath() const
{
    return m_path;
}

QStringList YtdlpPlugin::downloadArguments(const DownloadOptions &options) const
{
    QStringList args;

    //Progress output: each output in new line and correct format
    args << "--newline";

    const QString outputTemplate = QDir(options.saveLocation).filePath(options.outputName + ".%(ext)s");

    args << "-o" << outputTemplate;

    //Output file
    if (options.audioOnly) {
        args <<"-x";
        args << "--audio-format" << options.extension;
        args << "--audio-quality" << "0";
    }
    else {
        args << "-f" << "bv+ba/b";
        args << "--merge-output-format" << options.extension;
    }

    //thumbnail
    if (options.addThumbnail) {
        args << "--write-thumbnail";
        args << "--embed-thumbnail";
    }

    //trim video
    if (options.enableTrim) {
        args << "--download-sections";
        args << QString ("*%1-%2").arg(options.trimStart).arg(options.trimEnd);
    }

    //lambda to add Metadata
    auto addMeta = [&](const QString& key, const QString& value) {
        if (value.isEmpty()) return;
        args << "--postprocessor-args" << QString("ffmpeg:-metadata %1=\"%2\"").arg(key, value);
    };

    addMeta("title", options.metadata.title);
    addMeta("artist", options.metadata.artist);
    addMeta("album", options.metadata.album);
    addMeta("genre", options.metadata.genre);
    addMeta("date", options.metadata.releaseDate);

    args << "--ffmpeg-location"<<m_ffmpeg;

    args << options.url;

    return args;
}


void YtdlpPlugin::onFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    //alternatively CrashExit
    if (exitStatus!=QProcess::NormalExit || exitCode!=0) {

        //returns all data available from the standard error of the process
        QString err = QString::fromUtf8(m_process->readAllStandardError());
        setState(DownloadState::Error);
        emit errorOccured(err.trimmed());
        qDebug()<<err.trimmed();
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
        qWarning()<<err;
        emit errorOccured(err.trimmed());
        return;
    }

    QJsonObject obj = doc.object();

    qDebug().noquote() << "Process finished";

    Video v;
    v.videoId   = obj["id"].toString();
    v.url       = tempUrl;
    v.title     = QTextDocumentFragment::fromHtml(obj["title"].toString()).toPlainText();
    v.channel   = QTextDocumentFragment::fromHtml(obj["uploader"].toString()).toPlainText();
    v.thumbnail = obj["thumbnail"].toString();
    v.views     = formatViews(obj["view_count"].toInt());
    v.playlist  = obj.contains("entries");

    if (v.playlist)
        v.duration = QString("%1 videos").arg(obj["entries"].toArray().size());
    else
        v.duration = formatDuration(obj["duration"].toDouble());

    m_videoModel->setVideos({v});

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
