QT += core concurrent

SOURCES += \
    $$PWD/csiopencvmodule.cpp

HEADERS += \
    $$PWD/csiopencvmodule.h

INCLUDEPATH += /usr/include/opencv4 #Add header file path

LIBS += -L/usr/lib/aarch64-linux-gnu -lopencv_core -lopencv_imgcodecs -lopencv_imgproc -lopencv_highgui -lopencv_objdetect -lopencv_videoio  #Add libraries that need to be linked

