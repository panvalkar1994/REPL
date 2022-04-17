#include<stdio.h>
#include "mpc.h"
#define true 1

// Define version of the program
const char* version = "v0.0.0.1";

// declare a bufer for input
const int input_size = 1024;
static char input[input_size];

int main(int argc, char* argv[]) {

    // Create some parser
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* REPL = mpc_new("repl");

    // define language
    mpca_lang(MPCA_LANG_DEFAULT,"number : /-?[0-9]+/; operator : '+' | '-' | '*' | '/'; expr : <number> | '('<operator> <expr>+ ')'; repl : /^/ <operator> <expr>+ /$/ ;",  Number, Operator, Expr, REPL);

    mpc_result_t r;
    printf("Welcome to REPL %s\n", version);
    puts("Press Ctrl+c to Exit");
    while (true)
    {
        // Prompt
        fputs("REPL> ", stdout);

        // read input
        fgets(input,input_size, stdin);

        if (mpc_parse("<stdin>", input, REPL, &r))
        {
            // Success print
            mpc_ast_print(r.output);
            mpc_ast_delete(r.output);
        }
        else
        {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }
                
    }

    // cleanup parser
    mpc_cleanup(4,Number,Operator,Expr,REPL);
    
    return 0;
}