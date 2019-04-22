/*
* File name: table.h
* Compiler: MS Visual Studio 2017
* Author: Liam Richens, 040896769
* Course: CST 8152 – Compilers, Lab Section: 12 
* Assignment: 2
* Date: April 1
* Professor: Sv. Ranev
* Purpose: Transition Table declarations.
* Function list: st_table, as_table, aa_table, kw_table*
*/

#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*   Source end-of-file (SEOF) sentinel symbol
*    '\0' or one of 255,0xFF,EOF
*/

/*  Special case tokens processed separately one by one
*  in the token-driven part of the scanner
*  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
*  white space
*  !!comment , ',' , ';' , '-' , '+' , '*' , '/', << ,
*  .AND., .OR. , SEOF, 'illegal symbol',
*/


/*REPLACE *ESN* and *ESR* WITH YOUR ERROR STATE NUMBER */
#define ES  11 /* Error state  with no retract */
#define ER  12 /* Error state  with retract */
#define IS -1    /* Invalid state */

/* State transition table definition */

/*REPLACE *CN* WITH YOUR COLUMN NUMBER*/

#define TABLE_COLUMNS 8
/*transition table - type of states defined in separate table */
/*Columns from left to right: [a-zA-z],	0,	[1-9],	. ,	@,	“,	SEOF,	other*/

/* st_table[]
* Purpose: 	transition table declaration
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions :
* Parameters : 
* Return value :
* Algorithm :
* details;*/
int st_table[][TABLE_COLUMNS] = {
	/* State 0 */{ 1, 6, 4, ES, ES, 9, 13, ES },
	/* State 1 */{ 1, 1, 1, 2, 3, 2, ER, 2 },
	/* State 2 */{ IS, IS, IS, IS, IS, IS, IS, IS },
	/* State 3 */{ IS, IS, IS, IS, IS, IS, IS, IS },
	/* State 4 */{ ES, 4, 4, 7, 5, 5, ER, 5 },
	/* State 5 */{ IS, IS, IS, IS, IS, IS, IS, IS },
	/* State 6 */{ ES, 6, ES, 7, ES, 5, ER, 5 },
	/* State 7 */{ 8, 7, 7, 8, 8, 8, ER, 8 },
	/* State 8 */{ IS, IS, IS, IS, IS, IS, IS, IS },
	/* State 9 */{ 9, 9, 9, 9, 9, 10, ER, 9 },
	/* State 10 */{ IS, IS, IS, IS, IS, IS, IS, IS },
	/* State 11 */{ IS, IS, IS, IS, IS, IS, IS, IS },
	/* State 12 */{ IS, IS, IS, IS, IS, IS, IS, IS },
	/* State 13 */{ IS, IS, IS, IS, IS, IS, IS, IS },
};
	
	
	

	/* Accepting state table definition */
	/*REPLACE *N1*, *N2*, and *N3* WITH YOUR NUMBERS*/
#define ASWR    14  /* accepting state with retract */
#define ASNR    15  /* accepting state with no retract */
#define NOAS    16  /* not accepting state */

int as_table[] = { NOAS, NOAS, ASWR, ASNR, NOAS, ASWR, NOAS, NOAS, ASWR, NOAS, ASNR, ASNR, ASWR };
	
/*FOR EACH OF YOUR ACCEPTING STATES YOU MUST PROVIDE
ONE FUNCTION PROTOTYPE.THEY ALL RETURN Token AND TAKE
ONE ARGUMENT : A string REPRESENTING A TOKEN LEXEME.*/
/* Accepting functions. */
Token aa_func02(char lexeme[]);
Token aa_func03(char lexeme[]);
Token aa_func05(char lexeme[]);
Token aa_func08(char lexeme[]);
Token aa_func10(char lexeme[]);
Token aa_func12(char lexeme[]);
Token aa_func13(char lexeme[]);


/* defining a new type: pointer to function (of one char * argument)
returning Token
*/

typedef Token(*PTR_AAF)(char *lexeme);


/* aa_table[]
* Purpose: accepting function call back table
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : aa_func02, aa_func03, aa_func05, aa_func08, aa_func10, aa_func12, aa_func13
* Parameters :
* Return value :
* Algorithm :
* details;*/

PTR_AAF aa_table[] = {

	NULL,
	NULL,
	aa_func02,
	aa_func03,
	NULL,
	aa_func05,
	NULL,
	NULL,
	aa_func08,
	NULL,
	aa_func10,
	aa_func12,
	aa_func12,
	aa_func13

	/*HERE YOU MUST PROVIDE AN INITIALIZATION FOR AN ARRAY OF POINTERS
	TO ACCEPTING FUNCTIONS.THE ARRAY HAS THE SAME SIZE AS as_table[].
	YOU MUST INITIALIZE THE ARRAY ELEMENTS WITH THE CORRESPONDING
	ACCEPTING FUNCTIONS(FOR THE STATES MARKED AS ACCEPTING IN as_table[]).
	THE REST OF THE ELEMENTS MUST BE SET TO NULL.
	*/


};

/* Keyword lookup table (.AND. and .OR. are not keywords) */

/* kw_table[]
* Purpose: keyword lookup table
* Author : Liam Richens 040896769
* History / Versions : 1.0
* Called functions : aa_func02, aa_func03, aa_func05, aa_func08, aa_func10, aa_func12, aa_func13
* Parameters :
* Return value :
* Algorithm :
* details;*/
#define KWT_SIZE  10

char * kw_table[] =
{
	"ELSE",
	"FALSE",
	"IF",
	"PLATYPUS",
	"READ",
	"REPEAT",
	"THEN",
	"TRUE",
	"WHILE",
	"WRITE"
};

#endif
