/***************************************************************************/ /**

  @file         newshell.c

  @author       Paweł Szapiel

  @date         27/04/2021

  @brief        Simple shell supporting pipes, history, redirections

*******************************************************************************/

#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "shell.h"

/**
   @brief Invokes in loop: 1. reading line, 2. parsing, 3. executing
 */
void main_loop(int argc, char* argv[])
{
    int i = 0;
    char **line;

    while (1)
    {
        fprintf(stdout, "> ");
        line = read_line();
        if (line)
        {
            while (line[i])
            {
                split_line(line[i]);
                i++;
            }
            char *tmp;
            for (int j = 0; tmp; j++)
            {
                tmp = line[j];
                free(line[j]);
            }
        }
    }
}

/**
 * @brief Handles SIGQUIT and overrides default job with printing current history
*/
void handler(int signum)
{
    psignal(signum, "   Printing history.txt\n");
    FILE *fptr = fopen("history.txt", "r");
    if (!fptr)
    {
        perror("Cant open history.txt");
    }
    char c = fgetc(fptr);
    while (c != EOF)
    {
        printf("%c", c);
        c = fgetc(fptr);
    }
    fclose(fptr);
}

/**
   @brief Main entry point.
   @return exit status code
 */
int main(int argc, char* argv[])
{
    struct sigaction sa = {
        .sa_handler = handler,
        .sa_flags = SA_RESTART,
    };

    if (sigemptyset(&sa.sa_mask) < 0)
    {
        perror("sigemptyset");
        return 1;
    }

    if (sigaction(SIGQUIT, &sa, NULL) < 0)
    {
        perror("sigaction");
        return 1;
    }

    // Run command loop.
    main_loop(argc, argv);

    return EXIT_SUCCESS;
}
