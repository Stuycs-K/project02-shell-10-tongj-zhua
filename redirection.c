#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
// arguments: char * command is line with >
// return: void
// redirects stdout of left to right
void rd_stdout(char * command){ // front text goes into back
    char * front;
    char * back = command;
    front = strsep(&back, ">");
    int fdback = open(back, O_WRONLY);
    int backup_stdout = dup(STDOUT_FILENO);
    char * arg_ary[2];
    arg_ary[0]=front;
    arg_ary[1]=NULL;
    dup2(fdback, STDOUT_FILENO);
    execvp(arg_ary[0], arg_ary);
    dup2(backup_stdout, STDOUT_FILENO);
}

// arguments: char * command is line with <
// return: void
// redirects stdout of left with stdin of right
void rd_stdin(char * command){ // back text goes into front
    char * front;
    char * back = command;
    front = strsep(&back, "<");
    int fdback = open(back, O_WRONLY);
    int backup_stdin = dup(STDIN_FILENO);
    char * arg_ary[2];
    arg_ary[0]=front;
    arg_ary[1]=NULL;
    dup2(fdback, STDIN_FILENO);
    execvp(arg_ary[0], arg_ary);
    dup2(backup_stdin, STDIN_FILENO);
}


int main(){
    char  test[256]= "ls>c.txt";
    rd_stdout(test);
}