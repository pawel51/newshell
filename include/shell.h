#ifndef SHELL_H
#define SHELL_H

//Takes a command string and devides it on chunks with delim 
char **split_cmd(char *cmd, char *delim);

// Split a line into atoms (uses strtok(char* str, char* delimeter)).
void split_line(char *line);

// Creates a new child process, handles pipes, redirections and suspension
int spawn_proc(char *cmd, int input, int first, int last);

// Updates the history 
void update_history(char* path);

// Reads a line from stdin or from script, which has to begin with "#!{SHELL_PATH}"
char **read_line(void);


#endif