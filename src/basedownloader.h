#ifndef BASEDOWNLOADER_H
#define BASEDOWNLOADER_H

#include <QObject>
#include <QtPlugin>

#include "structures.h"

class BaseDownloader {
public:
    virtual ~BaseDownloader() = default;

    virtual void getDataFromLink(QString url)=0;

    virtual QString executablePath() const = 0;
    virtual QStringList downloadArguments(const DownloadOptions& options) const = 0;
};

Q_DECLARE_INTERFACE (BaseDownloader, "com.SyroySync.BaseDownloader/2.0");

#endif // BASEDOWNLOADER_H

