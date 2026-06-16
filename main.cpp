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

int main(int argc, char *argv[])
{

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
