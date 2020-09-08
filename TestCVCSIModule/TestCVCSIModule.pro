#-------------------------------------------------
#
# Project created by QtCreator 2020-09-07T09:49:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TestCVCSIModule
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        testwindow.cpp

HEADERS += \
        testwindow.h

FORMS += \
        testwindow.ui

include(TCPTransmissionModule/tcptransmissionmodule.pri)
include(CSIOpenCVModule/CSIOpenCVModule.pri)

#INCLUDEPATH += /usr/include/opencv4 #Add header file path

#LIBS += -L/usr/lib/aarch64-linux-gnu -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_videoio  #Add libraries that need to be linked
