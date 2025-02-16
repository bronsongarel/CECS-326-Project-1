# CECS-326-Project-1

Our program aims to open a text file named "source.txt' and write its contents to a pipe.  This pipe then is read by a second text file "output.txt" where the contents are copied.  This is accomplished through c as well as the use of windows built-in API functions: CreatePipe, WriteFile, ReadFile and CloseHandle.  

# How to Compile
right click windows folder and select "Open Integrated Terminal"
First command: gcc main.c -o main.exe
Second command: .\main.exe source.txt output.txt
