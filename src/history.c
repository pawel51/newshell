#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "shell.h"

#define BUFF_SIZE 64
#define LINE_COUNT 20


void update_history(char* cmd_line){
    
    if(!strcmp("\n", cmd_line)){
        return;
    }

    char *line = cmd_line;


    char **cmd_history_array;
    char *arg = "history.txt", *cmd;
    int i = 0;
    size_t buffersize = BUFF_SIZE;
    
    
    FILE *rfile = fopen(arg, "r");
    if(!rfile){
        fprintf(stderr, "Cannot open history.txt mode: r\n");
    }
    
    cmd_history_array = (char **)calloc(sizeof(char *), LINE_COUNT); 
    if(!cmd_history_array){
        printf("Error while allocating history memory\n");
    }
    
    cmd_history_array[i] = (char *)malloc(sizeof(char) * BUFF_SIZE);
    while(getline(&cmd_history_array[i], &buffersize, rfile) != -1){
        i++;
        cmd_history_array[i] = (char *)malloc(sizeof(char) * BUFF_SIZE);
    }
    cmd_history_array[i] = NULL;
    
    fclose(rfile);

    if (i <= LINE_COUNT){
        // translating  all lines one row below
        if(i == LINE_COUNT){
            i--;
        }
        for(int j=i; j>=1; j--){
            cmd_history_array[j] = cmd_history_array[j-1];
        }
        cmd_history_array[0] = line;
    }
    else{
        fprintf(stderr, "Too much lines in history.txt\n");
        i = LINE_COUNT - 1;
    }

    FILE *wfile = fopen(arg, "w");
    if (!wfile){
        fprintf(stderr, "Cannot open history.txt mode: w\n");
    }
    for (int j=0; j<=i; j++){
        fputs(cmd_history_array[j], wfile);
    }

    free(cmd_history_array);
    fclose(wfile);
}