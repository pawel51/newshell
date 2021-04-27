#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>


#include "shell.h"

#define SCR_DELIM "#!"
#define SHELL_LOCATION "/home/pawel/Desktop/newshell/main.c"

/**
   @brief Reads a line from stdin or from script, which has to begin with "#!{SHELL_PATH}"
   @return cmd_array - array containing all command lines 
 */
char **read_line(void)
{
    int i = 0;
    char *line=NULL, *buffer, **loc_read, **cmd_array;
    ssize_t bufsize = 0, cmd_count = 20;
    if (getline(&line, &bufsize, stdin) == -1)
        perror("Cant read the line");
    if(line[strlen(line)-1] == '\n')
        line[strlen(line)-1] = '\0';
    FILE *fptr = fopen(line, "r");
    if (!fptr)
    {
        if(line[strlen(line)] == '\0')
            line[strlen(line)] = '\n';
        cmd_array = (char **)calloc(sizeof(char *), cmd_count);
        if (!cmd_array)
        {
        fprintf(stderr, "Memory allocation error");
        exit(EXIT_FAILURE);
        }
        cmd_array[i] = (char *)malloc(sizeof(char) * bufsize);
        if (!cmd_array[i])
        {
        fprintf(stderr, "Memory allocation error");
        exit(EXIT_FAILURE);
        }
        cmd_array[i] = line;
        cmd_array[i + 1] = NULL;
        return cmd_array;
    }
    else
    {
        // read script
        bufsize = 64;
        buffer = (char*) malloc(sizeof(char*) * bufsize);
        getline(&buffer, &bufsize, fptr);
        loc_read = split_cmd(buffer, SCR_DELIM);
        if (strcmp(loc_read[0], "/home/pawel/Desktop/newshell/script\n") == 0)
        {
            cmd_array = (char **)calloc(sizeof(char *), cmd_count);
            if (!cmd_array)
            {
            fprintf(stderr, "Memory allocation error");
            exit(EXIT_FAILURE);
            }
            cmd_array[i] = (char *)malloc(sizeof(char) * bufsize);
            if (!cmd_array[i])
            {
            fprintf(stderr, "Memory allocation error");
            exit(EXIT_FAILURE);
            }
            while (getline(&cmd_array[i], &bufsize, fptr) != -1)
            {
                i++;
                if (i == 20)
                {
                    cmd_count += cmd_count;
                    cmd_array = (char**)realloc(cmd_array, cmd_count);
                }
                cmd_array[i] = (char *)malloc(sizeof(char) * bufsize);
            }
            fclose(fptr);
            cmd_array[i] = NULL;
            return cmd_array;
        }
        else
        {
            perror("Incorrect location to main.c");
            return NULL;
        }
    }
}