#ifndef BASEDOWNLOADER_H
#define BASEDOWNLOADER_H

#include <QObject>
#include <QtPlugin>

class BaseDownloader {
public:
    virtual ~BaseDownloader();

};

#endif // BASEDOWNLOADER_H

Q_DECLARE_INTERFACE (BaseDownloader, "com.SyroySync.BaseDownloader/2.0");