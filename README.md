# CAdvanced Dictionary ver1

(bugs and segmentation faults are inevitable :D )

## Installation
If gtk3+ stuff werent installed 
>> sudo apt-get install libgtk3.0-dev

>> sudo apt-get install gnome-core-devel 

>> sudo apt-get install pkg-config

>> sudo apt-get install devhelp

>> sudo apt-get install libglib2.0-doc libgtk2.0-doc

>> sudo apt-get install glade libglade2-dev 

>> sudo apt-get install glade


## So how to compile?
## 1. Extract btree library and find 'libbt.a' in bt-5.0.0/lib 
   Replace 'libbt.a' with your 'libbt.a' 

## 2. Create dictionary db from demo3.c
>> gcc demo3.c libbt.a -w

## 3. Compile maindemo.c 
>> gcc `pkg-config --cflags gtk+-3.0` -o dictionary maindemo.c `pkg-config --libs gtk+-3.0` -w libbt.a -rdynamic

## 4. Run dictionary
>> ./dictionary

OR you could simply run ./rundict.sh
