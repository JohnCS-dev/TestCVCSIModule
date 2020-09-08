#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include "csiopencvmodule.h"

namespace Ui {
class TestWindow;
}

class TestWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TestWindow(QWidget *parent = 0);
    ~TestWindow();

protected:
    void timerEvent(QTimerEvent *event);

private slots:
    void on_pbGetFrame_clicked();

    void on_pbStartTimer_clicked();

    void on_pbStopTimer_clicked();

private:
    Ui::TestWindow *ui;
    CSIOpenCVModule fSciModule;
    int timerID;
};

#endif // TESTWINDOW_H
