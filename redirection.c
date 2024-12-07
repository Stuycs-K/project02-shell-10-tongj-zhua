#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
// arguments: char * file is the file redirected to stdout 
// return: int 
// redirects stdout to file 
int rd_stdout(char * file){ // front text goes into back
    int fdback = open(file, O_WRONLY);
    int backup_stdout = dup(STDOUT_FILENO);
    dup2(fdback, STDOUT_FILENO);
    return backup_stdout; 
}

// arguments: char * file is the file redirected to stdin 
// return: int
// redirects stdin to file 
int rd_stdin(char * file){ // back text goes into front
    int fdback = open(file, O_RDONLY);
    int backup_stdin = dup(STDIN_FILENO);
    dup2(fdback, STDIN_FILENO);
    return backup_stdin; 
}

// arguments: char * command is the name of the program to left of pipe
// return: int
// redirects stdout of command to temp file and redirects stdin to temp file
int rd_pipes(char * command){
    open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0611);
    char * file = "temp.txt"; 
    int backup_stdout = rd_stdout(file); 
    char * arg_ary[3]; 
    arg_ary[0] = command; 
    arg_ary[1] = NULL; 
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
    int backup_stdin = rd_stdin(file); 
    arg_ary[0] = "rm"; 
    arg_ary[1] = "temp.txt"; 
    arg_ary[2] = NULL; 
    pid_t p2 = fork(); 
    if (p2 < 0){
        perror("forkfail"); 
        exit(1); 
    } else if (p2 == 0){
        execvp(arg_ary[0], arg_ary);
    } else{
        int status; 
        wait(&status); 
    }
    return backup_stdin;  
}

void redirectme(char ** arg_ary){ 
    int backup_stdin; int backup_stdout; 
    for (int i = 1; arg_ary[i];i+=2){ 
        if (arg_ary[i][0] == '<'){ 
            backup_stdin = rd_stdin(arg_ary[i+1]); 
            arg_ary[i+1] = arg_ary[i-1]; 
        }
        if (arg_ary[i][0] == '>'){
            backup_stdout = rd_stdout(arg_ary[i+1]); 
            arg_ary[i+1] = arg_ary[i-1]; 
        }
        if (arg_ary[i][0] == '|'){
            backup_stdin = rd_pipes(arg_ary[i-1]); 
        }
        if (!(arg_ary[i+2])){
            char **args = (char **)malloc(2 * sizeof(char *)); 
            args[0] = arg_ary[i+1]; 
            args[1] = NULL;
            pid_t p1 = fork(); 
            if (p1 < 0){
                perror("forkfail"); 
                exit(1); 
            } else if (p1 == 0){
                execvp(args[0], args);
            } else{
                int status; 
                wait(&status); 
            }
            free(args); 
        }
    } 
    dup2(backup_stdin, STDIN_FILENO); 
    dup2(backup_stdout, STDOUT_FILENO); 
}

int main(){
    char * arg_ary[4]; 
    // arg_ary[0] = "cowsay"; 
    // arg_ary[1] = "<"; 
    // arg_ary[2] = "test.txt"; 
    // arg_ary[3] = "|"; 
    arg_ary[0] = "ls"; 
    arg_ary[1] = ">"; 
    arg_ary[2] = "out.txt"; 
    arg_ary[3] = NULL;
    redirectme(arg_ary);
}
