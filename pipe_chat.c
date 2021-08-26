#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUF_SIZE 256
#define CHANNEL_COUNT 2

int main(void)
{
    pid_t pid = 0;
    char buf[BUF_SIZE] = {0};
    int fd[CHANNEL_COUNT] = {0};
    
    if (-1 == pipe(fd))
    {
        perror("PIPE ERROR!");
        exit(1);
    }

    pid = fork();
    if (pid == -1)
    {
        perror("PROCESS FAILURE");
        exit(1);
    }

    if (pid == 0) 
    {    
        if (close(fd[1]) == -1)
        {
            perror("CLOSE FD1 ERROR");
            exit(1);
        }

        do
        {
            bzero(buf, 256); 
            if (-1 == read(fd[0], &buf, 256))
            {
                perror("READ ERROR");
                exit(1);                        
            }
            printf("Received %s\n", buf); 
        } while(strncmp(buf, "exit", 4) != 0);

        if (close(fd[0]) == -1)
        {
            perror("CLOSE FD0 ERROR");
            exit(1);
        }
        exit(0);
    }
    else 
    {
        if (close(fd[0]) == -1)
        {
            perror("CLOSE FD0 ERROR");
            exit(1);
        } 

        do
        {
            bzero(buf, 256); 
             
            fgets(buf, 256 ,stdin); 
            char *p = strchr(buf, '\n');
            if (p != NULL)
            {
                buf[strlen(buf) - 1] = '\0';
            }

            if (-1 == write(fd[1], &buf, strlen(buf)))
            {
                perror("WRITE ERROR");
                exit(1);
            }       
        } while(strncmp(buf, "exit", 4) != 0);

        if (close(fd[1]) == -1)
        {
            perror("CLOSE FD1 ERROR");
            exit(1);
        }         
        wait(NULL); 
        exit(0);
    }


    return 0;
}
