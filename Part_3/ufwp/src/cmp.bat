g++ -c UFWP_8_me.cpp -I ..\include
ar rcs ufwp.lib *.o
del ..\lib\ufwp.lib
move ufwp.lib ..\lib