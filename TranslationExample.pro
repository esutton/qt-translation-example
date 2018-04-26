#-------------------------------------------------
#
# Project created by QtCreator 2018-04-26T08:01:08
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TranslationExample
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

# Copy the compiled *.qm translation files for deployment with app
# Using $$files() to expand wildcard pattern
CompiledTranslationFiles = $$files($${PWD}/translation/*.qm)
CompiledTranslationFiles = $${CompiledTranslationFiles}
CompiledTranslationFiles ~= s,/,\\,g
message("QMAKE_POST_LINK: Copy qm Files to: $$OUTDIR_WIN" )
for(FILE,CompiledTranslationFiles){
    QMAKE_POST_LINK +=$$quote(cmd /c copy /y $${FILE} $${OUTDIR_WIN}$$escape_expand(\n\t))
}


TRANSLATIONS += \
    languages/TranslationExample_en.ts  \
    languages/TranslationExample_de.ts

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui

CONFIG += mobility
MOBILITY = 

DISTFILES += \
    languages/TranslationExample_de.ts \
    languages/TranslationExample_en.ts \
    README.md

RESOURCES += \
    res/translation.qrc

