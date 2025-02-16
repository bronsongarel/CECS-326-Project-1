#include <windows.h>
#include <stdio.h>



#define BUFFER_SIZE 4096  // Buffer size for reading/writing
#define STRING_SIZE 25
void FileCopy(HANDLE pipeWriteEnd, const char *sourceFile);
void child(HANDLE pipeReadEnd, const char *outputFile);

int main(int argc, char *argv[]) {
    if(argc < 3){
        perror("Error with text files passed");
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
     return 0;
 }

 void FileCopy(HANDLE pipeWriteEnd, const char *sourceFile){ 
    FILE *stream = fopen(sourceFile, "r");  // Open file in read mode
    if (stream == NULL) {
       perror("Error opening source file");
       return;
    }
    DWORD written;
    char buffer[BUFFER_SIZE];
    while(fgets(buffer, sizeof(buffer), stream)){           //Write to the pipe from input file
       if(!WriteFile(pipeWriteEnd, buffer, strlen(buffer), &written, NULL)){
          perror("Error when writing to the pipe");
          break;
       }
    } 
    fclose(stream); //Close the file
    return;
 }

 void child(HANDLE pipeReadEnd, const char *outputFile){
    FILE *output = fopen(outputFile, "w"); // Open in text mode
    if (!output) {
        perror("Error opening output file");
        return;
    }
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    while (ReadFile(pipeReadEnd, buffer, BUFFER_SIZE, &bytesRead, NULL) && bytesRead > 0) {
       if(! fwrite(buffer, 1, bytesRead, output)){
        perror("Error when writing to output file");
       }
    }
    fclose(output);
}
