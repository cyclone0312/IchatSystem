QT       += core gui
QT       += core network
QT       += widgets
QT       += sql
RESOURCES += resource/tubiao.qrc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addfriends.cpp \
    autocleartextedit.cpp \
    changeinformation.cpp \
    changepassword.cpp \
    choicedialog.cpp \
    cutavator.cpp \
    dialog.cpp \
    findpassword.cpp \
    friendmessage.cpp \
    logout.cpp \
    main.cpp \
    login.cpp \
    mainwindow-else.cpp \
    mainwindow.cpp \
    registerwindow.cpp

HEADERS += \
    addfriends.h \
    autocleartextedit.h \
    changeinformation.h \
    changepassword.h \
    choicedialog.h \
    cutavator.h \
    dialog.h \
    findpassword.h \
    friendmessage.h \
    login.h \
    logout.h \
    mainwindow-else.h \
    mainwindow.h \
    registerwindow.h

FORMS += \
    addfriends.ui \
    changeinformation.ui \
    changepassword.ui \
    choicedialog.ui \
    cutavator.ui \
    dialog.ui \
    findpassword.ui \
    friendmessage.ui \
    login.ui \
    logout.ui \
    mainwindow.ui \
    registerwindow.ui

TRANSLATIONS += \
    user_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../../resource/tubiao.qrc
