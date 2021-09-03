mkdir build
clang -shared -undefined dynamic_lookup -o build/clientlist.so src/clientlist.c
