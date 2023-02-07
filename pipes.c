#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h> 
#include <time.h>
#include <sys/wait.h> 

main(int argc, char* argv[])
{
        int     fd[2];
        pid_t   childpid;
        pipe(fd);
        // fd[0] - read 
        // fd[1] - write 
        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }
        if(childpid == 0)
        {
                /* Child process closes up input side of pipe */
                close(fd[0]);
                char file_name[100];
                printf("type the name of file\n"); 
                //scanf("%d \n", &file_name); 
                fgets(file_name, 100, stdin);
                int n = strlen(file_name)+1;
                if (write(fd[1], &n, sizeof(int)) <0){
                        return 2; 
                }
                file_name[strlen(file_name)-1] = "\0"; 
                if (write(fd[1], &file_name, strlen(file_name)+1) <0){
                        return 3; 
                }
                close(fd[1]);
        }
        else
        {
                /* Parent process closes up output side of pipe */
                char buff[100];
                int n;
                close(fd[1]);
                if (read(fd[0], &n, sizeof(int))<0){
                        return 4;
                }
                if (read(fd[0], &buff, sizeof(char)*100) <0){
                        return 5;
                }
                printf("output = %s \n", &buff);
                close(fd[0]);
                wait(NULL); 
        }
        return 0; 

}