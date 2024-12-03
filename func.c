#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "cwd.h"

//arguments: char *buffer is line reading in, char ** arg_ary holds arguments of line 
//returns void 
//parses arguments of command line input
void parse_args(char *buffer, char ** arg_ary){
    char *curr = buffer; 
    int i = 0; 
    while (curr){
        arg_ary[i] = strsep(&curr, " ");
        i++; 
    }
    int size = strlen(arg_ary[i-1]);
    arg_ary[i-1][size-1] = '\0';
    arg_ary[i] = NULL; 
}

//arguments: char *shortprompt is a pointer to an empty char array
//returns void 
//shortens path by using ~ for home directory 
void prompt(char * shortprompt){
    char prompt[256]; 
    getcwd(prompt, 256); 
    char *homedir = getenv("HOME"); 
    shortprompt[0] = '~';
    for (int i = 1; prompt[i + strlen(homedir) - 1]; i++){
        shortprompt[i] = prompt[i + strlen(homedir) - 1];
    }
}

//no arguments, returns void 
//general functionality: prompts user, forks and executes command
void func(){
    while (1){
        char shortprompt[256]; char buffer[1000]; char * arg_ary[10]; 
        prompt(shortprompt);
        printf("%s $", shortprompt); 
        if (!fgets(buffer, 1000, stdin)) exit(0);
        parse_args(buffer, arg_ary); 
        pid_t p1 = fork(); 
        if (p1 < 0){
            perror("forkfail"); 
            exit(1); 
        } else if (p1 == 0){
            if (!strcmp(arg_ary[0], "cd")){
                cd(arg_ary[1]);
            } 
            else execvp(arg_ary[0], arg_ary);
            exit(0);
        } else{
            if (!strcmp(arg_ary[0], "exit")){
                exit(2);
            }
            int status; 
            wait(&status); 
        }
        fflush(stdout); 
    }
}

int main(){
    func();
}