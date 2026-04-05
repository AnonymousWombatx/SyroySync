#ifndef VIDEOMODEL_H
#define VIDEOMODEL_H

#include <QAbstractListModel>
#include "youtubeservice.h"

class VideoModel : public QAbstractListModel
{
    Q_OBJECT

public:

    //Links roles to unique integer IDs
    enum Roles {
        VideoIdRole = Qt::UserRole+1,
        TitleRole,
        ChannelRole,
        ThumbnailRole,
        DurationRole,
        ViewsRole
    };

    Q_ENUM(Roles)

    explicit VideoModel(QObject *parent = nullptr);

    //returns the itemcount in the model
    int rowCount(const QModelIndex &parent={}) const override;
    //function to call data from a row with a specific role
    //model.title in qml calls data(index, TitleRole) in C++
    //index comes from context when calling model in QML
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const override;
    //Maps role numbers to role names
    //TitleRole:"title" enables to call model.title
    QHash<int, QByteArray> roleNames() const override;

public slots:
    //updates model with new data when YoutubeService emits signal
    void setVideos(const QList<YoutubeService::Video> &videos);

private:
    //variable to store data from youtubeservice class
    QList<YoutubeService::Video> m_videos;
};

#endif // VIDEOMODEL_H
