#ifndef DOWNLOADSERVICE_H
#define DOWNLOADSERVICE_H

#include <QObject>

#include "BaseDownloader.h"

class downloadservice : public QObject
{
    Q_OBJECT
public:
    explicit downloadservice(BaseDownloader *baseDl = nullptr);

    bool getDataFromLink(QString url);

private:

    BaseDownloader *m_bdl;

    bool isYoutube(QString url);
};

#endif // DOWNLOADSERVICE_H
