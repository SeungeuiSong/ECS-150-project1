#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define CMDLINE_MAX 512
#define ARGUMENT_MAX 16
#define TOKEN_MAX 32

int ISERROR = 0; //global variable because an error can occur anywhere in the program.

void runExit(){

        fprintf(stderr, "Bye...\n");
        fprintf(stderr, "+ completed 'exit' [%i]\n", EXIT_SUCCESS);               
        exit(0);
}

void runPwd(int argc){

        char buffer[CMDLINE_MAX]; //to be able to print out some directory that is potentially 512 chars long
        if (argc > 1){
                fprintf(stderr, "Error: Assignment said assume no arguments for pwd\n");
                ISERROR = 1;
        }else{
                fprintf(stdout, "%s\n", getcwd(buffer,CMDLINE_MAX));
        }
}

void runCd(int argc, char *destination){
        
        if (argc == 3){
                fprintf(stderr, "Error: Cannot cd into directory\n"); //Real reason is we assume one argument for cd
                ISERROR = 1;
        }
        else if (chdir(destination) == -1){
                fprintf(stderr, "Error: Cannot cd into directory\n");
                ISERROR = 1;
        }
}

void runRedirection(int argc, int redirectionLocation, char *argumentList[]){
        
        int output_fd;
        int retval;
        if (redirectionLocation == 0){ //Command line related errors
                fprintf(stderr, "Error: missing command\n");
                ISERROR = 1;
        }
        else if (redirectionLocation + 1 == argc){  //Array location + 1 equals size. If it's last, there's not output file
                fprintf(stderr, "Error: no output file\n");
                ISERROR = 1;
        }
        else{ //Passed all initial errors
                output_fd = open(argumentList[redirectionLocation + 1], O_WRONLY | O_TRUNC | O_CREAT, 0600);
                char * precedingCommand[redirectionLocation + 1]; //holds a command, message, and NULL

                        for (int i = 0; i < redirectionLocation; i++){
                                precedingCommand[i] = argumentList[i];
                        }
                        precedingCommand[redirectionLocation] = NULL;

                        if (output_fd == -1){
                                fprintf(stderr, "Error: cannot open output file\n");
                        }
                        else{
                                pid_t pid = fork();
                                if (pid == 0){ //Child
                                        dup2(output_fd, STDOUT_FILENO);
                                        retval = execvp(precedingCommand[0], precedingCommand);
                                        close(output_fd); 
                                } else if (pid !=0){
                                        wait(&retval);
                                }
                                else {
                                        perror("Error with forking");
                                        exit(1);
                                }
                                      
                        }

        }

}

void executeRegularCommand(char * cmd, char* argumentList[]){

        int retval;
        pid_t pid = fork();
                if (pid == 0){ //Child
                        retval = execvp(cmd, argumentList);
                        if (retval == -1){
                                printf("Error: command not found\n");
                        }
                } else if (pid !=0){
                        wait(&retval);
                }
                else {
                        perror("Error with forking");
                        exit(1);
                }
}


int main(void){

        char cmd[CMDLINE_MAX];
        char cmdDisplay[CMDLINE_MAX]; //required because strtok will manipulate our cmd
        int argc;
        int redirectionLocation;
        int isRedirect;

        while (1) {
                char *nl;
                
                /* Must set these to 0 as we literally read a new command line every time */
                *cmd = '\0';
                *cmdDisplay='\0';
                isRedirect = 0;
                redirectionLocation = 0; 
                argc = 0;
                ISERROR = 0;

                /* Print prompt */
                printf("sshell$ ");
                fflush(stdout);

                /* Get command line */
                fgets(cmd, CMDLINE_MAX, stdin);

                /* Print command line if stdin is not provided by terminal */
                if (!isatty(STDIN_FILENO)) {
                        printf("%s", cmd);
                        fflush(stdout);
                }

                /* Remove trailing newline from command line */
                nl = strchr(cmd, '\n');
                if (nl)
                        *nl = '\0';

                strcpy(cmdDisplay, cmd); //Necessary to display the command at the end. strtok modifies cmd

                char* argumentList[ARGUMENT_MAX + 1]; //must hold null at the end as well
                char *token;
                
                token = strtok(cmd, " ");

                while (token != NULL){ //2 Cases. Redirection or a normal command

                        if (!strcmp(token,">") ){
                                isRedirect = 1;
                                redirectionLocation = argc; //such that it is 1:1 with array location

                        }
                        if (strlen(token) > TOKEN_MAX){
                                fprintf(stderr, "Error: Token exceeded token max\n");
                                ISERROR = 1;
                        } else
                                argumentList[argc] = token;

                        token = strtok(NULL, " "); //must set the token to null again
                        argc +=1;
                }
                
        
                if (argc > ARGUMENT_MAX){
                        fprintf(stderr, "Error: Too many process arguments\n");
                        ISERROR = 1;
                } else{
                        argumentList[argc] = NULL; //Necessary for execvp
                }


                //3 possible builtin commands
                if (!(ISERROR)){

                        if (!strcmp(argumentList[0],"exit")){
                                runExit();
                        } else if (!strcmp(argumentList[0],"pwd")){
                                runPwd(argc);
                        } else if (!strcmp(argumentList[0],"cd")){
                                runCd(argc, argumentList[1]); //if cd is ran with no argument, still works as NULL will be passed.
                        }
                        else if (isRedirect){
                                runRedirection(argc, redirectionLocation, argumentList);
                        }
                        else {
                                executeRegularCommand(argumentList[0], argumentList);\
                        }


                }
                
                if (!ISERROR){
                        fprintf(stderr, "+ completed '%s' [%i]\n", cmdDisplay,EXIT_SUCCESS);
                }
                else{
                        fprintf(stderr, "+ completed '%s' [%i]\n", cmdDisplay,EXIT_FAILURE);
                }

        }

}
