#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
#include <sys/wait.h>

//arguments: char *buffer is line reading in, char ** arg_ary holds arguments of line 
//return: void
//parses arguments of command line input
void parse_args(char *buffer, char ** arg_ary){
    int size = strlen(buffer); 
    if (buffer[size-1] == '\n') buffer[size-1] = '\0';
    char *curr = buffer; 
    int i = 0; 
    while (curr){
        arg_ary[i] = strsep(&curr, " ");
        i++; 
    }
    arg_ary[i] = NULL;
}

// arguments: char * command is line with >
// return: void 
// redirects stdout to file in line
void rd_stdout(char * command){ 
    char front[256]; char back[256];
    sscanf(command, "%[^>]> %s", front, back);
    int size = strlen(front); 
    front[size -1] = '\0';

    char * arg_ary[10];
    parse_args(front, arg_ary);

    int fdback = open(back, O_WRONLY, 0);
    int backup_stdout = dup(STDOUT_FILENO);
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
// redirects stdin to file in line
void rd_stdin(char * command){ // back text goes into front
    char front[256]; char back[256];
    sscanf(command, "%[^<]< %s", front, back);
    int size = strlen(front); 
    front[size -1] = '\0';

    char * arg_ary[10];
    parse_args(front, arg_ary);

    int fdback = open(back, O_RDONLY);
    int backup_stdin = dup(STDIN_FILENO);
    dup2(fdback, STDIN_FILENO);

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
    sscanf(command, "%[^|]| %[^0-9]", front, back);
    int size = strlen(front); 
    front[size -1] = '\0';
    char prog1[256]; char file1[256]; int backup_stdin; int val = 1; 
    for (int i = 0; front[i];i++){
        if (front[i] == '<'){
            val = 0; 
            sscanf(front, "%[^<]< %s", prog1, file1);
            int size = strlen(prog1); 
            prog1[size -1] = '\0';
            int fdback = open(file1, O_RDONLY);
            backup_stdin = dup(STDIN_FILENO);
            dup2(fdback, STDIN_FILENO);
        }
    }
    if (val) strcpy(prog1, front); 
    open("temp.txt", O_RDWR | O_CREAT | O_TRUNC, 0611);
    strcat(prog1, " > temp.txt");
    rd_stdout(prog1); 
    dup2(backup_stdin, STDIN_FILENO); 
    char prog2[256]; char file2[256]; int backup_stdout; val = 1; 
    for (int i = 0; back[i];i++){
        if (back[i] == '>'){
            val = 0; 
            sscanf(back, "%[^>]> %s", prog2, file2);
            int size = strlen(prog2); 
            prog2[size -1] = '\0';
            int fdback = open(file2, O_WRONLY);
            backup_stdout = dup(STDOUT_FILENO);
            dup2(fdback, STDOUT_FILENO);
        }
    }
    if (val) strcpy(prog2, back); 
    strcat(prog2, " < temp.txt");
    rd_stdin(prog2); 
    dup2(backup_stdout, STDOUT_FILENO); 

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

// arguments: char * command is that redirects
// return: int 1 or 0 indicating whether the command has >, <, or | 
// calls redirection if in command
int is_redirect(char * command){
    int val = 0; 
    for (int i = 0; command[i]; i++){
        if (command[i] == '|'){
            rd_pipes(command); 
            val = 1; 
        }
    }
    if (!val){
        for (int i = 0; command[i]; i++){
            if (command[i] == '<'){
                rd_stdin(command);
                val = 1; 
            }
            if (command[i] == '>'){
                rd_stdout(command);
                val = 1; 
            }
        }
    }
    return val;
}


//UNSUCCESSFUL ATTEMPT: 
// int is_redirect(char * command){
//     for (int i = 0; command[i]; i++){
//         if (command[i] == '>'){
//             return
//         }
//     }
// }


// // arguments: char * command is the name of the program to left of pipe
// // return: int
// // redirects stdout of command to temp file and redirects stdin to temp file
// int rd_pipes(char * command){
//     open("temp.txt", O_WRONLY | O_CREAT | O_TRUNC, 0611);
//     char * file = "temp.txt";
//     int backup_stdout = rd_stdout(file);
//     char * arg_ary[3];
//     arg_ary[0] = command;
//     arg_ary[1] = NULL;
//     pid_t p1 = fork();
//     if (p1 < 0){
//         perror("forkfail");
//         exit(1);
//     } else if (p1 == 0){
//         execvp(arg_ary[0], arg_ary);
//     } else{
//         int status;
//         wait(&status);
//     }
//     dup2(backup_stdout, STDOUT_FILENO);
//     int backup_stdin = rd_stdin(file);
//     arg_ary[0] = "rm";
//     arg_ary[1] = "temp.txt";
//     arg_ary[2] = NULL;
//     pid_t p2 = fork();
//     if (p2 < 0){
//         perror("forkfail");
//         exit(1);
//     } else if (p2 == 0){
//         execvp(arg_ary[0], arg_ary);
//     } else{
//         int status;
//         wait(&status);
//     }
//     return backup_stdin; 
// }


// int redirectme(char ** arg_ary){
//     int backup_stdin; int backup_stdout;
//     int val = 0;
//     for (int i = 1; arg_ary[i];i+=2){
//         if (arg_ary[i][0] == '<'){
//             val = 1;
//             backup_stdin = rd_stdin(arg_ary[i+1]);
//             arg_ary[i+1] = arg_ary[i-1];
//         }
//         if (arg_ary[i][0] == '>'){
//             val = 1;
//             backup_stdout = rd_stdout(arg_ary[i+1]);
//             arg_ary[i+1] = arg_ary[i-1];
//         }
//         if (arg_ary[i][0] == '|'){
//             val = 1;
//             backup_stdin = rd_pipes(arg_ary[i-1]);
//         }
//         if (val && !(arg_ary[i+2])){
//             char **args = (char **)malloc(2 * sizeof(char *));
//             args[0] = arg_ary[i+1];
//             args[1] = NULL;
//             pid_t p1 = fork();
//             if (p1 < 0){
//                 perror("forkfail");
//                 exit(1);
//             } else if (p1 == 0){
//                 execvp(args[0], args);
//             } else{
//                 int status;
//                 wait(&status);
//             }
//             free(args);
//         }
//     }
//     dup2(backup_stdin, STDIN_FILENO);
//     dup2(backup_stdout, STDOUT_FILENO);
//     fflush(stdout);
//     return val;
// }





