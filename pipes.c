#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h> 
#include <time.h>
#include <sys/wait.h> 
#include <wait.h>
#include <fcntl.h> 
#include <sys/stat.h>
#include <locale.h>
#include "file_handler.h"
void text_file(char* name); 

int main()
{
        int     fd[2];
        pid_t   childpid;
        pipe(fd);
        // fd[0] - read 
        // fd[1] - write 
        setlocale(LC_ALL, "Rus");

        if((childpid = fork()) == -1)
        {
                perror("fork");
                exit(1);
        }
        if(childpid == 0)
        {
                /* Child process*/
                close(fd[0]);
                char file_name[100];
                printf("type the name of file\n"); 
                fgets(file_name, 100, stdin);
                file_name[strlen(file_name)-1] = '\0';//
                int n = strlen(file_name) + 1;//cause fget counting also "Enter" too
                if (write(fd[1], &n, sizeof(int)) <0){//length of the message
                        return 2; 
                }
                if (write(fd[1], &file_name, sizeof(char)*n) <0){//
                        return 3; 
                }  
                close(fd[1]);
        }
        else
        {
                /* Parent process*/
                close(fd[1]);
                char name_of_file[100];
                int n;
                if (read(fd[0], &n, sizeof(int))<0){
                        return 4;
                }
                if (read(fd[0], &name_of_file, sizeof(char)*n) <0){
                        return 5;
                }

                text_file(name_of_file); 
                close(fd[0]);
                wait(NULL); 
        }
        return 0; 
}