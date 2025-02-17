#include <windows.h>
#include <stdio.h>
#include "time.h"


#define BUFFER_SIZE 4096  // Buffer size for reading/writing

//FileCopy will take in the write end of the pipe as well as source.txt as inputs 
//Goal: Using the write end of the pipe, write to the pipe from the input file
void FileCopy(HANDLE pipeWriteEnd, const char *sourceFile);

//child will take in the read end of the pipe as well as the output.txt as inputs 
//Goal: copy data from the pipe using the read end into output.txt
void child(HANDLE pipeReadEnd, const char *outputFile);

int main(int argc, char *argv[]) {

    clock_t start, end;
    double tot;

    start = clock();
    if(argc < 3){    //Check if there were less than two files passed                  
        perror("Error with text files passed to command line, too few");
        return 1;
    }
    if(argc > 3){    //Check if there were more than two files passed                  
        perror("Error with text files passed to command line, too many");
        return 1;
    }
    HANDLE pipeReadEnd, pipeWriteEnd;  //handles for reading and writing to our pipe
    SECURITY_ATTRIBUTES security = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };  // Allows inheritance
    if(!CreatePipe(&pipeReadEnd, &pipeWriteEnd, &security, BUFFER_SIZE)){ 
        perror("The Pipe was not Created");
        return 0;
    }

    FileCopy(pipeWriteEnd, argv[1]);      // Write file contents to the pipe
    CloseHandle(pipeWriteEnd);            // Close the write end of the pipe before reading
    child(pipeReadEnd, argv[2]);          // Read from the pipe and write to output file
    CloseHandle(pipeReadEnd);             // Close the read end of the pipe
    printf("source.txt successfully copied through the pipe and into output.txt!\n");

    end = clock();

    tot = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", tot);

    return 0;
 }

 void FileCopy(HANDLE pipeWriteEnd, const char *sourceFile){ 
    FILE *stream = fopen(sourceFile, "r");  // Open file in read mode
    if (stream == NULL) {                   //Check if there was an error opening the file
       perror("Error opening source file");
       return;
    }
    DWORD written;
    char buffer[BUFFER_SIZE];
    while(fgets(buffer, sizeof(buffer), stream)){                             //While there are still lines of text in the input file
       if(!WriteFile(pipeWriteEnd, buffer, strlen(buffer), &written, NULL)){  //Write to the pipe from input file
          perror("Error when writing to the pipe");                           //and check for error writing to the pipe
          break;
       }
    } 
    fclose(stream); //Close the input file
    return;
 }

 void child(HANDLE pipeReadEnd, const char *outputFile){
    FILE *output = fopen(outputFile, "w"); //Open in write mode
    if (!output) {                         //Check if there was an error opening the output file
        perror("Error opening output file");
        return;
    }
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    while (ReadFile(pipeReadEnd, buffer, BUFFER_SIZE, &bytesRead, NULL) && bytesRead > 0) {  //While the pipe is reading more than 0 bytes
       if(! fwrite(buffer, 1, bytesRead, output)){                                           //write the buffer to the output file
        perror("Error when writing to output file");                                         //and check if there was an error doing so
       }
    }
    fclose(output);  //close the output file
}
