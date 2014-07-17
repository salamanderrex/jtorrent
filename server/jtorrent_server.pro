SOURCES += \
    server_multi.cpp \
    ../include/C_R.cpp

HEADERS += \
    ../include/C_R.h \
    ../include/Torrent.h \
    ../include/Peer_list.h \
    ../include/C_BASE.h \
    server_console.h



unix:!macx:!symbian: LIBS += -L$$PWD/../lib/ -ljsoncpp



INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
