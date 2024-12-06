#ifndef REDIRECTION_H
#define REDIRECTION_H
void rd_stdout(char * command);
void rd_stdin(char * command);
void rd_pipes(char * command);
int is_redirect(char ** arg_ary)
#endif