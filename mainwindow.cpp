#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QTextEdit>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("Theia Video Player"));
    setWindowIcon(QIcon(":/images/icon.png"));
    createLayout();
    ui->actionClose->setEnabled(false);

    connect(videoPlayer, SIGNAL(playStatusChanged(bool)), this, SLOT(onPlayStatusChanged(bool)));
    connect(videoPlayer, SIGNAL(playbackEnded()), this, SLOT(onPlaybackEnded()));
    connect(videoPlayer, SIGNAL(muteChanged(bool)), this, SLOT(onMuteChanged(bool)));
    connect(muteButton, SIGNAL(clicked(bool)), this, SLOT(onMuteClicked(bool)));
    connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(onVolumeSliderChanged(int)));
    connect(videoPlayer, SIGNAL(videoProgressChanged(qint64,qint64)), this, SLOT(onVideoProgressChanged(qint64,qint64)));
    connect(progressSlider, SIGNAL(valueChanged(int)), this, SLOT(onProgressSliderChanged(int)));
    ui->statusbar->showMessage(QString("Welcome! Please select a video using the file menu"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createLayout()
{
    videoPlayer = new OverlayVideoPlayer;
    mainLayout = new QVBoxLayout;
    progressLabel = new QLabel(tr("--:--/--:--"));
    progressSlider = new QSlider(Qt::Orientation::Horizontal);
    progressSlider->setEnabled(false);
    muteButton = new QPushButton;
    muteButton->setIcon(volumeNormalIcon);
    volumeSlider = new QSlider(Qt::Orientation::Horizontal);
    volumeSlider->setMaximum(100);
    volumeSlider->setMinimum(0);
    volumeSlider->setValue(100);
    volumeSlider->setMaximumWidth(85);
    mainLayout->addWidget(videoPlayer);
    auto hbox = new QHBoxLayout;
    hbox->setContentsMargins(10, -10, 10, 5);
    hbox->setAlignment(Qt::AlignTop);
    hbox->addWidget(progressLabel);
    hbox->addWidget(progressSlider);
    hbox->addWidget(muteButton);
    hbox->addWidget(volumeSlider);
    mainLayout->addLayout(hbox);
    setMinimumSize(QSize(660, 500));
    ui->centralwidget->setLayout(mainLayout);
}

void MainWindow::onPlayStatusChanged(bool isPlaying)
{
    if (isPlaying) {
        ui->statusbar->showMessage(QString("Playing video"), 5000);
    } else {
        ui->statusbar->showMessage(QString("Paused video"), 5000);
    }
}

void MainWindow::onPlaybackEnded()
{
    ui->statusbar->showMessage(QString("Video ended"), 5000);
}

void MainWindow::onMuteChanged(bool muted)
{
    if (muted) {
        muteButton->setIcon(volumeMutedIcon);
    } else {
        muteButton->setIcon(volumeNormalIcon);
    }
}

void MainWindow::onMuteClicked(bool _)
{
    videoPlayer->toggleMuted();
}

void MainWindow::onVolumeSliderChanged(int value)
{
    videoPlayer->setVolume(value);
}

void MainWindow::onVideoProgressChanged(qint64 pos, qint64 dur)
{
    auto curSec = QString::number((pos / 1000) % 60).rightJustified(2, '0'), curMin = QString::number((pos / 1000) / 60).rightJustified(2, '0');
    auto totSec = QString::number((dur / 1000) % 60).rightJustified(2, '0'), totMin = QString::number((dur / 1000) / 60).rightJustified(2, '0');
    progressLabel->setText(QString("%1:%2/%3:%4").arg(curMin, curSec, totMin, totSec));
    if (!progressSlider->isSliderDown()) {
        progressSlider->setMaximum(dur / 10);
        progressSlider->setValue(pos / 10);
        progressSlider->setEnabled(true);
    }
}

void MainWindow::onProgressSliderChanged(int v)
{
    if (progressSlider->isSliderDown()){
        videoPlayer->setPosition(v * 10);
    }
}

void MainWindow::on_actionFile_triggered()
{
    auto fileUrl = QFileDialog::getOpenFileUrl(this, tr("Select Video"), QUrl(),
                                               tr("Video Files (*.mp4 *.mov *.avi *.wmv *.mkv *.flv *.m4v)"));
    if (!fileUrl.isEmpty()) {
        videoPlayer->setVideoUrl(fileUrl);
        ui->statusbar->showMessage(QString("Loaded file %1").arg(fileUrl.fileName()), 5000);
        ui->actionClose->setEnabled(true);
    }
}


void MainWindow::on_actionURL_triggered()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Open URL"),
                                         tr("Video URL:"), QLineEdit::Normal,
                                         tr("https://"), &ok);
    if (ok && !text.isEmpty()) {
        videoPlayer->setVideoUrl(QUrl(text));
        ui->statusbar->showMessage(QString("Loaded URL %1").arg(text), 5000);
        ui->actionClose->setEnabled(true);
    }
}


void MainWindow::on_actionClose_triggered()
{
    videoPlayer->closeVideo();
    ui->statusbar->showMessage(QString("Closed video"), 5000);
    progressSlider->setValue(0);
    progressLabel->setText(tr("--:--/--:--"));
    progressSlider->setEnabled(false);
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}


void MainWindow::on_actionAbout_Qt_triggered()
{
    QApplication::aboutQt();
}


void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "About",
                             "This application was made as a submission for an assignment by \
                            Theia Technologies. Made by Priyansh Agrahari (2024).");
}

