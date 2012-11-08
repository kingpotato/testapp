testapp
=======

This repository includes some useful test program.
It can be simply compiled by gcc or arm-none-linux-gnueabi-gcc command without any CFLAG or LIB.

Compile
=======

gcc file_name -o exe_name

ex) gcc settimer.c -o settimer -lrt

CrossCompile
============

arm-none-linux-gnueabi-gcc file_name -o exe_name

ex) arm-none-linux-gnueabi-gcc settimer.c -o settimer -lrt
