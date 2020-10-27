#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#define MAXLINE 80 /* The maximum length command */

void ReadCommand(char* args[])
{
    char r_input[80];
    int len_r_input = read(STDIN_FILENO, r_input, 80);

    if (r_input[len_r_input - 1] == '\n')
    {
        r_input[len_r_input - 1] = '\0';
    }
    const char s[2] = " ";
    char *token;
    
    /* get the first token */
    token = strtok(r_input, s);
    int index = 0;
    /* walk through other tokens */
    while( token != NULL ) {       
        // printf( "%s\n", token );
        args[index] = strdup(token);
        token = strtok(NULL, s);
        index++;
    }
    args[index] = NULL;
}
int main(void) {
    char *args[MAXLINE/2 + 1]; /* command line arguments */
    char* cmd;
    int argc = 0;
    int run = 1; /* flag to determine when to exit program */
    pid_t pid;
    int status;
    // cmd = "ls";
    // args[0] = "ls";
    // args[1] = "-la";
    // args[3] = NULL;
    // ReadCommand(cmd, args, argc); 
    // printf("%s", cmd);
    while (run) {
        printf("osh>");
        fflush(stdout);    
        ReadCommand(args); 
        cmd = strdup(args[0]);
        pid = fork();
        if (strcmp(cmd, "exit") == 0) { // Nếu lệnh là exit thì thoát chương trình 
            exit(0);
        }
        else {
            if (pid == 0) {
                //Đây là tiến trình con
                if (execvp(cmd, args) < 0) {     /* execute the command  */
                    printf("*** ERROR: exec failed\n");
                    exit(1);
                }
            }
            else if (pid > 0) {

                //Đây là tiến trình cha
                printf("Parents\n");
                while (wait(&status) != pid);

            }
            else {
                printf("*** ERROR: forking child process failed\n");
                exit(1);
            }
        }        
        // After reading user input, the steps are:
        // (1) fork a child process using fork()

        // (2) the child process will invoke execvp()
        // (3) parent will invoke wait() unless command included &
    }
    return 0;
}