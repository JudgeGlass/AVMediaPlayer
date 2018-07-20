#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QFileDialog>
#include <QMediaPlaylist>
#include <QTime>
#include <QInputDialog>
#include <QKeyEvent>
#include <QStringListModel>
#include <QMessageBox>

#include "videowidget.h"
#include "aboutdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionplay_triggered();

    void on_actionPause_triggered();

    void on_actionStop_triggered();

    void on_actionOpen_triggered();

    void onDurationChanged(qint64 d);

    void progressChanged(qint64 d);

    void updateStatus(qint64 d);

    void on_actionURL_triggered();

    void on_actionFullscreen_triggered();

    void on_btnPlay_clicked();

    void on_btnPause_clicked();

    void on_btnStop_clicked();

    void on_btnMute_clicked();

    void on_btnFullscreen_clicked();

    void on_sliderVolume_sliderMoved(int position);

    void on_progressSlider_sliderMoved(int position);

    void on_btnNTrack_clicked();

    void on_btnPTrack_clicked();

    void on_btnShuffle_clicked();

    void on_actionClear_triggered();

    void on_actionMute_triggered();

    void on_actionHide_List_triggered();

    void resizeEvent(QResizeEvent *event);

    void on_actionAbout_triggered();

    void on_btnRemove_clicked();

private:
    void activateComps(const bool& act);

    Ui::MainWindow *ui;

    QMediaPlayer* player;
    QVideoWidget* videoWidget;
    QStringList file;

    qint64 totalLength;

    bool isMuted;
    bool isPlaying;
    bool listHiding;

    QMediaPlaylist* playlist;
    QStringListModel* model;
    QStringList list;
};

#endif // MAINWINDOW_H
