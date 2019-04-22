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
/*static Buffer *sc_buf;*/

/* Scanner functions and variables */
extern Token malar_next_token();
extern int line;
/*extern STD sym_table; */
extern Buffer *str_LTBL;
extern char *kw_table[KWT_SIZE];

/* Parser function prototypes */
void parser();
void match(int, int);
void syn_eh(int);
void syn_printe(void);
void gen_incode(char*);

