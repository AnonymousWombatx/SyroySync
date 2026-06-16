#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <QString>


//Structure for the general data of a video
struct Video {
    QString videoId;
    QString url;
    QString title;
    QString channel;
    QString thumbnail;
    QString duration;
    QString views;
};

struct Metadata {
    QString title;
    QString artist;
    QString album;
    QString genre;
    QString releaseDate;
};

//Structure for the selected options for downloading
struct DownloadOptions {
    bool audioOnly;
    QString ffmpeg;
    QString codec;
    QString extension;

    QString outputName;
    QString thumbnail;

    bool addThumbnail;
    bool replaceVideoWithThumbnail;

    QString saveLocation;

    bool enableTrim;
    QString trimStart;
    QString trimEnd;

    QString url;

    Metadata metadata;
};

#endif // STRUCTURES_H
