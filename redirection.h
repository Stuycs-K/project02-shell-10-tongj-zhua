#ifndef REDIRECTION_H
#define REDIRECTION_H
int rd_stdout(char * file);
int rd_stdin(char * file);
int rd_pipes(char * command);
void redirectme(char ** arg_ary);
#endif