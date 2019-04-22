/*
File name: buffer.c
Compiler: MS Visual Studio 2017
Author: Liam Richens, 040896769
Course: CST 8152 – Compilers, Lab Section: 12
Assignment: 1
Date: February 4, 2019
Professor: Sv. Ranev
Purpose: implement the functions given in platy_bt.c  
Function list: b_allocate(), b_addc(), b_clear(), b_free(), b_isfull(), b_limit(), b_capacity(), b_mark(), b_mode(), b_incfactor(), b_load(), b_isempty(), b_getc(), b_print(), b_eob(), b_compact(), b_rflag(), b_retract(), b_reset(), b_location(), b_getcoffset(), b_rewind()
*/
#include "buffer.h"

Buffer * b_allocate(short init_capacity, char inc_factor, char o_mode) {
	Buffer * b;
	char mode;

	if (init_capacity<0 || init_capacity>SHRT_MAX) {
		return NULL;
	}
	if (init_capacity == 0) {
		init_capacity = 200;
		inc_factor = 15;
		if (o_mode == 'f') {
			inc_factor = 0;
		}
	}

	switch (o_mode) {
	case 'f':
		mode = 0;
		break;
	case 'm':
		mode = -1;
		if (inc_factor < 1 || inc_factor>100) {
			return NULL;
		}
		break;
	case 'a':
		mode = 1;
		if (inc_factor < 1 || inc_factor>255) {
			return NULL;
		}
		break;
	default:
		return NULL;
	}
	b = (Buffer *)calloc(1, sizeof(Buffer));
	if (b == NULL) {
		return NULL;
	}
	b->inc_factor = inc_factor;
	b->mode = mode;
	b->cb_head = (char *)malloc(init_capacity * sizeof(char));
	if (b->cb_head == NULL) {
		free(b);
		return NULL;
	}
	b->capacity = init_capacity;
	b->flags = 0xfffc;

	return b;
}
pBuffer b_addc(pBuffer const pBD, char symbol) {
	unsigned short inc, avail, max;

	if (pBD == NULL) {
		return NULL;
	}
	if (pBD->addc_offset == pBD->capacity) {
		if (pBD->mode == 0) {
			return NULL;
		}
		if (pBD->mode == 1) {
			max = pBD->capacity + pBD->inc_factor;
		}
		if (pBD->mode == -1) {
			avail = SIZE_MAX - pBD->capacity;
			inc = avail * pBD->inc_factor / 100;
			max = pBD->capacity + inc;
		}
		if (max > SIZE_MAX) {
			max = (unsigned short)(SIZE_MAX - 1);
		}
		pBD->cb_head = (char*)realloc(pBD->cb_head, max * sizeof(char));
		if (pBD->cb_head == NULL) {
			return NULL;
		}
		pBD->capacity = max;
		pBD->flags |= 1 << R_FLAG;
	}
	*(pBD->addc_offset + pBD->cb_head) = symbol;
	pBD->addc_offset++;
	return pBD;
}
int b_clear(Buffer * const pBD) {
	if (pBD == NULL) {
		return -1;
	}
	pBD->capacity = 0;
	pBD->addc_offset = 0;
	pBD->getc_offset = 0;
	pBD->markc_offset = 0;
	pBD->flags = 0;
	return RT_SUCCEED;
	
}
void b_free(Buffer * const pBD) {
	if (pBD == NULL) {
		return;
	}
	free(pBD->cb_head);
	free(pBD);
}
int b_isfull(Buffer * const pBD) {
	if (pBD == NULL) {
		return -1;
	}
	if (pBD->addc_offset == pBD->capacity) {
		return 1;
	}
	return 0;
}
short b_limit(Buffer * const pBD) {
	if (pBD == NULL) {
		return -1;
	}
	return pBD->addc_offset;
}
short b_capacity(Buffer * const pBD) {
	if (pBD == NULL) {
		return -1;
	}
	return pBD->capacity;
}
short b_mark(pBuffer const pBD, short mark) {
	short save;
	if (pBD == NULL) {
		return -1;
	}
	save = pBD->markc_offset;
	pBD->markc_offset = mark;
	return save;
}
int b_mode(Buffer * const pBD) {
	if (pBD == NULL) {
		return -2;
	}
	return pBD->mode;
}
size_t b_incfactor(Buffer * const pBD) {
	if (pBD == NULL) {
		return 0x100;
	}
	return pBD->inc_factor;
}
int b_load(FILE * const fi, Buffer * const pBD) {
	int c;
	int count = 0;
	Buffer * p;

	if (pBD == NULL || fi == NULL) {
		return -1;
	}
	while (1) {
		c = fgetc(fi);
		if (feof(fi)) {
			break;
		}
		p = b_addc(pBD, c);
		if (p == NULL) {
			ungetc(c, fi);
			printf("Last character read from input file is: %c %d\n", c, c);
			return LOAD_FAIL;
		}
		count++;
	}
	return count;
}
int b_isempty(Buffer * const pBD) {
	if (pBD == NULL) {
		return -1;
	}
	if (pBD->addc_offset == 0) {
		return 1;
	}
	return 0;
}
char b_getc(Buffer * const pBD) {
	char c;
	if (pBD == NULL) {
		return -2;
	}
	c = *(pBD->cb_head + pBD->getc_offset);
	if (pBD->getc_offset == pBD->addc_offset) {
		pBD->flags |= 1 << EOB_FLAG;
	}
	else {
		pBD->flags &= ~(1 << EOB_FLAG);
		pBD->getc_offset++;
	}
	return c;
}
int b_eob(Buffer * const pBD) {
	if (pBD == NULL) {
		return -1;
	}
	return pBD->flags & (1 << EOB_FLAG);
}
int b_print(Buffer * const pBD) {
	char c;
	int count = 0;
	if (pBD == NULL) {
		return -1;
	}
	while (1) {
		c = b_getc(pBD);
		count++;
		if (b_eob(pBD)) {
			break;
		}
		printf("%c", c);
	}
	printf("\n");
	return count;

}
Buffer * b_compact(Buffer * const pBD, char symbol) {
	if (pBD == NULL) {
		return NULL;
	}
	pBD->cb_head = (char*)realloc(pBD->cb_head, (pBD->addc_offset + 1) * sizeof(char));
	if (pBD->cb_head == NULL) {
		return NULL;
	}
	pBD->capacity = pBD->addc_offset + 1;
	pBD->flags |= 1 << R_FLAG;
	*(pBD->addc_offset + pBD->cb_head) = symbol;
	pBD->addc_offset++;
	return pBD;
}
char b_rflag(Buffer * const pBD) {
	if (pBD == NULL) {
		return -2;
	}
	return (char)pBD->flags;
}
short b_retract(Buffer * const pBD) {
	if (pBD == NULL) {
		return -1;
	}
	pBD->getc_offset--;
	return pBD->getc_offset;
}
short b_reset(Buffer * const pBD) {
	if (pBD == NULL) {
		return -1;
	}
	pBD->getc_offset = pBD->markc_offset;
	return pBD->getc_offset;
}
short b_getcoffset(Buffer * const pBD) {
	if (pBD == NULL) {
		return -1;
	}
	
	return pBD->getc_offset;
}
int b_rewind(Buffer * const pBD) {
	if (pBD == NULL) {
		return -1;
	}

	pBD->markc_offset = 0;
	pBD->getc_offset = 0;
	return 0;
}
char * b_location(Buffer * const pBD) {
	if (pBD == NULL) {
		return NULL;
	}
	return pBD->cb_head + pBD->markc_offset;
}