SOURCES += \
    server_multi.cpp \
    ../include/C_R.cpp

<<<<<<< HEAD

=======
>>>>>>> 4b61685cfdd2e51c5bd421b51a91111d880b8f56
HEADERS += \
    ../include/C_R.h \
    ../include/Torrent.h \
    ../include/Peer_list.h \
<<<<<<< HEAD
    ../include/C_BASE.h \
    server_console.h



unix:!macx:!symbian: LIBS += -L$$PWD/../lib/ -ljsoncpp
=======
    ../include/C_BASE.h

unix:!macx:!symbian: LIBS += -L$$PWD/../lib/ -ljsonlib
>>>>>>> 4b61685cfdd2e51c5bd421b51a91111d880b8f56

INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../include
