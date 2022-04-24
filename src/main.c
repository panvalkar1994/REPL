#include<stdio.h>
#include "mpc.h"
#define true 1

// Define version of the program
const char* version = "v0.0.0.1";

// declare a bufer for input
const int input_size = 1024;
static char input[input_size];

int number_of_nodes(mpc_ast_t* t){
    if(!t) return 0;
    if (t->children_num==0)
    {
        return 1;
    }
    else
    {
        int total = 1;
        for (int i = 0; i < t->children_num; i++)
        {
            total += number_of_nodes(t->children[i]);
        }
        return total;
        
    }
}

long eval_op(long x, char* op, long y){
    if (strcmp(op, "+")==0) return x + y;
    if (strcmp(op, "-")==0) return x - y;
    if (strcmp(op, "*")==0) return x * y;
    if (strcmp(op, "/")==0 && y!=0) return x / y;
    if (strcmp(op, "min")==0) return x<y ? x : y;
    if (strcmp(op, "max")==0) return x>y ? x : y;
    if(strcmp(op, "%")==0) return x % y;
    return 0;
    
}


long evaluate(mpc_ast_t* t){
    // tagged number return that number
    if (strstr(t->tag, "number"))
    {
        return atoi(t->contents);
    }
    

    // always second child will be the operator
    char* op = t->children[1]->contents;

    // evaluate again from third child
    long x = evaluate(t->children[2]);


    // iterate remaining children
    int i = 3;
    while (strstr(t->children[i]->tag, "expr"))
    {
        x = eval_op(x, op, evaluate(t->children[i]));
        i++;
    }
    // change sign only
    if(i == 3 && strcmp(op, "-")==0){
        return -x;
    }
    return x;
    
}

int main(int argc, char* argv[]) {

    // Create some parser
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* REPL = mpc_new("repl");

    // define language
    mpca_lang(MPCA_LANG_DEFAULT,"number : /-?[0-9]+/; operator : '+' | '-' | '*' | '/'|\"min\"|\"max\"|'%'; expr : <number> | '('<operator> <expr>+ ')'; repl : /^/ <operator> <expr>+ /$/ ;",  Number, Operator, Expr, REPL);

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
            long res = evaluate(r.output);
            printf("%li\n", res);
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