#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QNetworkRequest>

#include "updater.h"
#include "youtubeservice.h"
#include "videomodel.h"

int main(int argc, char *argv[])
{

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
    YoutubeService service (nullptr, &networkManager);
    VideoModel model;

    //enable QML properties from classes
    engine.rootContext()->setContextProperty("updater", &updater);
    engine.rootContext()->setContextProperty("videoModel", &model);

    engine.loadFromModule("SyroySync", "Main");

    return app.exec();
}
