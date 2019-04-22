/*
File name: buffer.h
Compiler: MS Visual Studio 2017
Author: Liam Richens, 040896769
Course: CST 8152 – Compilers, Lab Section: 12
Assignment: 1
Date: February 4, 2019
Professor: Sv. Ranev
Purpose: header file with definitions for the buffer program
Function list: Struct BufferDescriptor
*/
#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

							/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

														   /* standard header files */
#include <stdint.h>
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

														   /* constant definitions */
#define RT_FAIL_1 -1         /* operation failure return value 1 */
#define RT_FAIL_2 -2         /* operation failure return value 2 */
#define LOAD_FAIL -2         /* load fail return value */

#define DEFAULT_INIT_CAPACITY 200   /* default initial buffer capacity */
#define DEFAULT_INC_FACTOR 15       /* default increment factor */


#define RT_SUCCEED 1										 /* You should add your own constant definitions here */

														   /* Add your bit-masks constant definitions here */
#define DEFAULT_FLAGS  0xFFFC
#define SET_EOB  
#define RESET_EOB 
#define CHECK_EOB 
#define SET_R_FLAG 
#define RESET_R_FLAG 
#define CHECK_R_FLAG 

														   /* user data type declarations */
typedef struct BufferDescriptor {
	char *cb_head;   /* pointer to the beginning of character array (character buffer) */
	short capacity;    /* current dynamic memory size (in bytes) allocated to character buffer */
	short addc_offset;  /* the offset (in chars) to the add-character location */
	short getc_offset;  /* the offset (in chars) to the get-character location */
	short markc_offset; /* the offset (in chars) to the mark location */
	char  inc_factor; /* character array increment factor */
	char  mode;       /* operational mode indicator*/
	unsigned short flags;     /* contains character array reallocation flag and end-of-buffer flag */
} Buffer, *pBuffer;
/*typedef Buffer *pBuffer;*/

/* function declarations */
Buffer * b_allocate(short init_capacity, char inc_factor, char o_mode);
pBuffer b_addc(pBuffer const pBD, char symbol);
int b_clear(Buffer * const pBD);
void b_free(Buffer * const pBD);
int b_isfull(Buffer * const pBD);
short b_limit(Buffer * const pBD);
short b_capacity(Buffer * const pBD);
short b_mark(pBuffer const pBD, short mark);
int b_mode(Buffer * const pBD);
size_t b_incfactor(Buffer * const pBD);
int b_load(FILE * const fi, Buffer * const pBD);
int b_isempty(Buffer * const pBD);
char b_getc(Buffer * const pBD);
int b_eob(Buffer * const pBD);
int b_print(Buffer * const pBD);
Buffer * b_compact(Buffer * const pBD, char symbol);
char b_rflag(Buffer * const pBD);
short b_retract(Buffer * const pBD);
short b_reset(Buffer * const pBD);
short b_getcoffset(Buffer * const pBD);
int b_rewind(Buffer * const pBD);
char * b_location(Buffer * const pBD);
/*
Place your function declarations here.
Do not include the function header comments here.
Place them in the buffer.c file
*/

#define EOB_FLAG 1
#define R_FLAG 0

#endif 
