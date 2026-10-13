QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boom.cpp \
    bulletbase.cpp \
    editor.cpp \
    editorbullet.cpp \
    enemy1.cpp \
    enemy2.cpp \
    enemy3.cpp \
    enemybullet.cpp \
    enemyplanebase.cpp \
    gamedialog.cpp \
    herobullet.cpp \
    heroplane.cpp \
    icon.cpp \
    main.cpp \
    mainwindow.cpp \
    userexchange.cpp

HEADERS += \
    boom.h \
    bulletbase.h \
    editor.h \
    editorbullet.h \
    enemy1.h \
    enemy2.h \
    enemy3.h \
    enemybullet.h \
    enemyplanebase.h \
    gamedialog.h \
    herobullet.h \
    heroplane.h \
    icon.h \
    mainwindow.h \
    userexchange.h

FORMS += \
    editor.ui \
    editorbullet.ui \
    gamedialog.ui \
    mainwindow.ui \
    userexchange.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    Game_1.pro

RESOURCES += \
    res.qrc

DISTFILES += \
    01.png \
    02.png \
    03.png \
    1.png \
    11.png \
    12.png \
    13.png \
    14.png \
    15.png \
    2.png \
    3.png \
    4.png \
    Game1_1.pro.user \
    Game_1.pro.user \
    Game_alpha.pro.user \
    Game_alpha01.pro.user \
    Game_alpha02.pro.user \
    Game_alpha03.pro.user \
    Game_bata.pro.user \
    Klight_0.png \
    Klight_1.png \
    Klight_10.png \
    Klight_11.png \
    Klight_12.png \
    Klight_13.png \
    Klight_14.png \
    Klight_15.png \
    Klight_16.png \
    Klight_17.png \
    Klight_18.png \
    Klight_19.png \
    Klight_2.png \
    Klight_20.png \
    Klight_21.png \
    Klight_22.png \
    Klight_23.png \
    Klight_24.png \
    Klight_25.png \
    Klight_26.png \
    Klight_27.png \
    Klight_28.png \
    Klight_29.png \
    Klight_3.png \
    Klight_30.png \
    Klight_31.png \
    Klight_4.png \
    Klight_5.png \
    Klight_6.png \
    Klight_7.png \
    Klight_8.png \
    Klight_9.png \
    bomb \
    bomb0.png \
    bomb1.png \
    bomb2.png \
    bomb3.png \
    bomb4.png \
    bomb5.png \
    bomb6.png \
    enemyBullet01.png \
    enemyBullet02.png \
    enemyBullet03.png \
    enemyBullet04.png \
    enemyBullet05.png \
    enemyBullet06.png \
    enemyBullet21.png \
    heroBullet01.png \
    heroBullet02.png \
    heroBullet03.png \
    heroBullet04.png \
    heroBullet05.png \
    heroBullet06.png \
    heroBullet07.png \
    heroBullet12.png \
    heroBullet21.png \
    mainscene.png
