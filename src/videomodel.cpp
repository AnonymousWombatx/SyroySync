#include "videomodel.h"
#include "structures.h"

VideoModel::VideoModel(QObject *parent)
    :QAbstractListModel(parent){}

int VideoModel::rowCount(const QModelIndex &parent) const
{
    //call for a children of a row, which doesn't apply for 1D-List
    if(parent.isValid()) return 0;
    return m_videos.count();
}

QVariant VideoModel::data(const QModelIndex &index, int role) const
{
    //Check whether the index is valid and within bounds, else returning nothing
    if(!index.isValid() || index.row()>=m_videos.count())
        return {};

    const Video &v = m_videos.at(index.row());

    //connects Role to specific data from YoutubeService
    switch (role) {
    case VideoIdRole:   return v.videoId;
    case TitleRole:     return v.title;
    case ChannelRole:   return v.channel;
    case ThumbnailRole: return v.thumbnail;
    case DurationRole:  return v.duration;
    case ViewsRole:     return v.views;
    case UrlRole:       return v.url;
    case PlaylistRole:  return v.playlist;
    default:            return {};
    }
}

//enables call with string property names, connected to integer roles
QHash<int, QByteArray> VideoModel::roleNames() const
{
    return {
            {   VideoIdRole,   "videoID"    },
            {   TitleRole,     "title"      },
            {   ChannelRole,   "channel"    },
            {   ThumbnailRole, "thumbnail"  },
            {   DurationRole,  "duration"   },
            {   ViewsRole,     "views"      },
            {   UrlRole,        "url"       },
            {   PlaylistRole,   "playlist"  },
            };
}

QVariantMap VideoModel::getByUrl(const QString &url) const
{
    for (const auto &v : m_videos) {
        if (v.url==url) {
            qDebug()<<"Successfully found Id in m_videos";
            return {
                {"videoId",     v.videoId},
                {"title",       v.title},
                {"channel",     v.channel},
                {"thumbnail",   v.thumbnail},
                {"duration",    v.duration},
                {"views",       v.views},
                {"url",         v.url},
                {"playlist",    v.playlist}
            };
        }
    }
    qDebug()<<"Error getById: Id not found in m_videos";
    return {};
}

//new data enters model
void VideoModel::setVideos(const QList<Video> &videos)
{
    //stop reading from model and discard cached state
    beginResetModel();
    //replace data with new lsit
    m_videos = videos;
    //trigger re-rerender with new data
    endResetModel();
}





