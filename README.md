# CommiFS

# Editing this README

When you're ready to make this README your own, just edit this file and use the handy template below (or feel free to structure it however you want - this is just a starting point!). Thank you to [makeareadme.com](https://www.makeareadme.com/) for this template.



## Installation
Requirements:

libfuse v2.9.9
Make sure that your system has this version of libfuse installed

You can compile your program by going into src folder and running:

```
gcc -Wall main.c `pkg-config fuse --cflags --libs` -o commiFS
```

If the compilation was successful you can run mount file system by running:

```
./commiFS -f {PATH_TO_FOLDER_TO_MOUNT}
```