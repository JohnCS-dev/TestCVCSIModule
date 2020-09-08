#include "testwindow.h"
#include "ui_testwindow.h"

TestWindow::TestWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TestWindow)
{
    ui->setupUi(this);
    bool SciModuleOpened = fSciModule.open(1280, 720, 30);
    bool streamStarted = fSciModule.startVideoStream();
    qDebug() << (SciModuleOpened ? "Sci module opened." : "Sci module not opened.") << (streamStarted ? "Video stream started." : "Video stream not started.");
}

TestWindow::~TestWindow()
{
    delete ui;
}

void TestWindow::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == timerID) {
        on_pbGetFrame_clicked();
    }
}

void TestWindow::on_pbGetFrame_clicked()
{
    QImage img = fSciModule.getFrame();
    ui->labelFrame->setPixmap(QPixmap::fromImage(img));
}

void TestWindow::on_pbStartTimer_clicked()
{
    timerID = startTimer(ui->sbInterval->value());
}

void TestWindow::on_pbStopTimer_clicked()
{
    killTimer(timerID);
}
