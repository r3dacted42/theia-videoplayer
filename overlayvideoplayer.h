#ifndef OVERLAYVIDEOPLAYER_H
#define OVERLAYVIDEOPLAYER_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>

class OverlayVideoPlayer : public QWidget
{
    Q_OBJECT

    QGraphicsScene *scene;
    QGraphicsView *view;
    QGraphicsVideoItem *videoItem;
    QMediaPlayer *player;
    QAudioOutput *audio;
    QGraphicsPixmapItem *logoItem;

protected:
    void resizeEvent(QResizeEvent *event) override;

    void mouseDoubleClickEvent(QMouseEvent *) override;

public:
    explicit OverlayVideoPlayer(QWidget *parent = nullptr);

    void setVideoUrl(QUrl url);
    void closeVideo();
    void setVolume(int);
    void toggleMuted();
    void setPosition(qint64);

signals:
    void playStatusChanged(bool isPlaying);
    void playbackEnded();
    void muteChanged(bool muted);
    void videoProgressChanged(qint64 current, qint64 total);

private slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus);
    void onPositionChanged(qint64);
};

#endif // OVERLAYVIDEOPLAYER_H
