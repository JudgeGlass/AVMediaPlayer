#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(player);
    player->setMedia(playlist);

    videoWidget = new VideoWidget(this);

    trayIcon = new QSystemTrayIcon;

    isMuted = false;
    listHiding = false;

    model = new QStringListModel;
    model->setStringList(list);

    ui->horizontalLayout_3->addWidget(videoWidget, 2);

    player->setVideoOutput(videoWidget);

    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::onDurationChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::progressChanged);

    ui->listView->setModel(model);
    ui->listView->setSelectionMode(QAbstractItemView::ExtendedSelection);


    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    videoWidget->setPalette(pal);
    videoWidget->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDurationChanged(qint64 d){
    ui->progressSlider->setMaximum(d / 1000);
    totalLength = d;
}

void MainWindow::progressChanged(qint64 d){
    ui->progressSlider->setValue(d / 1000);

    QModelIndex index = ui->listView->model()->index(playlist->currentIndex(), 0);
    ui->listView->setCurrentIndex(index);

    this->setWindowTitle("AVMediaPlayer - " + player->currentMedia().canonicalUrl().toString().mid(
                             player->currentMedia().canonicalUrl().toString().lastIndexOf("/")+1, player->currentMedia().canonicalUrl().toString().length()));

    updateStatus(d);
}

void MainWindow::updateStatus(qint64 d){
    QTime currentTime(0,0,0);
    currentTime = currentTime.addMSecs(d);
    QTime totalTime(0,0,0);
    totalTime = totalTime.addMSecs(totalLength);

    QString message = currentTime.toString("mm:ss") + " / " + totalTime.toString("mm:ss");
    qDebug() << message;
    ui->lblTimeStamp->setText(message);
}

void MainWindow::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
    qDebug() << "Resized!!!";
    videoWidget->setPalette(palette());
}

void MainWindow::on_actionplay_triggered()
{
    player->play();
}

void MainWindow::on_actionPause_triggered()
{
    player->pause();
}

void MainWindow::on_actionStop_triggered()
{
    player->stop();
}

void MainWindow::on_actionOpen_triggered()
{
    file = QFileDialog::getOpenFileNames(this, "Open File", "", "Media files(*.mp3 *.m4a *.ogg *.flac *.aac *.mp4 *.avi *.flv *.mkv *.mpeg *.mov *.mpg);;Audio files (*.mp3 *.m4a *.ogg *.flac *.aac);;Video files(*.mp4 *.avi *.flv *.mkv *.mpeg *.mov *.mpg);;All files (*.*)");
    if(!file.isEmpty()){
        for(const QString& i: file){
            playlist->addMedia(QUrl::fromLocalFile(i));
            list << i.mid(i.lastIndexOf("/")+1, i.length());
        }

        model->setStringList(list);

        activateComps(true);
    }
}

void MainWindow::on_actionURL_triggered()
{
    bool ok;
    QString URL = QInputDialog::getText(this, "URL", "Enter a URL", QLineEdit::Normal, "", &ok);
    if(ok){
        playlist->addMedia(QUrl(URL));
        list << URL.mid(URL.lastIndexOf("/")+1, URL.length());
        on_actionplay_triggered();
    }
}

void MainWindow::on_actionFullscreen_triggered()
{
    videoWidget->setFullScreen(true);
}

void MainWindow::on_btnPlay_clicked()
{

    isPlaying = true;
    on_actionplay_triggered();
}

void MainWindow::on_btnPause_clicked()
{
    isPlaying = false;
    on_actionPause_triggered();
}

void MainWindow::on_btnStop_clicked()
{
    isPlaying = false;
    on_actionStop_triggered();
}


void MainWindow::on_btnMute_clicked()
{
    on_actionMute_triggered();
}

void MainWindow::on_btnFullscreen_clicked()
{
    on_actionFullscreen_triggered();
}

void MainWindow::on_sliderVolume_sliderMoved(int position)
{
    player->setVolume(position);
    ui->lblVolume->setText(QString::number(position) + "%");
}

void MainWindow::on_progressSlider_sliderMoved(int position)
{
    player->setPosition(position * 1000);
}

void MainWindow::on_btnNTrack_clicked()
{
    playlist->next();
}

void MainWindow::on_btnPTrack_clicked()
{
    playlist->previous();
}

void MainWindow::on_btnShuffle_clicked()
{
    playlist->shuffle();
}

void MainWindow::on_actionClear_triggered()
{
    player->stop();
    playlist->clear();
    model->removeRows(0, model->rowCount());
    list.clear();
    activateComps(false);
}

void MainWindow::activateComps(const bool& act){
    ui->progressSlider->setEnabled(act);
    ui->btnStop->setEnabled(act);
    ui->btnShuffle->setEnabled(act);
    ui->btnFullscreen->setEnabled(act);
    ui->actionplay->setEnabled(act);
    ui->actionStop->setEnabled(act);
    ui->actionFullscreen->setEnabled(act);
}

void MainWindow::on_actionMute_triggered()
{
    if(isMuted){
        player->setMuted(false);
        ui->actionMute->setChecked(false);
        ui->btnMute->setText("Mute");
        isMuted = false;
    }else{
        player->setMuted(true);
        ui->actionMute->setChecked(true);
        ui->btnMute->setText("Unmute");
        isMuted = true;
    }
}

void MainWindow::on_actionHide_List_triggered()
{
    if(listHiding){
        ui->listView->setVisible(true);
        ui->actionHide_List->setChecked(false);
        videoWidget->repaint();
        listHiding = false;
    }else{
        ui->listView->setVisible(false);
        ui->actionHide_List->setChecked(true);
        videoWidget->repaint();
        listHiding = true;
    }
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog abtd;
    abtd.setModal(true);
    abtd.exec();
}

void MainWindow::on_btnRemove_clicked()
{
    QModelIndex index = ui->listView->currentIndex();
    if(index.row() != -1){
        playlist->removeMedia(index.row());
        model->removeRow(index.row());
    }else{
        QMessageBox dia;
        dia.information(0, "Nothing selected", "Nothing has been selected!");
        dia.show();
    }
}
