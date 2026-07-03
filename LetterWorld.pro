QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/book.cpp \
    src/box.cpp \
    src/door.cpp \
    src/gameobject.cpp \
    src/gold.cpp \
    src/hardwood.cpp \
    src/jar.cpp \
    src/lake.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/npc.cpp \
    src/player.cpp \
    src/rock.cpp \
    src/stone.cpp \
    src/tpdoor.cpp \
    src/tree.cpp \
    src/worm.cpp \
    src/zombie.cpp

HEADERS += \
    src/book.h \
    src/box.h \
    src/door.h \
    src/gameobject.h \
    src/gold.h \
    src/hardwood.h \
    src/jar.h \
    src/lake.h \
    src/mainwindow.h \
    src/npc.h \
    src/player.h \
    src/rock.h \
    src/stone.h \
    src/tpdoor.h \
    src/tree.h \
    src/worm.h \
    src/zombie.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
