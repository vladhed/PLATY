#include <stdlib.h>
#include "parser.h"

void parser(){ 
  lookahead = malar_next_token(); 
  program(); 
  match(SEOF_T,NO_ATTR); 
  gen_incode("PLATY: Source file parsed"); 
}

void gen_incode(char* s) {
    printf("%s\n", s);
}

void match(int pr_token_code,int pr_token_attribute) {
    /* if the code doesn't match, post error and return */
    if (pr_token_code != lookahead.code) {
        syn_eh(pr_token_code);
        return;
    }

    /* if there's an attribute, check that matches */
    switch (pr_token_code) {
        case KW_T: 
        case ART_OP_T: 
        case LOG_OP_T: 
        case REL_OP_T: 
            
            if (pr_token_attribute != lookahead.attribute.get_int) {
                syn_eh(pr_token_code);
                return;
            }
            
    /* otherwise, get the next token */
        default:
            if (lookahead.code == SEOF_T)
                return;
            
            lookahead = malar_next_token();
            
    /* if there's an error, post it, and skip to the next token */
            if (lookahead.code == ERR_T) {
                syn_printe();
                ++synerrno;
                lookahead = malar_next_token();
            }
            
            return;
    }
}

/* error printing function for Assignment 3 (Parser), W19 */
void syn_printe(){
    Token t = lookahead;

    printf("PLATY: Syntax error:  Line:%3d\n",line);
    printf("*****  Token code:%3d Attribute: ", t.code);
    switch(t.code){
	    case  ERR_T: /* ERR_T     0   Error token */
		    printf("%s\n",t.attribute.err_lex);
	    break;
	    case  SEOF_T: /*SEOF_T    1   Source end-of-file token */
		    printf("SEOF_T\t\t%d\t\n", t.attribute.seof);
	    break;
	    case  AVID_T: /* AVID_T    2   Arithmetic Variable identifier token */
	    case  SVID_T :/* SVID_T    3  String Variable identifier token */
		    printf("%s\n", t.attribute.vid_lex);
	    break;
	    case  FPL_T: /* FPL_T     4  Floating point literal token */
		    printf("%5.1f\n",t.attribute.flt_value);
	    break;
	    case INL_T: /* INL_T      5   Integer literal token */
	            printf("%d\n",t.attribute.get_int);
	    break;
	    case STR_T:/* STR_T     6   String literal token */
		    b_mark(str_LTBL, t.attribute.str_offset);
	            printf("%s\n", b_location(str_LTBL));
	    break;
            
            case SCC_OP_T: /* 7   String concatenation operator token */
	            printf("NA\n" );
	    break;
	    
	    case  ASS_OP_T:/* ASS_OP_T  8   Assignment operator token */
		    printf("NA\n" );
	    break;
	    case  ART_OP_T:/* ART_OP_T  9   Arithmetic operator token */
		    printf("%d\n",t.attribute.get_int);
	    break;
	    case  REL_OP_T: /*REL_OP_T  10   Relational operator token */ 
		    printf("%d\n",t.attribute.get_int);
	    break;
	    case  LOG_OP_T:/*LOG_OP_T 11  Logical operator token */
		    printf("%d\n",t.attribute.get_int);
	    break;
	    
	    case  LPR_T: /*LPR_T    12  Left parenthesis token */
		    printf("NA\n" );
	    break;
	    case  RPR_T: /*RPR_T    13  Right parenthesis token */
	            printf("NA\n" );
	    break;
	    case LBR_T: /*    14   Left brace token */
	            printf("NA\n" );
	    break;
	    case RBR_T: /*    15  Right brace token */
	            printf("NA\n" );
	    break;
		    
	    case KW_T: /*     16   Keyword token */
	            printf("%s\n",kw_table [t.attribute.get_int]);
	    break;
	    
	    case COM_T: /* 17   Comma token */
	            printf("NA\n");
	    break;
	    case EOS_T: /*    18  End of statement *(semi - colon) */
	            printf("NA\n" );
	    break; 		
	    default:
	            printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
    }/*end switch*/
}/* end syn_printe()*/

void syn_eh(int sync_token_code) {
    syn_printe();
    ++synerrno;

    /* keep going until we reach the code we were looking for */
    while (lookahead.code != SEOF_T) {
        lookahead = malar_next_token();
        
        if (lookahead.code == sync_token_code) {
            
            if (lookahead.code != SEOF_T)
                lookahead = malar_next_token();
            
            return;
        }
    }
    
    if (sync_token_code != SEOF_T)
        exit(synerrno);

    /* else we hit the end of the file and we're are done */
}

void arithmetic_expression() {
}

void string_expression() {
}

void assignment_expression() {
    switch (lookahead.code) {
        case AVID_T:
            match(AVID_T, NO_ATTR);
            match(ASS_OP_T, NO_ATTR);
            arithmetic_expression();
            gen_incode("PLATY: Assignment expression (arithmetic) parsed");
            return;
        
        case SVID_T:
            match(SVID_T, NO_ATTR);
            match(ASS_OP_T, NO_ATTR);
            string_expression();
            gen_incode("PLATY: Assignment expression (string) parsed");
            return;
        
        /* If the current token code is not AVID or SVID, print an error. */
        default:
            syn_printe();
            return;
    }
    
}

/* FIRST set: AVID_T, SVID_T */
void assignment_statement() {
    assignment_expression();
    match(EOS_T, NO_ATTR);
    gen_incode("PLATY: Assignment statement parsed");
}

/* match TRUE or FALSE */
void pre_condition() {
    if(lookahead.code == KW_T) {
        switch (lookahead.attribute.kwt_idx) {
            case TRUE:
            case FALSE:
                break;
            default:
               syn_printe();
                break;
        }
    }
}

void logical_or_expression() {
}

/*
 * Production: Conditional Expression
 * FIRST set: { AVID_T, FPL_T, INL_T, SVID_T, STR_T }
 */
void conditional_expression() {
    logical_or_expression();
    gen_incode("PLATY: Conditional expression parsed");
}

void selection_statement() {
    match(KW_T, IF);
    pre_condition();
    match(LPR_T, NO_ATTR);
    conditional_expression();
    match(RPR_T, NO_ATTR);
    match(KW_T, THEN);
    match(LBR_T, NO_ATTR);
    opt_statements();
    match(RBR_T, NO_ATTR);
    match(KW_T, ELSE);
    match(LBR_T, NO_ATTR);
    opt_statements();
    match(RBR_T, NO_ATTR);
    match(EOS_T, NO_ATTR);
    gen_incode("PLATY: IF statement parsed");
}

void iteration_statement() {
    match(KW_T, WHILE);
    pre_condition();
    match(LPR_T, NO_ATTR);
    conditional_expression();
    match(RPR_T, NO_ATTR);
    match(KW_T, REPEAT);
    match(LBR_T, NO_ATTR);
    opt_statements();
    match(RBR_T, NO_ATTR);
    match(EOS_T, NO_ATTR);
    gen_incode("PLATY: WHILE statement parsed");
}

 /* FIRST set: AVID_T, SVID_T, IF, REPEAT, READ, WRITE */
void statement(void) {
    switch (lookahead.code) {
        case AVID_T:
        case SVID_T:
            assignment_statement();
            return;
        
        case KW_T:
            switch (lookahead.attribute.kwt_idx) {
                case IF:
                    selection_statement();
                    return;
                
                case READ:
                    input_statement();
                    return;
                
                case WRITE:
                    output_statement();
                    return;
                
                case WHILE:
                    iteration_statement();
                    return;
                
                default:
                    syn_printe();
                    return;
            }
        
        default:
            syn_printe();
            return;
    }
}
/* FIRST set: AVID_T, SVID_T, IF, WHILE, READ, WRITE */
void statements(void) {
    statement();
    sub_statements();
}

/*
 * Production: sub_Statements 
 * { AVID_T, SVID_T, IF, WHILE, READ, WRITE, empty }
 */
void sub_statements(void) {
    switch (lookahead.code) {
        case KW_T:
            /*
             * The token attribute must be ELSE, PLATYPUS, REPEAT or THEN. If the
             * keyword token attribute doesn't match any of the previous, return.
             */
            switch (lookahead.attribute.kwt_idx) {
                case ELSE:
                case PLATYPUS:
                case REPEAT:
                case THEN:
                    return;
            }
        
        case AVID_T:
        case SVID_T:
            statement();
            sub_statements();
            return;
    }
}

/* FIRST set for the <opt_statements> production is: {KW_T(IF), 
KW_T(WHILE) , KW_T(READ), KW_T(WRITE), AVID_T, SVID_T, and empty. */
void opt_statements(void){ 
    switch(lookahead.code){ 
        case AVID_T: 
        case SVID_T: 
            statements();
            break; 
        case KW_T: 
        /* check for IF,WHILE,READ,WRITE and in statements_p()*/ 
            if (lookahead.attribute.get_int == IF 
                || lookahead.attribute.get_int == WHILE 
                || lookahead.attribute.get_int == READ  
                || lookahead.attribute.get_int == WRITE) { 
                    statements();  
                    break; 
            } 
        default: /*empty string – optional statements*/ ;  
            gen_incode("PLATY: Optional statements parsed"); 
    }
}

void variable_list(){ 
   gen_incode("PLATY: Variable list parsed"); 
}

void program(){ 
    match(KW_T,PLATYPUS);
    match(LBR_T,NO_ATTR);
    opt_statements(); 
    match(RBR_T,NO_ATTR);  
    gen_incode("PLATY: Program parsed"); 
} 

void input_statement(){ 
   match(KW_T,READ);
   match(LPR_T,NO_ATTR);
   variable_list(); 
   match(RPR_T,NO_ATTR); 
   match(EOS_T,NO_ATTR);  
   gen_incode("PLATY: Input statement parsed"); 
} 

void output_list() {
   variable_list(); 
   /* or a STR_T */
}

void output_statement() {
   match(KW_T,WRITE);
   match(LPR_T,NO_ATTR);
   output_list();
   match(RPR_T,NO_ATTR); 
   match(EOS_T,NO_ATTR);  
   gen_incode("PLATY: Output statement parsed"); 
}

