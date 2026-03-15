#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QNetworkRequest>

#include "updater.h"

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

    QNetworkAccessManager networkManager;
    Updater updater(nullptr, &networkManager);

    engine.rootContext()->setContextProperty("updater", &updater);

    engine.loadFromModule("SyroySync", "Main");

    return app.exec();
}
