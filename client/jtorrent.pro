HEADERS += sha1.h \
    send.h \
    ../lib/C_R.h \
    client_console.h \
    ../include/C_R.h \
    ../include/Torrent.h \
    ../include/Peer_list.h
SOURCES += sha1.cpp \
    send.cpp \
    multi_client.cpp \
    ../include/C_R.cpp

#unix:!macx:!symbian: LIBS += -L$$PWD/../lib/ -ljsonlib

#INCLUDEPATH += $$PWD/../include
#DEPENDPATH += $$PWD/../include

unix:!macx: LIBS += -L$$PWD/../lib/ -ljsoncpp

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
