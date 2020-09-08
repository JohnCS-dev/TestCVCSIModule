#ifndef CSIOPENCVMODULE_H
#define CSIOPENCVMODULE_H

#include <string>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <QImage>
#include <QFuture>
#include <QMutex>

class CSIOpenCVModule
{
public:
    CSIOpenCVModule();
    ~CSIOpenCVModule();
    bool open(int width, int height, int framerate);
    QImage getFrame();
    std::vector<uchar> getCompressedFrame(int quality = 20); // По умолчанию качество сжатия (20) 0-100
    bool startVideoStream();

private:
    cv::VideoCapture cap;
    std::string gstreamer_pipeline(int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method);
    QFuture<void> future;
    void videoStream();
    cv::Mat lastFrame;
    QMutex frameMutex;
    volatile bool stopVideoStream;
};

#endif // CSIOPENCVMODULE_H
