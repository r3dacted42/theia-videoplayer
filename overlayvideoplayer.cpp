#include "overlayvideoplayer.h"
#include <QVBoxLayout>

OverlayVideoPlayer::OverlayVideoPlayer(QWidget *parent)
    : QWidget{parent}
{
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);

    videoItem = new QGraphicsVideoItem;
    audio = new QAudioOutput;
    player = new QMediaPlayer;
    player->setVideoOutput(videoItem);
    player->setAudioOutput(audio);
    audio->setVolume(1);
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(onMediaStatusChanged(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(onPositionChanged(qint64)));

    videoItem->setAspectRatioMode(Qt::KeepAspectRatio);
    videoItem->setSize(QSize(400, 300));
    scene->addItem(videoItem);

    QImage img;
    img.load(":/images/logo.png");
    auto logoPixmap = QPixmap::fromImage(img);
    logoPixmap = logoPixmap.scaled(560, 200, Qt::KeepAspectRatio);
    logoItem = scene->addPixmap(logoPixmap);
    logoItem->setOffset(-280, -100);

    setLayout(new QVBoxLayout());
    this->layout()->addWidget(view);
}

void OverlayVideoPlayer::resizeEvent(QResizeEvent *event)
{
    auto rect = this->contentsRect();
    int diff = 25;
    rect.setSize(QSize(rect.size().width() - diff, rect.size().height() - diff));
    scene->setSceneRect(rect);
    videoItem->setSize(rect.size());
    videoItem->setOffset(QPointF(-rect.width() / 2, -rect.height() / 2));
    auto logoPos = QPointF(rect.width() / 2, rect.height() / 2);
    logoItem->setPos(logoPos);
    videoItem->setPos(logoPos);
    view->centerOn(logoPos);
}

void OverlayVideoPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (player->mediaStatus() != QMediaPlayer::NoMedia){
        if (player->isPlaying()) {
            player->pause();
            logoItem->setVisible(true);
            emit playStatusChanged(false);
        } else {
            player->play();
            logoItem->setVisible(false);
            emit playStatusChanged(true);
        }
    }
}

void OverlayVideoPlayer::setVideoUrl(QUrl url)
{
    player->setSource(url);
    player->play();
    logoItem->setVisible(false);
}

void OverlayVideoPlayer::closeVideo()
{
    player->setSource(QUrl());
    logoItem->setVisible(true);
}

void OverlayVideoPlayer::setVolume(int v)
{
    if (v == 0) {
        audio->setMuted(true);
        emit muteChanged(true);
    } else if (audio->isMuted()) {
        audio->setMuted(false);
        emit muteChanged(false);
    }
    float target = 0.01 * v;
    audio->setVolume(target);
}

void OverlayVideoPlayer::toggleMuted()
{
    if (audio->isMuted()) {
        audio->setMuted(false);
        emit muteChanged(false);
    } else {
        audio->setMuted(true);
        emit muteChanged(true);
    }
}

void OverlayVideoPlayer::setPosition(qint64 pos)
{
    if (pos <= player->duration()) {
        player->setPosition(pos);
    }
}

void OverlayVideoPlayer::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::MediaStatus::EndOfMedia)
    {
        logoItem->setVisible(true);
        emit playbackEnded();
    }
}

void OverlayVideoPlayer::onPositionChanged(qint64 pos)
{

    emit videoProgressChanged(pos, player->duration());
}
