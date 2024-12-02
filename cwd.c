#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cd(char * path){ // changes directory to path
    char * newcwd=malloc(256);
    getcwd(newcwd; 256);
    strcat(newcwd, path);
    chdir(newcwd);
}