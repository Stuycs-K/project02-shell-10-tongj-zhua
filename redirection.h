#ifndef REDIRECTION_H
#define REDIRECTION_H
void rd_stdout(char * command);
void rd_stdin(char * command);
void rd_pipes(char * command);
void parse_args(char *buffer, char ** arg_ary);
int is_redirect(char * command);
#endif