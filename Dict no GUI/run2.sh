tar xvzf bt-5.0.0.tar.gz
cd bt-5.0.0
make clean
make
cd ..
gcc -o demo.out autocomplete.c -Lbt-5.0.0/lib -lbt -Ibt-5.0.0/inc -lreadline
./demo.out
