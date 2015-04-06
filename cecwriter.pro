#-------------------------------------------------
#
# Project created by QtCreator 2014-10-19T17:56:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CeCWriter
TEMPLATE = app


SOURCES += main.cpp\
        cecwriter.cpp \
    options.cpp \
    categories.cpp \
    listviewdelegate.cpp \
    functions.cpp \
    apropos.cpp \
    tinyxml2.cpp \
    sendmanual.cpp \
    mylineedit.cpp \
    myplaintextedit.cpp \
    sendautomatic.cpp  \
    spellcheckdialog.cpp \
    spellchecker.cpp \
    filedownloader.cpp \
    insertvideo.cpp \
    insertpicture.cpp \
    qinputmultiline.cpp

HEADERS  += cecwriter.h \
    options.h \
    categories.h \
    listviewdelegate.h \
    functions.h \
    apropos.h \
    tinyxml2.h \
    sendmanual.h \
    mylineedit.h \
    myplaintextedit.h \
    sendautomatic.h \
    spellcheckdialog.h \
    spellchecker.h \
    filedownloader.h \
    insertvideo.h \
    insertpicture.h \
    qinputmultiline.h

FORMS    += cecwriter.ui \
    options.ui \
    categories.ui \
    apropos.ui \
    sendmanual.ui \
    sendautomatic.ui \
    spellcheckdialog.ui \
    insertvideo.ui \
    insertpicture.ui \
    qinputmultiline.ui

RESOURCES += \
    Ressources.qrc

CONFIG += wwwidgets

unix {
    INCLUDEPATH += /usr/include/hunspell
    contains(QMAKE_HOST.arch, x86_64) {
        LIBS += /usr/lib/x86_64-linux-gnu/libhunspell-1.3.a
    }
    else {
        LIBS += /usr/lib/i386-linux-gnu/libhunspell-1.3.a
    }
}

win32 {
    INCLUDEPATH += C:/hunspell/include
    LIBS += C:/hunspell/lib/libhunspell-1.3.a
    RC_FILE = myapp.rc
}
