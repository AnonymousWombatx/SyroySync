#include "downloadmanager.h"
#include "downloadjob.h"
#include <QDir>

DownloadManager::DownloadManager(BaseDownloader* downloader, QObject* parent)
    : QAbstractListModel(parent), m_downloader(downloader)
{}

void DownloadManager::addDownload(const QVariantMap &data, const QVariantMap &metadata)
{

    DownloadOptions options = optionsFromMap (data, metadata);

    //Create download-directory if it doesn't exist
    QDir().mkpath(options.saveLocation);

    //New downloadjob
    //auto: Compiler auto detects datatype from right side
    auto* job = new DownloadJob(m_downloader, options, this);

    connect(job, &DownloadJob::progressChanged, this, [this, job]() {
        int row = m_jobs.indexOf(job);
        if(row<0) return;
        QModelIndex idx = index(row);
        emit dataChanged(idx, idx, {ProgressRole});
    });

    connect(job, &DownloadJob::stateChanged, this, [this, job]() {
        int row = m_jobs.indexOf(job);
        if (row < 0) return;
        QModelIndex idx = index(row);
        emit dataChanged(idx, idx, {StateRole});
    });

    qDebug() << "New downloadjob created";

    //get the row count
    const int row = m_jobs.size();

    //Notify, execute, finish
    beginInsertRows(QModelIndex(), row, row);
    m_jobs.append(job);
    endInsertRows();
    emit countChanged();

    job->start();
}

DownloadOptions DownloadManager::optionsFromMap(const QVariantMap &data, const QVariantMap &metadata)
{
    DownloadOptions options;

    options.name = data.value("name").toString();
    options.audioOnly = data.value("audioOnly").toBool();
    options.extension = data.value("extension").toString();

    options.outputName = data.value("outputName").toString();

    options.thumbnail = data.value("thumbnail").toString();
    options.addThumbnail = data.value("addThumbnail").toBool();
    options.replaceVideoWithThumbnail =
    data.value("replaceVideoWithThumbnail").toBool();

    options.saveLocation = QUrl(data.value("saveLocation").toString()).toLocalFile();

    options.enableTrim = data.value("enableTrim").toBool();
    options.trimStart = data.value("trimStart").toString();
    options.trimEnd = data.value("trimEnd").toString();

    options.url = data.value("url").toString();

    options.metadata.title = metadata.value("title").toString();
    options.metadata.album = metadata.value("album").toString();
    options.metadata.artist = metadata.value("artist").toString();
    options.metadata.genre = metadata.value("genre").toString();
    options.metadata.releaseDate = metadata.value("releaseDate").toString();

    return options;
}

int DownloadManager::rowCount(const QModelIndex& parent) const {
    if (parent.isValid()) return 0;
    return m_jobs.count();
}

QVariant DownloadManager::data(const QModelIndex &index, int role) const
{
    //Check whether the index is valid and within bounds, else returning nothing
    if(!index.isValid() || index.row()>=m_jobs.count())
        return {};

    const DownloadJob *d = m_jobs.at(index.row());
    if (!d) return {};

    //connects Role to specific data from YoutubeService
    switch (role) {
    case ProgressRole:  return d->progress();
    case NameRole:      return d->name();
    case ThumbnailRole: return d->thumbnail();
    case StateRole:     return d->state();
    default:            return {};
    }
}

//enables call with string property names, connected to integer roles
QHash<int, QByteArray> DownloadManager::roleNames() const
{
    return {
            {   ProgressRole,   "progress"    },
            {   NameRole,     "name"      },
            {   ThumbnailRole,   "thumbnail"    },
            {   StateRole, "dwlState"  }
            };
}

void DownloadManager::stopDownload(int row){
    if(row<0||row>=m_jobs.size()) return;
    m_jobs[row]->stop();
}
void DownloadManager::pauseDownload(int row){
    if(row<0||row>=m_jobs.size()) return;
    m_jobs[row]->pause();
}
void DownloadManager::resumeDownload(int row){
    if(row<0||row>=m_jobs.size()) return;
    m_jobs[row]->resume();
}