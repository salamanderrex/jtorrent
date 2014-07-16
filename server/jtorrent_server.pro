SOURCES += \
    server_multi.cpp \
    ../include/C_R.cpp

HEADERS += \
    ../include/C_R.h \
    ../include/Torrent.h \
    ../include/Peer_list.h \
    ../include/C_BASE.h

unix:!macx:!symbian: LIBS += -L$$PWD/../lib/ -ljsonlib

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
