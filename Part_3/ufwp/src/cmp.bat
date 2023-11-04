g++ -c UFWP_7_me.cpp -I ..\include
ar rcs ufwp.lib *.o
del ..\lib\ufwp.lib
move ufwp.lib ..\lib