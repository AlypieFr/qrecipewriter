#-------------------------------------------------
#
# Project created by QtCreator 2014-10-19T17:56:27
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qrecipewriter
TEMPLATE = app

INCLUDEPATH += headers
INCLUDEPATH += src
INCLUDEPATH += ui

SOURCES += src/main.cpp\
    src/qrecipewriter.cpp \
    src/options.cpp \
    src/categories.cpp \
    src/listviewdelegate.cpp \
    src/functions.cpp \
    src/apropos.cpp \
    src/tinyxml2.cpp \
    src/sendmanual.cpp \
    src/mylineedit.cpp \
    src/myplaintextedit.cpp \
    src/sendautomatic.cpp  \
    src/spellcheckdialog.cpp \
    src/spellchecker.cpp \
    src/filedownloader.cpp \
    src/insertvideo.cpp \
    src/insertpicture.cpp \
    src/qinputmultiline.cpp \
    src/coupdecoeur.cpp \
    src/opendistant.cpp \
    src/searchupdates.cpp

HEADERS  += headers/qrecipewriter.h \
    headers/options.h \
    headers/categories.h \
    headers/listviewdelegate.h \
    headers/functions.h \
    headers/apropos.h \
    headers/tinyxml2.h \
    headers/sendmanual.h \
    headers/mylineedit.h \
    headers/myplaintextedit.h \
    headers/sendautomatic.h \
    headers/spellcheckdialog.h \
    headers/spellchecker.h \
    headers/filedownloader.h \
    headers/insertvideo.h \
    headers/insertpicture.h \
    headers/qinputmultiline.h \
    headers/coupdecoeur.h \
    headers/opendistant.h \
    headers/searchupdates.h

FORMS    += ui/qrecipewriter.ui \
    ui/options.ui \
    ui/categories.ui \
    ui/apropos.ui \
    ui/sendmanual.ui \
    ui/sendautomatic.ui \
    ui/spellcheckdialog.ui \
    ui/insertvideo.ui \
    ui/insertpicture.ui \
    ui/qinputmultiline.ui \
    ui/coupdecoeur.ui \
    ui/opendistant.ui

RESOURCES += \
    Ressources.qrc

CONFIG += wwwidgets

TRANSLATIONS+=  i18n/qrecipewriter_en.ts
TRANSLATIONS+=  i18n/qrecipewriter_fr.ts

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
                    LIBS += /usr/lib/libhunspell.so
                    !exists("/usr/lib/libhunspell.so") {
                        message("ERROR: libhunspell.so not found. Please edit qrecipewriter.pro file and set the correct location")
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
                    LIBS += /usr/lib/libhunspell.so
                    !exists("/usr/lib/libhunspell.so") {
                        message("ERROR: libhunspell.so not found. Please edit qrecipewriter.pro file and set the correct location")
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
    contains(QMAKE_HOST.arch, x86_64) {
        INCLUDEPATH += C:/hunspellX64/include
        !exists("C:/hunspellX64/lib/libhunspell-1.3.a") {
            message("Error: hunspell lib not found: C:/hunspellX64/lib/libhunspell-1.3.a");
        }
        LIBS += C:/hunspellX64/lib/libhunspell-1.3.a
    }
    else {
        INCLUDEPATH += C:/hunspellI686/include
        !exists("C:/hunspellI686/lib/libhunspell-1.3.a") {
            message("Error: hunspell lib not found: C:/hunspellI686/lib/libhunspell-1.3.a");
        }
     	LIBS += C:/hunspellI686/lib/libhunspell-1.3.a   
    }
    RC_FILE = myapp.rc
    TARGET = QRecipeWriter
}

DISTFILES +=
