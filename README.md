jtorrent
========

This is a final project for course
stable under: Ubuntu 12.04 32-bit or 64-bit

How to compile and run
-----------------------------------

### this is a project base on qt

you may have problem like "Path or permissions wrong?"
In order to run this , you can delete the .pro.user file and re-open the project, rebuild.

##run in the terminal 
in order to run in the terminal and get input,open the qt creator
<pre>
change System/Terminal: /usr/bin/xterm -e)
then in run setting, set it to the "run in terminal" option
</pre>
it will use xterm to open

how to use
---------------------------------
* ###server
just run

* ###client
input user name and port, and then follow the panel(input should be correct)


Design
---------------------------------
in the design floder



library
--------------------------------------------
* jsoncpp(32bit and 64 bit) 

if you are under the 32-bit, you should copy libjsoncpp.so
if you are under the 64-bit, you should copy libjson_linux-gcc-4.6_libmt.so
(copy the corresponding .so file to the directory /usr/local/lib/)
<pre>
NOTICE: in order to let qt to load, you should change .pro file, unix:!macx: LIBS += -L$$PWD/../lib/ -ljsoncpp,
 change to specific name,  example , if you rename to libjson.cpp then should change -ljsoncpp to -ljson
</pre>

* SHA-1
