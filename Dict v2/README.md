# CAdvanced Dictionary ver2

## Update
### Enhancing tabcomplete search by using strcmp
ex: `comput` and tab will result in `computing`

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
### 1. Extract btree library and find 'libbt.a' in bt-5.0.0/lib 
   Replace `libbt.a` with your `libbt.a` 

### 2. Create dictionary db from demo31.c
Note: This includes removing used databases
>> ./createdict.sh

### 3. Run dictionary
>> ./rundict.sh

