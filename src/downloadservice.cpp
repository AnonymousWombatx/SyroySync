#include "downloadservice.h"

downloadservice::downloadservice(BaseDownloader *baseDl) : m_bdl(baseDl) {}

bool downloadservice::getDataFromLink(QString url)
{
    if (isYoutube(url)){
        //return service.getDataFromLink(url);
        return true;
    }
}

bool downloadservice::isYoutube(QString url)
{
    return url.contains("youtube", Qt::CaseInsensitive);
}
