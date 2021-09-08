#-------------------------------------------------
#
# Project created by QtCreator 2018-05-26T18:36:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

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

#---------------------------------------------------------------------------------------------------
# Version
#---------------------------------------------------------------------------------------------------

DEFINES += MAJOR_VERSION=1
DEFINES += MINOR_VERSION=0

#---------------------------------------------------------------------------------------------------
# Target
#---------------------------------------------------------------------------------------------------
CONFIG(debug, debug|release) {
TARGET = QemuFrontend_d
DEFINES += DEBUG
} else {
TARGET = QemuFrontend
}

#---------------------------------------------------------------------------------------------------
# Destination directory
#---------------------------------------------------------------------------------------------------
DESTDIR = ../bin

#---------------------------------------------------------------------------------------------------
# Source files
#---------------------------------------------------------------------------------------------------

SOURCES += \
    src/main.cpp \
    src/MainWindow.cpp \
    src/AboutDialog.cpp \
    src/TreeModel.cpp \
    src/TreeItem.cpp \
    src/utils/ShowInContainerFolder.cpp \
    src/Languages.cpp \
    src/RecentFilesMenu.cpp \
    src/QemuProcess.cpp \
    src/dlgs/ConfigureMachineDialog.cpp \
    src/dlgs/ConfigureCdromDialog.cpp \
    src/dlgs/ConfigureFloppyDialog.cpp \
    src/dlgs/ConfigureParallelDialog.cpp \
    src/dlgs/ConfigureKeyboardDialog.cpp \
    src/dlgs/ConfigureTabletDialog.cpp \
    src/dlgs/ConfigureMouseDialog.cpp \
    src/dlgs/ConfigureDisplayDialog.cpp \
    src/dlgs/ConfigureAudioDialog.cpp \
    src/dlgs/ConfigureMemoryDialog.cpp \
    src/dlgs/ConfigureNetworkInterfaceDialog.cpp \
    src/dlgs/AddMachineDialog.cpp

HEADERS += \
    src/MainWindow.h \
    src/AboutDialog.h \
    src/TreeModel.h \
    src/TreeItem.h \
    src/Languages.h \
    src/RecentFilesMenu.h \
    src/QemuProcess.h \
    src/dlgs/ConfigureMachineDialog.h \
    src/dlgs/ConfigureCdromDialog.h \
    src/dlgs/ConfigureFloppyDialog.h \
    src/dlgs/ConfigureParallelDialog.h \
    src/dlgs/ConfigureKeyboardDialog.h \
    src/dlgs/ConfigureTabletDialog.h \
    src/dlgs/ConfigureMouseDialog.h \
    src/dlgs/ConfigureDisplayDialog.h \
    src/dlgs/ConfigureAudioDialog.h \
    src/dlgs/ConfigureMemoryDialog.h \
    src/dlgs/ConfigureNetworkInterfaceDialog.h \
    src/dlgs/AddMachineDialog.h

FORMS += \
    src/MainWindow.ui \
    src/AboutDialog.ui \
    src/dlgs/ConfigureMachineDialog.ui \
    src/dlgs/ConfigureCdromDialog.ui \
    src/dlgs/ConfigureFloppyDialog.ui \
    src/dlgs/ConfigureParallelDialog.ui \
    src/dlgs/ConfigureKeyboardDialog.ui \
    src/dlgs/ConfigureTabletDialog.ui \
    src/dlgs/ConfigureMouseDialog.ui \
    src/dlgs/ConfigureDisplayDialog.ui \
    src/dlgs/ConfigureAudioDialog.ui \
    src/dlgs/ConfigureMemoryDialog.ui \
    src/dlgs/ConfigureNetworkInterfaceDialog.ui \
    src/dlgs/AddMachineDialog.ui

#---------------------------------------------------------------------------------------------------
# Resource files
#---------------------------------------------------------------------------------------------------

RESOURCES += \
    res/resource.qrc

win32:RC_FILE = res/resource_win32.rc

#---------------------------------------------------------------------------------------------------
# Translation files
#---------------------------------------------------------------------------------------------------
TRANSLATIONS =	translate/qmenufronted_es.ts \
                translate/qmenufronted_en.ts

