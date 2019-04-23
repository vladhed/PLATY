#include "buffer.h"
#include "token.h"

/* Begin constants */
#define NO_ATTR     0
#define ELSE        0
#define FALSE       1
#define IF          2
#define PLATYPUS    3
#define READ        4
#define REPEAT      5
#define THEN        6
#define TRUE        7
#define WHILE       8
#define WRITE       9
#define KWT_SIZE    9

/* Error counter */
int synerrno;
/* Global variables */
static Token lookahead;

/* Scanner functions and variables */
extern Token malar_next_token();
extern int line;
extern Buffer *str_LTBL;
extern char *kw_table[KWT_SIZE];

/* Parser function prototypes */
void parser();
void match(int, int);
void syn_eh(int);
void syn_printe(void);
void gen_incode(char*);

void primary_arithmetic_expression();
void unary_arithmetic_expression();
void multiplicative_arithmetic_expression_p();
void multiplicative_arithmetic_expression();
void additive_arithmetic_expression_p();
void additive_arithmetic_expression();
void arithmetic_expression();
void primary_string_expression();
void string_expression_p();
void string_expression();
void assignment_expression();
void assignment_statement();
void pre_condition_p();
void primary_a_relational_expression();
void primary_s_relational_expression();
void relational_expression_a_p();
void relational_expression_s_p();
void relational_expression();
void logical_and_expression_p();
void logical_and_expression();
void logical_or_expression();
void logical_or_expression_p();
void conditional_expression();
void selection_statement();
void iteration_statement();
void statement();
void statements();
void statements_p();
void opt_statements(); 
void variable_identifier();
void variable_list_p();
void variable_list(); 
void program(); 
void input_statement();
void output_list();
void output_statement();
void syn_printi(int);
