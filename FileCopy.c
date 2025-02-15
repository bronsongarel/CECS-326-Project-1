/*
 * FileCopy.c
 * 
 * This program opens a file and writes its contents to a pipe 
 */



#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <sys/types.h>
#include <process.h>

#include <string>
#include <fstream>
#include <iostream>

#define BUFFER_SIZE 25
#define STRING_SIZE 25
  
//argc = # of arguments passed from command line
//argv = array of inputs, what the function will use to copy into char[]     
[]char FileCopy(int argc, char *argv[]){ 
   HANDLE hReadPipe, hWritePipe; //handles for reading and writing to our pipe
   SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };  // Allows inheritance

   FILE *stream = fopen(argv[1], "r");  // Open file in read mode
    if (stream == NULL) {
        perror("Error opening file");
        return 1;
    }

   //Error with argc
   //Error with argv               
   if (!stream) {                //Error opening the file
      perror("Error opening the file!\n");
      CloseHandle(stream);

      return;
   }
   struct _stat fileStat;

   if (_stat(argv[1], &fileStat) != 0) {
       perror("Error: Could not get file stats.\n");
       return 1;
   }

  
   //Create the pipe. 
   //Check for error creating the pipe
   if(!CreatePipe(&hReadPipe, &hWritePipe, &sa, BUFFER_SIZE)){ 
      perror("The Pipe was not Created\n");
      return;

   }

   //Write to the pipe
   DWORD written;
   WriteFile(hWritePipe, stream, fileStat.st_size, &written, NULL);  
   //Check for error writing to the pipe
   //Check for error reading from the file
   

   //Close the file 
   //Check for error closing the file
   CloseHandle(stream);

}
