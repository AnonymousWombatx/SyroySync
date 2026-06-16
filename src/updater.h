#define APP_VERSION "2.0.0"

#ifndef UPDATER_H
#define UPDATER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Updater : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString latestVersion READ latestVersion NOTIFY latestVersionChanged FINAL)
    Q_PROPERTY(bool updateAvailable READ updateAvailable NOTIFY updateAvailableChanged FINAL)

public:
    explicit Updater(QObject *parent = nullptr, QNetworkAccessManager* m_networkManager = nullptr);

    Q_INVOKABLE void checkForUpdates();
    Q_INVOKABLE void updateYtDlp(const QString& ytDlpPath);

    QString latestVersion() const;
    bool updateAvailable() const;

signals:
    void latestVersionChanged();
    void updateAvailableChanged();
    void updateCheckFinished();

    void ytDlpUpdateFinished();
    void ytDlpUpdateFailed(const QString& error );

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager* m_networkManager;
    QString m_latestVersion;
    bool m_updateAvailable = false;

};

#endif // UPDATER_H
