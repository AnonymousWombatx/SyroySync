#ifndef DOWNLOADSERVICE_H
#define DOWNLOADSERVICE_H

#include <QObject>

class downloadservice : public QObject
{
    Q_OBJECT
public:
    downloadservice();

    bool getDataFromLink(QString url);

private:

    bool isYoutube(QString url);
};

#endif // DOWNLOADSERVICE_H
