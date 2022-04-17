#include<stdio.h>
#define true 1

// Define version of the program
const char* version = "v0.0.0.1";

// declare a bufer for input
const int input_size = 1024;
static char input[input_size];

int main(int argc, char* argv[]) {
    printf("Welcome to REPL %s\n", version);
    puts("Press Ctrl+c to Exit");
    while (true)
    {
        // Prompt
        fputs("REPL> ", stdout);

        // read input
        fgets(input,input_size, stdin);

        // print 
        printf("%s", input);
    }
    
    return 0;
}