jtorrent
========
Introduction
---------------------
Jtorrent a a P2P download software like BitTorrent, uTorrent.
This is a final project for course Computer Network in [UMJI][umji_web]
[umji_web]:http://umji.sjtu.edu.cn/


A note on compatibility
-------------------------
This program is developed under Ubuntu Linux.
It is stable under: Ubuntu 12.04
both 32-bit and 64-bit are available.

How to compile and run
-----------------------------------

###This is a qt project

This project is developed by [Qt Creator][qt_org] IDE
[qt_org]:http://qt-project.org/
If you have this IDE, you can directly import the project and run it.
When you load the project, you may have problem like `Path or permissions wrong?`
In order to avoid this problem, you can delete the `.pro.user` file and re-open the project, rebuild.

###Run in the terminal 
In order to run in the terminal and get input, you need to modify the configuration of the qt creator.(Because Qt creator does not support the terminal console in Ubuntu)
	open the qt creator
	change System/Terminal: /usr/bin/xterm -e)
	in run setting, set it to the "run in terminal" option,
	it will use xterm to open

How to use
---------------------------------
##server
Run the project

##client
First you need to input user name and port, and then just follow the menu(input should be correct, no error checking here)

Design
---------------------------------
There is a document in the `design` folder containing the communication protocal. You can check it in details.

Algorithm
-----------------------
There is a document in the `Algorithm` folder . You can check it in details.

Libraries used
--------------------------------------------
* `jsoncpp` (32bit and 64 bit) 
if you are under the 32-bit, you should copy libjsoncpp.so
if you are under the 64-bit, you should copy libjson_linux-gcc-4.6_libmt.so
(copy the corresponding .so file to the directory /usr/local/lib/)
	NOTICE: in order to let qt to load, you should change .pro file, unix:!macx: LIBS += -L$$PWD/../lib/ -ljsoncpp,
	change to specific name,  example , if you rename to libjson.cpp then should change -ljsoncpp to -ljson
*SHA-1
