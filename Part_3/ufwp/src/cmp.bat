g++ -c UFWP_12_me.cpp -I ..\include
ar rcs ufwp.lib *.o
del ..\lib\ufwp.lib
move ufwp.lib ..\lib