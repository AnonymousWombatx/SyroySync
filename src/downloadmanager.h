#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QAbstractListModel>

#include "downloadjob.h"

class DownloadManager : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)

public:

    enum Roles {
        ProgressRole = Qt::UserRole + 1,
        NameRole,
        ThumbnailRole,
        StateRole
    };

    Q_ENUM (Roles)

    explicit DownloadManager(BaseDownloader* downloader, QObject* parent = nullptr);

    Q_INVOKABLE void addDownload (const QVariantMap& data, const QVariantMap& metadata);
    Q_INVOKABLE void addPlaylist (const QVariantMap& data);

    //implemented functions for QAbstractListModel
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(
        const QModelIndex& index,
        int role = Qt::DisplayRole
        ) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void stopDownload(int row);
    Q_INVOKABLE void pauseDownload(int row);
    Q_INVOKABLE void resumeDownload(int row);

    Q_INVOKABLE void clearFinished();

signals:
    void countChanged();

private:

    QList<DownloadJob*> m_jobs;
    BaseDownloader* m_downloader = nullptr;

    //gets the options from the qml map to the settings struct
    DownloadOptions optionsFromMap(const QVariantMap &data, const QVariantMap &metadata);
    DownloadOptions optionsFromMap(const QVariantMap &data);

};

#endif // DOWNLOADMANAGER_H
