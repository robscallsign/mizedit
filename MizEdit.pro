QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += static

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mizedit.cpp \
    qtmisc.cpp

HEADERS += \
    mainwindow.h \
    mizedit.h \
    qtmisc.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


INCLUDEPATH+=$$(ZLIB_INCLUDE)
INCLUDEPATH+=$$(QUAZIP_INCLUDE)

LIBS += -L$$(ZLIB_LIB) -lzlib

CONFIG( debug, debug|release) {
## debug
    LIBS += -L$$(QUAZIP_LIB) -lquazipd
} else {
## release
    LIBS += -L$$(QUAZIP_LIB) -lquazip
}
