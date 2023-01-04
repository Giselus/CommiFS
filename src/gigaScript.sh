gcc -Wall main.c `pkg-config fuse --cflags --libs` -o commiFS
./commiFS -f A BBB
# fusermount -uz comiFolder