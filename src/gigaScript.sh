gcc -Wall main.c `pkg-config fuse --cflags --libs` -o commiFS
./commiFS -f comiFolder mountComiFolder
# fusermount -uz comiFolder