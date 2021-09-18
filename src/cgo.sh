g++ -o imp_vdf.o -c imp_vdf.cpp
g++ -o c_interface.o -c c_interface.c
ar r libcgo.so c_interface.o imp_vdf.o
ar r libzcnt.so lzcnt.o