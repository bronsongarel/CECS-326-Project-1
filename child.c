/*
 * child.c
 *
 * This program reads the contents of the pipe and writes it to a file.
 */
  
 #include <stdio.h>
 #include <stdlib.h>
 #include <windows.h>
 #include <unistd.h>
 #include <fcntl.h>
 
 #define BUFFER_SIZE 10
 
 int main(int argc, char *argv[])
 {
    if (argc != 2){
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int copy = open(argv[1], O_RDONLY);
    if (copy == -1) {
        perror("Error opening file to copy");
        return 1;
    }

    int destination = open(argv[2], O_TRUNC | O_CREAT | O_WRONLY, 0644);
    if (destination == -1) {
        perror("Error opening copy destination file");
        close(copy);
        return 1;
    }

    // Assuming pipe is created

    // Fork Child process
    pid_t pid = fork();
    if (pid == -1){
        perror("Fork not created");
        close(copy);
        close(destination);
        return 1;
    }


    // Child process
    if (pid == 0){
        // NEED TO ClOSE WRITE END OF PIPE (PIPE[1])
        char buf[BUFFER_SIZE];
        ssize_t read_b;
        while ((read_b = read(/*PIPE FIRST INDEX*/, buf, sizeof(buf))) > 0) {
            write(destination, buf, read_b);
        }
        close(/*PIPE FIRST INDEX*/);
        close(destination);
        exit(0);
    }
    // else, parent function read copy file and write to pipe
 }
 
