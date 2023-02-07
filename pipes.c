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
                int n = strlen(file_name);//cause fget counting also "Enter" too
                if (write(fd[1], &n, sizeof(int)) <0){//length of the message
                        return 2; 
                }
                file_name[strlen(file_name)-1] = '\0';//
                if (write(fd[1], &file_name, strlen(file_name)) <0){//message itself 
                        return 3; 
                }
                close(fd[1]);
        }
        else
        {
                /* Parent process*/
                char name_of_file[100];
                int n;
                int inputFd, outputFd, numRead, total, openFlags;
                mode_t filePerms;

                close(fd[1]);
                if (read(fd[0], &n, sizeof(int))<0){
                        return 4;
                }
                if (read(fd[0], &name_of_file, sizeof(char)*n) <0){
                        return 5;
                }
                for (int i=0; i<strlen(name_of_file);i++){
                        printf("%d  ", name_of_file[i]);
                }
                printf("\n");

                inputFd = open(name_of_file, O_RDONLY);
                if (inputFd == -1) {
                    printf("Error opening file: %s\n", name_of_file);
                    exit(-2);
                }
                openFlags = O_CREAT | O_WRONLY | O_TRUNC;
                filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
                char output_name[]="out.txt";
                outputFd = open(output_name, openFlags, filePerms);
                char buf;
                total = 0;
                // цикл для обработки файла
                while ((numRead = read(inputFd, &buf, 1) > 0))
                   {
                    write(outputFd, &buf, numRead);
                    if (buf == '.') {
                       buf = 0x0a;
                       write(outputFd, &buf, numRead);
                       total++;
                    }
                }
                printf("Amount of operations: %d\n", total);
                close(inputFd);
                close(outputFd);

                close(fd[0]);
                wait(NULL); 
        }
        return 0; 
}