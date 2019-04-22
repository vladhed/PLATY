/*
* File name: table.h
* Compiler: MS Visual Studio 2017
* Author: Liam Richens, 040896769
* Course: CST 8152 – Compilers, Lab Section: 12
* Assignment: 2
* Date: April 1
* Professor: Sv. Ranev
* Purpose: Transition Table declarations.
* Function list: scanner_init, malar_next_token, get_next_state, char_class, aa_func02, aa_func03, aa_func05, aa_func08, aa_func10, aa_func12, aa_func13, iskeyword*
*/

/* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
* to suppress the warnings about using "unsafe" functions like fopen()
* and standard sting library functions defined in string.h.
* The define does not have any effect in Borland compiler projects.
*/
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

/*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define BUF_ERR 255
#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
It is defined in platy_st.c */
extern Buffer * str_LTBL; /*String literal table */
int line; /* current line number of the source code */
extern int scerrnum;     /* defined in platy_st.c - run-time error number */

						 /* Local(file) global objects - variables */
static Buffer *lex_buf;/*pointer to temporary lexeme buffer*/
static pBuffer sc_buf; /*pointer to input source buffer*/
					   /* No other global variable declarations/definitiond are allowed */

					   /* scanner.c static(local) function  prototypes */
static int char_class(char c); /* character class function */
static int get_next_state(int, char, int *); /* state machine function */
static int iskeyword(char * kw_lexeme); /*keywords lookup functuion */

/*
* Purpose: Initializes Scanner
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : b_isempty, b_rewind, b_clear
* Parameters : Buffer * psc buf
* Return value : EXIT_SUCCESS
* Algorithm : 
* details; */
int scanner_init(Buffer * psc_buf) {
	if (b_isempty(psc_buf)) return EXIT_FAILURE;

	b_rewind(psc_buf);
	b_clear(str_LTBL);
	line = 1;
	sc_buf = psc_buf;
	return EXIT_SUCCESS;

}
/* malar_next_token
* Purpose: Special case token handling
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : b_getc, b_retract, b_mark, get_next_state, b_reset, b_allocate, b_getcoffset, b_free
* Parameters : void
* Return value : t
* Algorithm : if statements dealing with cases not dealt with by the transition table
* details;*/
Token malar_next_token(void) {
	
		Token t = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
		unsigned char c; /* input symbol */
		int state = 0; /* initial state of the FSM */
		char* lexeme;
		short lexstart;  /*start offset of a lexeme in the input char buffer (array) */
		short lexend;    /*end   offset of a lexeme in the input char buffer (array)*/
		int accept = NOAS; /* type of state - initially not accepting */

		
		
		while (1) { /* endless loop broken by token returns it will generate a warning */
			/*GET THE NEXT SYMBOL FROM THE INPUT BUFFER */
			c = b_getc(sc_buf);
			/* Part 1: Implementation of token driven scanner */
			/* every token is possessed by its own dedicated code */

			if (c == ' ' || c == '\t') continue;
			if (c == '\r') {
			
				c = b_getc(sc_buf);
			
				if (c != '\n')
					b_retract(sc_buf);
				line++;
				continue;
			}
			if (c == '\n') {
				line++;
				continue;
			}
			if (c == '=') {
				c = b_getc(sc_buf);
				if (c == '=') {
					t.code = REL_OP_T;
					t.attribute.rel_op = EQ;
					return t;
				}
				b_retract(sc_buf);
				t.code = ASS_OP_T;
				return t;
			}
			if (c == '.') {
				b_mark(sc_buf, b_getcoffset(sc_buf));
				c = b_getc(sc_buf);
				if (c == 'A' && b_getc(sc_buf) == 'N' && b_getc(sc_buf) == 'D' && b_getc(sc_buf) == '.') {
					t.code = LOG_OP_T;
					t.attribute.log_op = AND;
					return t;
				}
				else if (c == 'O' && b_getc(sc_buf) == 'R' && b_getc(sc_buf) == '.') {
					t.code = LOG_OP_T;
					t.attribute.log_op = OR;
					return t;
				}
				
				t.code = ERR_T;
				sprintf(t.attribute.err_lex, ".");
				b_reset(sc_buf);
				return t;
			}
			if (c == ',') {
				t.code = COM_T;
				return t;
			}
			if (c == '(') {
				t.code = LPR_T;
				return t;
			}
			if (c == ')') {
				t.code = RPR_T;
				return t;
			}
			if (c == '<') {
				c = b_getc(sc_buf);
				if (c == '<') {
					t.code = SCC_OP_T;
					return t;
				}
				else if (c == '>') {
					t.code = REL_OP_T;
					t.attribute.rel_op = NE;
					return t;
				}
				t.code = REL_OP_T;
				t.attribute.rel_op = LT;
				b_retract(sc_buf);
				return t;
			}
			if (c == '>') {
				t.code = REL_OP_T;
				t.attribute.rel_op = GT;
				return t;
			}
			if (c == '!') {
				c = b_getc(sc_buf);
				if (c == '!') {
					while (c != '\r' && c != '\n' && c != '\0' && c != 255) {
						c = b_getc(sc_buf);
					}
					b_retract(sc_buf);
					continue;
				}

				else {
					t.code = ERR_T;
					sprintf(t.attribute.err_lex, "!%c", c);
					while (c != '\r' && c != '\n' && c != '\0' && c != 255) {
						c = b_getc(sc_buf);
					}
					b_retract(sc_buf);
					return t;
				}
			}
			if (c == '/') {
				t.code = ART_OP_T;
				t.attribute.arr_op = DIV;
				return t;
			}
			if (c == ';') {
				t.code = EOS_T;
				return t;
			}
			if (c == '*') {
				t.code = ART_OP_T;
				t.attribute.arr_op = MULT;
				return t;
			}
			if (c == '-') {
				t.code = ART_OP_T;
				t.attribute.arr_op = MINUS;
				return t;
			}
			if (c == '+') {
				t.code = ART_OP_T;
				t.attribute.arr_op = PLUS;
				return t;
			}
			if (c == '{') {
				t.code = LBR_T; 
				return t;
			}
			if (c == '}') {
				t.code = RBR_T; 
				return t;
			}
			if (c == 255) {
				t.code = SEOF_T; 
				return t;
			}


			
			b_mark(sc_buf, b_getcoffset(sc_buf) - 1); 
			state = get_next_state(state, c, &accept);
			while (accept == NOAS) {
				
				c = b_getc(sc_buf);
				state = get_next_state(state, c, &accept);
			}

			
			lexstart = b_mark(sc_buf, 0);
			b_mark(sc_buf, lexstart);
			
			lex_buf = b_allocate(0, 0, 'f');

			if (!lex_buf) {
				scerrnum = BUF_ERR;
				t.code = ERR_T;
				strncpy(t.attribute.err_lex, "RUN TIME ERROR: ", ERR_LEN);
				t.attribute.err_lex[ERR_LEN] = '\0';
				return t;
			}

			if (accept == ASWR)
				b_retract(sc_buf);

			lexend = b_getcoffset(sc_buf);

			b_reset(sc_buf);

			while (b_getcoffset(sc_buf) < lexend)
				b_addc(lex_buf, b_getc(sc_buf));

			b_addc(lex_buf, '\0');

			lexeme = b_location(lex_buf);
			t = aa_table[state](lexeme);
			b_free(lex_buf);

			return t;
	}/*end while(1)*/
}
		
	
int get_next_state(int state, char c, int *accept) {
	int col;
	int next;
	col = char_class(c);
	next = st_table[state][col];
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif
		
	assert(next != IS);

		
#ifdef DEBUG
	if (next == IS) {
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);
	}
#endif
	*accept = as_table[next];
	return next;
}
/* char_class
* Purpose: Returns column number in st_table for the input character
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : isalpha, isdigit
* Parameters : char c
* Return value : 0, 1, 2, 3, 4, 5, 6 or 7
* Algorithm : if statements determine which column to return
* details;
*/
int char_class(char c) {
				

		if (isalpha(c))
			return  0;
		else if (c == '0')
			return  1;
		else if (isdigit(c))
			return 2;
		else if (c == '.')
			return 3;
		else if (c == '@')
			return 4;
		else if (c == '"')
			return 5;
		else if (c == '\0' || c == 255)
			return 6;
		else
			return 7;				
}
			
/* aa_func02
* Purpose: ACCEPTING FUNCTION FOR THE arithmentic variable identifier AND keywords(VID - AVID / KW)
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : 
* Parameters : char lexeme[]
* Return value : k
* Algorithm : 
* details;*/
Token aa_func02(char lexeme[]) {

	Token k = { 0 }; 
	int k_index = -1; 
	/*int vid_offset = 0;*/
	k_index = iskeyword(lexeme);
	if (k_index >= 0) {
		k.code = KW_T;
		k.attribute.kwt_idx = k_index;
	}

	else {
		k.code = AVID_T;
		if (strlen(lexeme) > VID_LEN) {
			lexeme[VID_LEN] = '\0';
		}
		strcpy(k.attribute.vid_lex, lexeme);
	}
		return k;
}

	

/* aa_func03
* Purpose: 	ACCEPTING FUNCTION FOR THE string variable identifier(VID - SVID)
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions :
* Parameters : char lexeme[]
* Return value : k
* Algorithm :
* details;*/
Token aa_func03(char lexeme[]) {
	Token k = { 0 };
	k.code = SVID_T;
	
	if (strlen(lexeme) > VID_LEN - 1) {
		lexeme[VID_LEN - 1]  = '@';
		lexeme[VID_LEN] = '\0';
	}
	strcpy(k.attribute.vid_lex, lexeme);
	
		return k;
}

	
/* aa_func08
* Purpose: ACCEPTING FUNCTION FOR THE floating - point literal(FPL)
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : strncpy
* Parameters : char lexeme[]
* Return value : k
* Algorithm : convert lexeme to floating point value
* details;*/
Token aa_func08(char lexeme[]) {
	Token k = { 0 };

		double fplVal = 0.0; 

	fplVal = atof(lexeme);

	
	if ((fplVal >= FLT_MIN && fplVal <= FLT_MAX) || fplVal == 0) {
		k.code = FPL_T;
		k.attribute.flt_value = (float)fplVal;
	}
	else {
		k.code = ERR_T;
		if (strlen(lexeme) > ERR_LEN) {
			lexeme[ERR_LEN - 3] = '.';
			lexeme[ERR_LEN - 2] = '.';
			lexeme[ERR_LEN - 1] = '.';
		}
		strncpy(k.attribute.err_lex, lexeme, ERR_LEN);
		k.attribute.err_lex[ERR_LEN] = '\0';
	}
		return k;
}


/* aa_func05
* Purpose: 	ACCEPTING FUNCTION FOR THE integer literal(IL) - decimal constant(DIL)
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : strncpy
* Parameters : char lexeme[]
* Return value : k
* Algorithm : convert lexeme to a decimal integer value
* details;*/
Token aa_func05(char lexeme[]) {
	Token k = { 0 };
	int ilVal = 0; 
	ilVal = atoi(lexeme);

	if ((ilVal >= SHRT_MIN && ilVal <= SHRT_MAX) || ilVal == 0) {
		k.code = INL_T;
		k.attribute.int_value = ilVal;
	}
	else {
		k.code = ERR_T;
		if (strlen(lexeme) > ERR_LEN) {
			lexeme[ERR_LEN - 3] = '.';
			lexeme[ERR_LEN - 2] = '.';
			lexeme[ERR_LEN - 1] = '.';
		}
		strncpy(k.attribute.err_lex, lexeme, ERR_LEN);
		k.attribute.err_lex[ERR_LEN] = '\0';
	}
		return k;
}


/* aa_func10
* Purpose: 	ACCEPTING FUNCTION FOR THE string literal(SL)
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : b_limit, b_addc
* Parameters : char lexeme[]
* Return value : k
* Algorithm : 
* details;*/
Token aa_func10(char lexeme[]) {
	Token k = { 0 };
	unsigned i = 0;
	/*int str_chars = 0;*/
	
	k.code = STR_T;
	k.attribute.str_offset = b_limit(str_LTBL);
	for (i = 0; i < strlen(lexeme); i++) {
		if (lexeme[i] == '\r') {
			line++;
		}
		if (lexeme[i] == '"') {
			continue;
		}
		b_addc(str_LTBL, lexeme[i]);
	}
	b_addc(str_LTBL, '\0');
		
	return k;
}
	



/* aa_func12
* Purpose: 	ACCEPTING FUNCTION FOR THE ERROR TOKEN
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : strncpy
* Parameters : char lexeme[]
* Return value : k
* Algorithm :
* details;*/
Token aa_func12(char lexeme[]) {
	Token k = { 0 };

	k.code = ERR_T;
	if (strlen(lexeme) > ERR_LEN) {
		lexeme[ERR_LEN - 3] = '.';
		lexeme[ERR_LEN - 2] = '.';
		lexeme[ERR_LEN - 1] = '.';
	}
	strncpy(k.attribute.err_lex, lexeme, ERR_LEN);
	k.attribute.err_lex[ERR_LEN] = '\0';
	return k;
}

/*			HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS(IF ANY).
				FOR EXAMPLE*/
/* aa_func13
* Purpose: 	ACCEPTING FUNCTION FOR SEOF
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : 
* Parameters : char lexeme[]
* Return value : k
* Algorithm :
* details;*/
Token aa_func13(char lexeme[]) {
	Token k = { 0 };
	
	k.code = SEOF_T;
	return k;
}
/* iskeyword
* Purpose: 	determines if a keyword from kw_table is present
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions :
* Parameters : char * kw_lexeme[]
* Return value : -1
* Algorithm :
* details;*/
int iskeyword(char * kw_lexeme) {
	int i = 0;
		   
	if (kw_lexeme == NULL) {
		return -1;
	}

	for (i = 0; i < KWT_SIZE; ++i) {
		if (strcmp(kw_lexeme, kw_table[i]) == 0)
			return i;
	}

	return -1;
}


