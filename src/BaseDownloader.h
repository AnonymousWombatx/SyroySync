#ifndef BASEDOWNLOADER_H
#define BASEDOWNLOADER_H

#include <QObject>
#include <QtPlugin>

class BaseDownloader {
public:
    virtual ~BaseDownloader() = default;

    virtual void getDataFromLink(QString url)=0;
};

Q_DECLARE_INTERFACE (BaseDownloader, "com.SyroySync.BaseDownloader/2.0");

#endif // BASEDOWNLOADER_H

