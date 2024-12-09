[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/Tfg6waJb)
# Systems Project2 - MyShell
 (Janet Tong & Ashley Zhu)
 Team Name: Zhu You Know Whats Tong??? 

#### Features Our Shell Implements: 
general functionality, working directory, prompt, exiting, simple redirection and pipes

#### Unsuccessful Features Attempted: 
handling multiple pipes and redirects 

#### Function Headers:
###### In cwd.c:
void cd(char * path)

###### In func.c:
void prompt(char * shortprompt)
void execcommand(char * buffer)
void func()
int main()

###### In redirection.c:
void parse_args(char *buffer, char ** arg_ary)
void rd_stdout(char * command)
void rd_stdin(char * command)
void rd_pipes(char * command)
int is_redirect(char * command)