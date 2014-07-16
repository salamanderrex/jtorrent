HEADERS += sha1.h \
    send.h \
    ../lib/C_R.h \
    client_console.h
SOURCES += sha1.cpp \
    send.cpp \
    multi_client.cpp

unix:!macx:!symbian: LIBS += -L$$PWD/../lib/ -ljsoncpp

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
