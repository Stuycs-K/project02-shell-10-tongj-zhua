#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void parse_args(char *buffer, char ** arg_ary){
    char *curr = buffer; 
    int i = 0; 
    while (curr){
        arg_ary[i] = strsep(&curr, " ");
        i++; 
    }
    arg_ary[i] = NULL; 
}

void func(){
    while (1){
        printf("Input: "); 
        char buffer[1000]; 
        fgets(buffer, 1000, stdin); 
        char * arg_ary[10]; 
        parse_args(buffer, arg_ary); 
        pid_t p1 = fork(); 
        if (p1 < 0){
            perror("forkfail"); 
            exit(1); 
        } else if (p1 == 0){
            execvp(arg_ary[0], arg_ary);
            exit(0);
        } 
        int status; 
        wait(&status); 
    }
}

int main(){
    func(); 
}