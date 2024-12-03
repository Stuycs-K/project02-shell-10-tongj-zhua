#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int cd(char * path){ // changes directory to path (relative to current)
    char * newcwd=malloc(256);
    getcwd(newcwd, 256);
    strcat(newcwd, "/");
    strcat(newcwd, path);
    chdir(newcwd);
}