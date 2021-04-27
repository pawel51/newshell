#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "shell.h"

#define LINE_BUFSIZE 64
#define SPACE_DELIM " \t\r\n\a"
#define PIPE_DELIM "|\n"
#define RED_DELIM ">>"
#define AMP_DELIM "&"



/**
   @brief Takes a command string and devides it on chunks with delim 
   @param cmd string which has to be devided
   @param delim separators
   @return **atoms - list of pointers to each atom  
 */
char **split_cmd(char *cmd, char *delim)
{
    int bufsize = LINE_BUFSIZE, position = 0;
    char **atoms = malloc(bufsize * sizeof(char *));
    char *atom, **atoms_copy;

    if (!atoms)
    {
        fprintf(stderr, "Memory allocation error");
        exit(EXIT_FAILURE);
    }

    atom = strtok(cmd, delim);
    while (atom != NULL)
    {
        atoms[position] = atom;
        position++;

        if (position >= bufsize)
        {
            bufsize += LINE_BUFSIZE;
            atoms_copy = atoms;
            atoms = realloc(atoms, bufsize * sizeof(char *));
            if (!atoms)
            {
                free(atoms_copy);
                fprintf(stderr, "Memory allocation error");
                exit(EXIT_FAILURE);
            }
        }

        atom = strtok(NULL, delim);
    } // endwhile
    atoms[position] = NULL;

    return atoms;
}


/**
   @brief Split a line into atoms (uses strtok(char* str, char* delimeter)).
   @param line The line.
   @return Null-terminated array of atoms.
 */
void split_line(char *line)
{
    int bufsize = LINE_BUFSIZE, position = 0;
    char **cmds = malloc(bufsize * sizeof(char *));
    char *cmd, *amp, *last_cmd, **cmds_buckup;
    int amp_ends = 1, redirect = 1;
    int first_pipe_cmd = 1, last_pipe_cmd = 0, input = 0;
    update_history(line);
    if (!cmds)
    {
        fprintf(stderr, "Memory allocation error");
        exit(EXIT_FAILURE);
    }

    cmd = strtok(line, PIPE_DELIM);
    while (cmd != NULL)
    {

        position++;

        if (position >= bufsize)
        {
            bufsize += LINE_BUFSIZE;
            cmds_buckup = cmds;
            cmds = realloc(cmds, bufsize * sizeof(char *));
            if (!cmds)
            {
                free(cmds_buckup);
                fprintf(stderr, "Memory allocation error");
                exit(EXIT_FAILURE);
            }
        }
        last_cmd = cmd;
        cmd = strtok(NULL, PIPE_DELIM);
        if (!cmd)
        {
            last_pipe_cmd = 1;
        }
        input = spawn_proc(last_cmd, input, first_pipe_cmd, last_pipe_cmd);
        first_pipe_cmd = 0;

    } 
}