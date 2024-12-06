#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
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

    pid_t p1 = fork(); 
    if (p1 < 0){
        perror("forkfail"); 
        exit(1); 
    } else if (p1 == 0){
        execvp(arg_ary[0], arg_ary);
    } else{
        int status; 
        wait(&status); 
    }
    dup2(backup_stdout, STDOUT_FILENO);
}

// arguments: char * command is line with <
// return: void
// redirects stdin of left to right
void rd_stdin(char * command){ // back text goes into front
    char * front;
    char * back = command;
    front = strsep(&back, "<");
    int fdback = open(back, O_RDONLY);
    int backup_stdin = dup(STDIN_FILENO);
    char * arg_ary[2];
    arg_ary[0]=front;
    arg_ary[1]=NULL;
    dup2(fdback, STDIN_FILENO);
    execvp(arg_ary[0], arg_ary);
    pid_t p1 = fork(); 
    if (p1 < 0){
        perror("forkfail"); 
        exit(1); 
    } else if (p1 == 0){
        execvp(arg_ary[0], arg_ary);
    } else{
        int status; 
        wait(&status); 
    }
    dup2(backup_stdin, STDIN_FILENO);
}

void rd_pipes(char * command){
    char * front;
    char * back = command;
    front = strsep(&back, "|");
    open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0611);
    char in[256]; 
    strcpy(in, front);
    strcat(in, ">"); 
    strcat(in, "test.txt");
    rd_stdout(in); 

    char out[256]; 
    strcpy(out, front); 
    strcat(out, "<"); 
    strcat(out, "test.txt"); 
    rd_stdin(out); 
}

int main(){
    char str[256] = "ls|cat";
    rd_pipes(str);
}
