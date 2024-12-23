#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "cwd.h"
#include "redirection.h"


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

//arguments: char * buffer is the command
//returns void 
//executes the command (redirect, cd, general func) 
void execcommand(char * buffer){
    char * arg_ary[10];
    int a = is_redirect(buffer);
        if (!a){
            parse_args(buffer, arg_ary); 
            if (!strcmp(arg_ary[0], "exit")) exit(0);
            else if (!strcmp(arg_ary[0], "cd")){
                cd(arg_ary[1]); 
            }
            else{
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
            }
        }
}

//no arguments, returns void 
//general functionality: prompts user, forks and executes command
void func(){
    while (1){
        char * shortprompt=malloc(256);
        char wholebuffer[1000]; 
        prompt(shortprompt);
        printf("%s $", shortprompt); 
        free(shortprompt);
        fflush(stdout);
        if (!fgets(wholebuffer, 1000, stdin)) exit(0);
        if(strchr(wholebuffer, ';')!=NULL){
            char * curr = wholebuffer;
            char * buffer;
            while(strchr(curr, ';')!=NULL){
                buffer = strsep(&curr, ";");
                execcommand(buffer);
            }
            buffer = curr;
            execcommand(curr);
        }
        else{
            char * buffer=wholebuffer;
            execcommand(buffer);
        }
        }
}

int main(){
    func();
}