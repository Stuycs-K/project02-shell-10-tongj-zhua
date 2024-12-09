#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>

// arguments: char * command is line with >
// return: void 
// redirects stdout to file in line
void rd_stdout(char * command){ // front text goes into back
    char front[256]; char back[256];
    sscanf(command, "%s > %s", front, back);
    int fdback = open(back, O_WRONLY);
    int backup_stdout = dup(STDOUT_FILENO);
    dup2(fdback, STDOUT_FILENO);
    char * arg_ary[2];
    arg_ary[0]=front;
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

// arguments: char * command is line with <
// return: void
// redirects stdin to file in line
void rd_stdin(char * command){ // back text goes into front
    char front[256]; char back[256];
    sscanf(command, "%s < %s", front, back);
    int fdback = open(back, O_RDONLY);
    int backup_stdin = dup(STDIN_FILENO);
    dup2(fdback, STDIN_FILENO);
    char * arg_ary[2];
    arg_ary[0]=front;
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

// arguments: char * command is line with |
// return: void
// redirects stdout of left to be stdin of right
void rd_pipes(char * command){
    char front[256]; char back[256]; 
    sscanf(command, "%[^|] | %[^0-9]", front, back);
    char prog1[256];
    char file1[256];
    int backup_stdin; 
    int val = 1; 
    for (int i = 0; front[i];i++){
        if (front[i] == '<'){
            val = 0; 
            sscanf(front, "%[^<] < %s", prog1, file1);
            int fdback = open(file1, O_RDONLY);
            backup_stdin = dup(STDIN_FILENO);
            dup2(fdback, STDIN_FILENO);
        }
    }
    if (val) strcpy(prog1, front); 
    open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0611);
    strcat(prog1, " > temp.txt");
    rd_stdout(prog1); 
    dup2(backup_stdin, STDIN_FILENO); 
    char prog2[256];
    char file2[256];
    int backup_stdout; 
    val = 1; 
    for (int i = 0; back[i];i++){
        if (back[i] == '>'){
            val = 0; 
            sscanf(back, "%[^>] > %s", prog2, file2); 
            int fdback = open(file2, O_RDONLY);
            backup_stdout = dup(STDOUT_FILENO);
            dup2(fdback, STDOUT_FILENO);
        }
    }
    if (val) strcpy(prog2, back); 
    strcat(prog2, " < temp.txt");
    rd_stdin(prog2); 
    dup2(backup_stdout, STDOUT_FILENO); 

//removing temp
    char * arg_ary[3]; 
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
}

int main(){
    char * command = "cowsay < test.txt | cat > out.txt"; 
    char * command4 = "ls | cat > out.txt";
    char * command2 = "cowsay < test.txt | cat";
    char * command3 = "ls | cat";
    rd_pipes(command); 
}
