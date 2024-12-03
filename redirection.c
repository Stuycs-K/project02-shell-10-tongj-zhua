#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// arguments: char * command is line with <
// return: void
// redirects stdin of left to right
void rd_stdin(char * command){
    char * front;
    char * back = command;
    front = strsep(&back, '<');
    int fdfront = open(front, O_WRONLY);
    int fdback = open(back, O_WRONLY);
    int backup_stdout = dup(stdout);
    dup2(fdback, stdout);
    fflush(stdout);
    dup(backup_stdout, stdout);
}