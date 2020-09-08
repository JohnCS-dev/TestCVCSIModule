#include "csiopencvmodule.h"

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc.hpp>
#include <opencv4/opencv2/imgproc/types_c.h>

#include <QtConcurrent>
#include <QMutexLocker>

using namespace std;

std::string CSIOpenCVModule::gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method)
{
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + to_string(capture_width) + ", height=(int)" +
           to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + to_string(flip_method) + " ! video/x-raw, width=(int)" + to_string(display_width) + ", height=(int)" +
            to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
}

void CSIOpenCVModule::videoStream()
{
    cv::Mat img;
    while (!stopVideoStream && cap.isOpened()) {
        cap.read(img);
        QMutexLocker lock(&frameMutex);
        lastFrame = img.clone();
    }
}

CSIOpenCVModule::CSIOpenCVModule()
{
    stopVideoStream = true;
}

CSIOpenCVModule::~CSIOpenCVModule()
{
    if (!stopVideoStream) {
        stopVideoStream = true;
        future.waitForFinished();
    }
    cap.release();
}

bool CSIOpenCVModule::open(int width, int height, int framerate)
{
    //Create pipe
    string pipeline = gstreamer_pipeline(width, height, width, height, framerate, 0);
    return cap.open(pipeline, cv::CAP_GSTREAMER);
}

QImage CSIOpenCVModule::getFrame()
{
    if (stopVideoStream) {
        cv::Mat img;
        if (cap.isOpened() && cap.read(img))
        {
            cv::Mat temp; // make the same cv::Mat
            cv::cvtColor(img, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
            QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
            dest.bits(); // enforce deep copy, see documentation
            // of QImage::QImage ( const uchar * data, int width, int height, Format format )
            return dest;
        }
    }
    else {
        cv::Mat temp; // make the same cv::Mat
        {
            QMutexLocker lock(&frameMutex);
            cv::cvtColor(lastFrame, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
        }
        QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
        dest.bits(); // enforce deep copy, see documentation
        // of QImage::QImage ( const uchar * data, int width, int height, Format format )
        return dest;
    }
    return QImage();
}

std::vector<uchar> CSIOpenCVModule::getCompressedFrame(int quality)
{
    vector<uchar> imgBuf;  // Буфер для сжатого изображения
    if (stopVideoStream) {
        cv::Mat img;
        if (cap.isOpened() && cap.read(img))
        {
            vector<int> quality_params = vector<int>(2); // Вектор параметров качества сжатия
            quality_params[0] = cv::IMWRITE_JPEG_QUALITY; // Кодек JPEG
            quality_params[1] = quality;
            cv::imencode(".jpg", img, imgBuf, quality_params);
        }
    }
    else {
        vector<int> quality_params = vector<int>(2); // Вектор параметров качества сжатия
        quality_params[0] = cv::IMWRITE_JPEG_QUALITY; // Кодек JPEG
        quality_params[1] = quality;
        QMutexLocker lock(&frameMutex);
        cv::imencode(".jpg", lastFrame, imgBuf, quality_params);
    }
    return imgBuf;
}

bool CSIOpenCVModule::startVideoStream()
{
    stopVideoStream = false;
    future = QtConcurrent::run(this, &CSIOpenCVModule::videoStream);
    return future.isStarted();
}
