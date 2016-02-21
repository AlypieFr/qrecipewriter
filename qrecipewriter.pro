#-------------------------------------------------
#
# Project created by QtCreator 2014-10-19T17:56:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qrecipewriter
TEMPLATE = app


SOURCES += main.cpp\
        qrecipewriter.cpp \
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
    qinputmultiline.cpp \
    coupdecoeur.cpp \
    opendistant.cpp \
    Json.cpp \
    searchupdates.cpp

HEADERS  += qrecipewriter.h \
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
    qinputmultiline.h \
    coupdecoeur.h \
    opendistant.h \
    Json.h \
    searchupdates.h

FORMS    += qrecipewriter.ui \
    options.ui \
    categories.ui \
    apropos.ui \
    sendmanual.ui \
    sendautomatic.ui \
    spellcheckdialog.ui \
    insertvideo.ui \
    insertpicture.ui \
    qinputmultiline.ui \
    coupdecoeur.ui \
    opendistant.ui

RESOURCES += \
    Ressources.qrc

CONFIG += wwwidgets

unix {
    INCLUDEPATH += /usr/include/hunspell
    contains(QMAKE_HOST.arch, x86_64) {
        exists("/usr/lib/x86_64-linux-gnu/libhunspell-1.3.a") {
            LIBS += /usr/lib/x86_64-linux-gnu/libhunspell-1.3.a
        }
        !exists("/usr/lib/x86_64-linux-gnu/libhunspell-1.3.a") {
            exists("/usr/lib64/libhunspell-1.3.so") {
                LIBS += /usr/lib64/libhunspell-1.3.so
            }
            !exists("/usr/lib64/libhunspell-1.3.so") {
                exists("/usr/lib/libhunspell-1.3.so") {
                    LIBS += /usr/lib/libhunspell-1.3.so
                }
                !exists("/usr/lib/libhunspell-1.3.so") {
                    message("ERROR: libhunspell-1.3.so not found. Please edit qrecipewriter.pro file and set the correct location")
                }
            }
        }
        !greaterThan(QT_MAJOR_VERSION, 5) {
            exists("/usr/lib/x86_64-linux-gnu/libqjson.so") {
                LIBS += /usr/lib/x86_64-linux-gnu/libqjson.so
            }
            !exists("/usr/lib/x86_64-linux-gnu/libqjson.so") {
                exists("/usr/lib64/libqjson.so") {
                    LIBS += /usr/lib64/libqjson.so
                }
                !exists("/usr/lib64/libqjson.so") {
                    exists("/usr/lib/libqjson.so") {
                        LIBS += /usr/lib/libqjson.so
                    }
                    !exists("/usr/lib/libqjson.so") {
                        message("ERROR: libqjson.so not found. Please edit qrecipewriter.pro file and set the correct location")
                    }
                }
            }
        }
    }
    else {
        exists("/usr/lib/i386-linux-gnu/libhunspell-1.3.a") {
            LIBS += /usr/lib/i386-linux-gnu/libhunspell-1.3.a
        }
        !exists("/usr/lib/i386-linux-gnu/libhunspell-1.3.a") {
            exists("/usr/lib32/libhunspell-1.3.so") {
                LIBS += /usr/lib32/libhunspell-1.3.so
            }
            !exists("/usr/lib32/libhunspell-1.3.so") {
                exists("/usr/lib/libhunspell-1.3.so") {
                    LIBS += /usr/lib/libhunspell-1.3.so
                }
                !exists("/usr/lib/libhunspell-1.3.so") {
                    message("ERROR: libhunspell-1.3.so not found. Please edit qrecipewriter.pro file and set the correct location")
                }
            }
        }
        !greaterThan(QT_MAJOR_VERSION, 5) {
            exists("/usr/lib/i386-linux-gnu/libqjson.so") {
                LIBS += /usr/lib/i386-linux-gnu/libqjson.so
            }
            !exists("/usr/lib/i386-linux-gnu/libqjson.so") {
                exists("/usr/lib32/libqjson.so") {
                    LIBS += /usr/lib32/libqjson.so
                }
                !exists("/usr/lib32/libqjson.so") {
                    exists("/usr/lib/libqjson.so") {
                        LIBS += /usr/lib/libqjson.so
                    }
                    !exists("/usr/lib/libqjson.so") {
                        message("ERROR: libqjson.so not found. Please edit qrecipewriter.pro file and set the correct location")
                    }
                }
            }
        }
    }

    ICONS = icons/128x128 icons/48x48 icons/96x96
    INSTICONS.path = /usr/share/icons/hicolor
    INSTICONS.files = $$ICONS
    INSTALLS += INSTICONS

    DESK = applications/QRecipeWriter.desktop
    INSTDESK.path = /usr/share/applications
    INSTDESK.files = $$DESK
    INSTALLS += INSTDESK

    SHARE = LICENCE.txt modeleHtml wordpress
    INSTSHARE.path = /usr/share/qrecipewriter
    INSTSHARE.files = $$SHARE
    INSTALLS += INSTSHARE

    BIN = qrecipewriter
    INSTBIN.path = /usr/bin
    INSTBIN.files = $$BIN
    INSTALLS += INSTBIN
}

win32 {
    INCLUDEPATH += C:/hunspell/include
    contains(QMAKE_HOST.arch, x86_64) {
        !exists("C:/hunspellX64/lib/libhunspell-1.3.a") {
            message("Error: hunspell lib not found: C:/hunspellX64/lib/libhunspell-1.3.a");
        }
        LIBS += C:/hunspellX64/lib/libhunspell-1.3.a
    }
    else {
        !exists("C:/hunspellI686/lib/libhunspell-1.3.a") {
            message("Error: hunspell lib not found: C:/hunspellI686/lib/libhunspell-1.3.a");
        }
     	LIBS += C:/hunspellI686/lib/libhunspell-1.3.a   
    }
    RC_FILE = myapp.rc
    TARGET = QRecipeWriter
}

DISTFILES +=
