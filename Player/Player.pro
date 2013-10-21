# Add more folders to ship with the application, here
folder_01.source = Views
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    WidgetUtils.cpp \
    UrlModel.cpp \
    UpdateModel.cpp \
    UpdateControler.cpp \
    TVPage.cpp \
    Titlebar.cpp \
    TabWidget.cpp \
    TabPagesController.cpp \
    TabPage.cpp \
    RadioPage.cpp \
    NcFramelessHelper.cpp \
    MainWindow.cpp \
    Log.cpp \
    IdleInputController.cpp \
    GenreModel.cpp \
    FileDownloader.cpp \
    EntryPoint.cpp \
    DownlaodController.cpp \
    DefaultBrowserController.cpp \
    ConfigModel.cpp \
    CheatController.cpp \
    ChannelSourceModel.cpp \
    ChannelMetadataModel.cpp \
    AudioStreamController.cpp \
    AppController.cpp

# Installation path
# target.path =

# Please do not modify the following two lines. Required for deployment.
include(qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    WidgetUtils.h \
    UrlModel.h \
    UpdateModel.h \
    UpdateControler.h \
    TVPage.h \
    Titlebar.h \
    TabWidget.h \
    TabPagesController.h \
    TabPage.h \
    RadioPage.h \
    NcFramelessHelper.h \
    MainWindow.h \
    Log.h \
    IdleInputController.h \
    GenreModel.h \
    FileDownloader.h \
    DownlaodController.h \
    DefaultBrowserController.h \
    ConfigModel.h \
    CheatController.h \
    ChannelSourceModel.h \
    ChannelMetadataModel.h \
    AudioStreamController.h \
    AppController.h
