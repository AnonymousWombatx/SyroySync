#include "downloadjob.h"
#include <QCoreApplication>
#include <QDir>
#include <QDateTime>
#include <windows.h>

DownloadJob::DownloadJob(BaseDownloader* downloader, DownloadOptions options, QObject* parent)
    : m_process(new QProcess(this)), m_downloader (downloader), m_options (options), QObject(parent), m_progress(0),
    m_state(States::starting), previousState(States::starting)
{
    //emits corresponding signal when the process writes new data to the standard output
    connect(m_process, &QProcess::readyReadStandardOutput, this, &DownloadJob::onReadyReadStandardOutput);
    connect(m_process, &QProcess::finished, this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit && exitCode == 0)
            setState(finished);
        else
                setState(error);
    });
}

void DownloadJob::start()
{
    QString program = m_downloader->executablePath();
    QStringList args = m_downloader->downloadArguments(m_options);
    qDebug() << program << "\n" << args;

    //Log file
    //Create Directory if not available
    //QString logDir = QCoreApplication::applicationDirPath() + "/logs";
    //QDir().mkpath(logDir);
    //Create name from date and videotitle
    //QString date = QDateTime::currentDateTime().toString("yyy-MM-dd_HH-mm-ss");
    //QString safeName = m_options.outputName;
    //safeName.replace(QRegularExpression(R"([\\/:*?"<>|])"), "_");
    //QString logPath = logDir + "/" + date + "_" + safeName + ".log";

    //m_process->setStandardErrorFile(logPath);

    m_process->start(program, args);
    qDebug() << "Downloadjob started";
}

int DownloadJob::progress() const
{
    qDebug()<<"Progress: "<<m_progress;
    return m_progress;
}

QString DownloadJob::outputName() const
{
    return m_options.outputName;
}

QString DownloadJob::thumbnail() const
{
    return m_options.thumbnail;
}

QString DownloadJob::state() const
{
    qDebug()<<"State: "<<stateString(m_state);
    return stateString(m_state);
}

void DownloadJob::onReadyReadStandardOutput()
{
    //don' read when paused or stopped to prevent delayed continue
    if (m_state == stopped || m_state == paused) return;

    //Reads from the standard output of the process
    const QString output = QString::fromUtf8(m_process->readAllStandardOutput());

    const QStringList lines = output.split('\n', Qt::SkipEmptyParts);

    //Checks each line of new console output for valid progress
    for (const QString& line : lines) {
        if (line.startsWith("[Merger]"))
            setState(converting);
        else if (line.startsWith("[EmbedThumbnail]"))
            setState(thumbnailing);
        else if (line.startsWith("[download]")) {
            setState(downloading);
            const int progress = parseProgressLine(line);
            if (progress>=0) {
                setProgress(progress);
            }
            else if (line.startsWith("ERROR"))
                setState(error);
        }
    }
}

void DownloadJob::setProgress(int progress)
{
    //no new progress, curent = new
    if (m_progress == progress) return;

    m_progress = progress;

    qDebug()<<"Progress: "<<m_progress;

    emit progressChanged();

}

const int DownloadJob::parseProgressLine(const QString &line)
{
    static QRegularExpression re(R"((\d+\.?\d*)%)");
    auto match = re.match(line);
    if(!match.hasMatch()) return -1;

    //Variable ok indicates if conversion succeeded
    bool ok =false;
    double value = match.captured(1).toDouble(&ok);

    return ok ? qRound(value) : -1;
}

void DownloadJob::setState(States state)
{
    m_state = state;
    emit stateChanged();
}



void DownloadJob::stop() {
    m_process->kill();
    setState(stopped);
}

void DownloadJob::pause(){
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, m_process->processId());
    DebugActiveProcess(m_process->processId());  // suspends all threads
    CloseHandle(hProcess);
    setState(paused);
}

void DownloadJob::resume(){
    DebugActiveProcessStop(m_process->processId());
    setState(previousState);
}

DownloadJob::States DownloadJob::getState(){
    return m_state;
}

QString DownloadJob::stateString(States state) const
{
    switch(state){
    case downloading:   return "Downloading";
    case converting:    return "Converting";
    case thumbnailing:  return "Adding thumbnail";
    case error:         return "Error";
    case paused:        return "Paused";
    case finished:      return "Finished";
    case stopped:       return "Stopped";
    case starting:      return "Starting";
    default:            return "Unknown";
    }
}


