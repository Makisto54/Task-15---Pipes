#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define DELIM " "
#define BUF_SIZE 255
#define CHANNEL_COUNT 2
#define CHANNEL_DELIM "|"

int main(void)
{
    pid_t pid;
    int status = 0;
    int counter = 0;
    char buf[BUF_SIZE] = {0};
    int fd[CHANNEL_COUNT] = {0};

    char command_mas[CHANNEL_COUNT][BUF_SIZE];

    if (-1 == pipe(fd))
    {
        perror("PIPE ERROR!");
        exit(1);
    }
    
    printf("Input commands (Maximum 1 channel, exit for exit)\n");
    
    do
    {      
        bzero(buf, BUF_SIZE);
        bzero(command_mas, CHANNEL_COUNT * BUF_SIZE);
        fgets(buf, BUF_SIZE, stdin);
        char *p = strchr(buf, '\n');
        if (p != NULL)
        {
            buf[strlen(buf) - 1] = '\0';
        }

        char *ptr = strtok(buf, CHANNEL_DELIM);
        while (ptr != NULL)
        {
            strncpy(command_mas[counter], ptr, strlen(ptr));
            ptr = strtok(NULL, CHANNEL_DELIM);
            counter++;
        }        

        pid = fork();
        if (pid == -1)
        {
            perror("PROCESS FAILURE");
            exit(1);
        }

        if (pid == 0)
        {
            int count = 0;
            char params[BUF_SIZE][BUF_SIZE];
            bzero(params, BUF_SIZE * BUF_SIZE);

            char *command = strtok(command_mas[0], DELIM);
            while (command != 0)
            {
                strncpy(params[count], command, BUF_SIZE);
                command = strtok(NULL, DELIM);
                count++;
            }

            char **args = (char **)malloc((count + 1) * sizeof(char*));
            for (int i = 0; i < count; i++)
            {
                args[i] = (char *)malloc(BUF_SIZE * sizeof(char));
                strncpy(args[i], params[i], BUF_SIZE);
            }
            args[count] = NULL;

            if (close(fd[0]) == -1)
            {
                perror("CLOSE FD0 ERROR");
                exit(1);
            }

            if(counter != 1 && fd[1] != STDOUT_FILENO)
            {
                if (dup2(fd[1], STDOUT_FILENO) == -1)
                {
                    perror("STDOUT ERROR");
                    exit(1);                
                }                    
                if (close(fd[1]) == -1)
                {
                    perror("CLOSE FD1 ERROR");
                    exit(1);
                }
            }

            if (execvp(args[0], args) == -1)
            {
                perror("EXEC PROC 1 ERROR");
                exit(1);
            }
            exit(0);
        }

        pid = fork();
        if (pid == -1)
        {
            perror("PROCESS FAILURE");
            exit(1);
        }

        if (pid == 0)
        {
            if (counter != 2)
            {
                exit(0);
            }
            int count = 0;
            char pars[BUF_SIZE][BUF_SIZE];
            bzero(pars, BUF_SIZE * BUF_SIZE);

            char *comm = strtok(command_mas[1], DELIM);
            while (comm != 0)
            {
                strncpy(pars[count], comm, BUF_SIZE);
                comm = strtok(NULL, DELIM);
                count++;
            }

            char **argus = (char **)malloc((count + 1) * sizeof(char*));
            for (int i = 0; i < count; i++)
            {
                argus[i] = (char *)malloc(BUF_SIZE * sizeof(char));
                strncpy(argus[i], pars[i], BUF_SIZE);
            }
            argus[count] = NULL;

            if (close(fd[1]) == -1)
            {
                perror("CLOSE FD1 ERROR");
                exit(1);
            }

            if(fd[0] != STDIN_FILENO)
            {
                if (dup2(fd[0], STDIN_FILENO) == -1)
                {
                    perror("STDIN ERROR");
                    exit(1);                
                }
                if (close(fd[0]) == -1)
                {
                    perror("CLOSE FD0 ERROR");
                    exit(1);
                }
            }

            if (execvp(argus[0], argus) == -1)
            {
                perror("EXEC PROC 2 ERROR");
                exit(1);
            }
            exit(0);
        }
        counter = 0;
    } while (strncmp("exit", buf, 4) != 0);

    if (wait(&status) == -1)
    {
        perror("WAIT 1 ERROR");
        exit(1);
    }
    if (wait(&status) == -1)
    {
        perror("WAIT 2 ERROR");
        exit(1);
    }
    if (close(fd[0]) == -1)
    {
        perror("CLOSE FD0 ERROR");
        exit(1);
    }
    if (close(fd[1]) == -1)
    {
        perror("CLOSE FD1 ERROR");
        exit(1);
    }

    return 0;
}
