#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "overlayvideoplayer.h"
#include <QMainWindow>
#include <QLayout>
#include <QLabel>
#include <QPushButton>
#include <QSlider>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QVBoxLayout *mainLayout;
    OverlayVideoPlayer *videoPlayer;
    QLabel *progressLabel;
    QSlider *progressSlider;
    QPushButton *muteButton;
    QSlider *volumeSlider;

    void createLayout();

    QIcon volumeNormalIcon = QIcon::fromTheme(QIcon::ThemeIcon::AudioVolumeMedium);
    QIcon volumeMutedIcon = QIcon::fromTheme(QIcon::ThemeIcon::AudioVolumeMuted);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionFile_triggered();

    void on_actionURL_triggered();

    void on_actionClose_triggered();

    void on_actionExit_triggered();

    void onPlayStatusChanged(bool);
    void onPlaybackEnded();

    void on_actionAbout_Qt_triggered();

    void on_actionAbout_triggered();

    void onMuteChanged(bool);
    void onMuteClicked(bool);
    void onVolumeSliderChanged(int);
    void onVideoProgressChanged(qint64, qint64);
    void onProgressSliderChanged(int);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
