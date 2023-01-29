gcc -Wall src/main.c `pkg-config fuse --cflags --libs` -o comiFS
./comiFS -f comiFolder mountComiFolder
fusermount -uz mountComiFolder
