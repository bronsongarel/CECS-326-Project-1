#include <windows.h>
#include <stdio.h>



#define BUFFER_SIZE 4096  // Buffer size for reading/writing
#define STRING_SIZE 25
void FileCopy(HANDLE hWritePipe, const char *inputFile);
void child(HANDLE hReadPipe, const char *outputFile);

int main(int argc, char *argv[]) {
    if(argc < 3){
        perror("Error with text files passed");
        return 1;
     }
    HANDLE hReadPipe, hWritePipe;  //handles for reading and writing to our pipe
    SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };  // Allows inheritance
    if(!CreatePipe(&hReadPipe, &hWritePipe, &sa, BUFFER_SIZE)){ 
        perror("The Pipe was not Created");
        return 0;
    }
     FileCopy(hWritePipe, argv[1]);      // Write file contents to the pipe
     CloseHandle(hWritePipe);            // Close the write end of the pipe before reading
     child(hReadPipe, argv[2]);          // Read from the pipe and write to output file
     CloseHandle(hReadPipe);             // Close the read end of the pipe
     printf("File successfully copied through the pipe!\n");
     return 0;
 }

 void FileCopy(HANDLE hWritePipe, const char *inputFile){ 
    FILE *stream = fopen(inputFile, "r");  // Open file in read mode
    if (stream == NULL) {
       perror("Error opening file");
       return;
    }
    DWORD written;
    char buffer[BUFFER_SIZE];
    while(fgets(buffer, sizeof(buffer), stream)){           //Write to the pipe from input file
       if(!WriteFile(hWritePipe, buffer, strlen(buffer), &written, NULL)){
          perror("Error when writing to the string");
          break;
       }
    
    } 
    fclose(stream); //Close the file
    return;
 }


 void child(HANDLE hReadPipe, const char *outputFile){
    FILE *output = fopen(outputFile, "w"); // Open in text mode
    if (!output) {
        perror("Error opening output file");
        return;
    }
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    while (ReadFile(hReadPipe, buffer, BUFFER_SIZE, &bytesRead, NULL) && bytesRead > 0) {
        fwrite(buffer, 1, bytesRead, output);
    }
    fclose(output);}
