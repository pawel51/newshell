#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "shell.h"

#define SPACE_DELIM " \t\r\n\a"
#define PIPE_DELIM "|\n"
#define RED_DELIM ">>"


/**
 * @brief Creates a new child process, handles pipes, redirections and suspension
 * @param cmd single programm
 * @param input where from to read
 * @param first it is first programm in the whole line
 * @param last it is last programm in the whole line
 * @return pipedes[1] - output of child process
 */
int spawn_proc(char *cmd, int input, int first, int last)
{
    int pipedes[2], status, ampersand = 0;
    pid_t pid;

    if (last == 1 && cmd[strlen(cmd) - 1] == '&')
    {
        cmd[strlen(cmd) - 1] = '\0';
        ampersand = 1;
    }

    /* Invoke pipe */
    pipe(pipedes);
    pid = fork();
    /*
	 SCHEME:
	 	STDIN --> O --> O --> O --> STDOUT
	*/
    if (pid == 0)
    {
        char **atoms, **redirect;
        if (first == 1 && last == 0 && input == 0)
        {
            atoms = split_cmd(cmd, SPACE_DELIM);
            // First command
            dup2(pipedes[1], STDOUT_FILENO);
        }
        else if (first == 0 && last == 0 && input != 0)
        {
            atoms = split_cmd(cmd, SPACE_DELIM);
            // Middle command
            dup2(input, STDIN_FILENO);
            dup2(pipedes[1], STDOUT_FILENO);
        }
        else
        {
            // Last command or First and Last in the same time
            redirect = split_cmd(cmd, RED_DELIM);
            if (redirect[1])
            {
                dup2(input, STDIN_FILENO);

                char **file_path = split_cmd(redirect[1], SPACE_DELIM);
                int fd = open(file_path[0], O_CREAT | O_RDWR, S_IRWXU);
                if (!fd)
                {
                    fprintf(stderr, "Error: opening file: %s\n", strerror(errno));
                }
                if (dup2(fd, 1) < 0)
                {
                    fprintf(stderr, "Error: duplicating file descriptor while redirecting: %s\n", strerror(errno));
                }

                char **first_cmd = split_cmd(redirect[0], SPACE_DELIM);
                if (execvp(first_cmd[0], first_cmd) == -1)
                {
                    perror("lsh");
                }
                exit(EXIT_FAILURE);
            }
            else
            {
                // Last command
                dup2(input, STDIN_FILENO);
                atoms = split_cmd(redirect[0], SPACE_DELIM);
                if (execvp(atoms[0], atoms) == -1)
                    exit(EXIT_FAILURE); // If child fails
            }
        }

        if (execvp(atoms[0], atoms) == -1)
            exit(EXIT_FAILURE); // If child fails
    }
    else if (pid > 0)
    {
        // Parent process
        if (input != 0)
            close(input);

        // Nothing more needs to be written
        close(pipedes[1]);

        // If it's the last command, nothing more needs to be read
        if (last == 1)
            close(pipedes[0]);
        // If there is & in the end wait for child
        if (!ampersand)
        {
            waitpid(pid, &status, WUNTRACED);
        }

        return pipedes[0];
    }
    else if (pid < 0)
    {
        // Error forking
        fprintf(stderr, "Fork error in pipeExecute: %d", errno);
        exit(EXIT_FAILURE);
    }
}
