#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QNetworkRequest>

#include "src/updater.h"
#include "src/youtubeservice.h"
#include "src/videomodel.h"
#include "src/ytdlpplugin.h"
#include "src/downloadmanager.h"

#include <QQuickStyle>

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <QDirIterator>

#include <QStandardPaths>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

static QFile* g_logFile = nullptr;

void pruneOldLogs(const QString& logDir, int maxAgeDays)
{
    const QDateTime cutoff = QDateTime::currentDateTime().addDays(-maxAgeDays);
    const QFileInfoList entries = QDir(logDir).entryInfoList(
        {"*.log"}, QDir::Files
        );

    for (const QFileInfo& info : entries) {
        if (info.lastModified() < cutoff) {
            QFile::remove(info.absoluteFilePath());
            // optional: qInfo() << "Pruned old log:" << info.fileName();
        }
    }
}

void messageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    if (!g_logFile || !g_logFile->isOpen())
        return;

    QTextStream out(g_logFile);

    const QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString prefix;
    switch (type) {
    case QtDebugMsg:    prefix = "DEBUG";    break;
    case QtInfoMsg:     prefix = "INFO";     break;
    case QtWarningMsg:  prefix = "WARNING";  break;
    case QtCriticalMsg: prefix = "ERROR";    break;
    case QtFatalMsg:    prefix = "FATAL";    break;
    }

    // Optionally include file/line in debug builds
#ifdef QT_DEBUG
    const QString location = context.file
                                 ? QStringLiteral(" [%1:%2]").arg(context.file).arg(context.line)
                                 : QString();
#else
    const QString location;
#endif

    out << timestamp << " [" << prefix << "]" << location << " " << msg << "\n";
    out.flush();

    // Also mirror to stderr so Qt Creator's output pane still works
    fprintf(stderr, "%s [%s]%s %s\n",
            timestamp.toLocal8Bit().constData(),
            prefix.toLocal8Bit().constData(),
            location.toLocal8Bit().constData(),
            msg.toLocal8Bit().constData());

    if (type == QtFatalMsg)
        abort();
}

int main(int argc, char *argv[])
{
    //logs
    const QString logDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/logs";
    QDir().mkpath(logDir);
    pruneOldLogs(logDir, 7);

    g_logFile = new QFile(logDir + "/app_" + QDateTime::currentDateTime().toString("yyyy-MM-dd") + ".log");
    g_logFile->open(QIODevice::Append | QIODevice::Text);
    qInstallMessageHandler(messageHandler);

    QQuickStyle::setStyle("FluentWinUI3");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    //Shared network manager
    QNetworkAccessManager networkManager;

    //cpp objects
    Updater updater(nullptr, &networkManager);
    VideoModel model;
    YoutubeService service (nullptr, &networkManager, &model);
    YtdlpPlugin plugin (&model);
    DownloadManager downloadManager(&plugin);

    //enable QML properties from classes
    engine.rootContext()->setContextProperty("updater", &updater);
    engine.rootContext()->setContextProperty("videoModel", &model);
    engine.rootContext()->setContextProperty("service", &service);
    engine.rootContext()->setContextProperty("plugin", &plugin);
    engine.rootContext()->setContextProperty("manager", &downloadManager);

    //expose formats to qml
    QFile file(":/qt/qml/SyroySync/qml/resources/data/extensions.json");
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open formats file:" << file.errorString();
    }

    file.open(QIODevice::ReadOnly);
    QJsonArray arr = QJsonDocument::fromJson(file.readAll()).array();

    QVariantList formats;
    for (const QJsonValue &v : arr)
        formats.append(v.toVariant());

    engine.rootContext()->setContextProperty("availableFormats", formats);

    qDebug() << "manager:" << &downloadManager;
    qDebug() << "manager rowCount:" << downloadManager.rowCount(QModelIndex());

    engine.loadFromModule("SyroySync", "Main");

    return app.exec();
}
