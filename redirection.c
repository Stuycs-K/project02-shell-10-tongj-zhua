#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
// arguments: char ** arg_ary with 2 char pointers to program name and file to use as stdout
// return: void
// redirects stdout of left to right
void rd_stdout(char ** arg_ary){ // front text goes into back
    int fdback = open(arg_ary[1], O_WRONLY);
    int backup_stdout = dup(STDOUT_FILENO);
    dup2(fdback, STDOUT_FILENO);
    arg_ary[1]=NULL;

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

// arguments: char ** arg_ary with 2 char pointers to program name and file to use as stdin
// return: void
// redirects stdin of left to right
void rd_stdin(char ** arg_ary){ // back text goes into front
    int fdback = open(arg_ary[1], O_RDONLY);
    int backup_stdin = dup(STDIN_FILENO);
    dup2(fdback, STDIN_FILENO);
    arg_ary[1]=NULL; 

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

// arguments: char ** arg_ary with 2 char pointers to program names for pipe 
// return: void
// redirects stdout of left to be stdin of right
void rd_pipes(char ** arg_ary){
    open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0611);
    char * args[3]; 
    args[0] = arg_ary[0]; 
    args[1] = "test.txt"; 
    args[2] = NULL; 
    rd_stdout(args); 
    args[0] = arg_ary[1]; 
    args[1] = "test.txt"; 
    args[2] = NULL; 
    rd_stdin(args); 
}

// // arguments: char ** arg_ary is parsed arguments
// // return: int
// // returns whether or not the command is redirection
// char is_redirect(char * val){
//     if ((val == '<') || (val == '>') || (val = '|')) return val;
//     return 0;
// }

void redirectme(char ** arg_ary){ 
    for (int i = 0; arg_ary[i];i++){ 
        char **args = (char **)malloc(4 * sizeof(char *)); 
        if (arg_ary[i+1][0] == '<'){
            
        }
    } 
}

int main(){
    char * arg_ary[3]; 
    arg_ary[0] = "ls"; 
    arg_ary[1] = "test.txt"; 
    arg_ary[2] = NULL;
    rd_stdout(arg_ary);
    arg_ary[0] = "cat"; 
    arg_ary[1] = "test.txt"; 
    arg_ary[2] = NULL;
    rd_stdin(arg_ary); 
    // redirectme(arg_ary);
}
