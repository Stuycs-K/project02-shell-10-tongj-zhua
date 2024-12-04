#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
// arguments: char * command is line with <
// return: void
// redirects stdin of left to right
void rd_stdin(char * command){ // back text goes into front
    char * front;
    char * back = command;
    front = strsep(&back, "<");
    int fdback = open(back, O_WRONLY);
    int backup_stdin = dup(STDIN_FILENO);
    int stdin = STDIN_FILENO;
    char * arg_ary[2];
    arg_ary[0]=front;
    arg_ary[1]=NULL;
    dup2(STDIN_FILENO,fdback);
    execvp(arg_ary[0], arg_ary);
    dup2(STDIN_FILENO,backup_stdin);
}
int main(){
    char  test[256]= "c.txt<ls";
    rd_stdin(test);
}