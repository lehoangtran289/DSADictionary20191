# !bin/bash
gcc `pkg-config --cflags gtk+-3.0` -o dictionary maindemo.c `pkg-config --libs gtk+-3.0` -w libbt.a -rdynamic
./dictionary