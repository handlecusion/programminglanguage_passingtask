#include "parser.h"

/*
* #define SEME_COLON 
* lookup function add case ';' to SEME_COLON
* ; read next line and count IDENT, CONST, OP
* it will be global variable
* 
* define variabel with struct.
* it work as symbol table.
* struct symbol
* 	char* name, int value
* then it will use define variable
* 
* how can calculate?
* -> include calculer.c
*/

/******************************************************/
/* main driver */

int main(int argc, char** argv) {
    printf("Starting ..... \n");

/* Open the input data file and process its contents */
    if (( in_fp = fopen(argv[1], "r")) == NULL) {
        printf("ERROR");
    } else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
		fclose(in_fp);
		printf("close file");
    }
    return 0;
}


/*****************************************************/ 
/* lookup - a function to lookup operators and parentheses and return the token */
int lookup (char ch) {
    switch (ch) {
    case '(' :
        addChar();
        nextToken = LEFT_PAREN;
        break;
    case ')' :
        addChar();
        nextToken = RIGHT_PAREN;
        break;
    case '+' :
        addChar();
        nextToken = ADD_OP;
        break;
    case '-' :
        addChar();
        nextToken = SUB_OP;
        break;
    case '*' :
        addChar();
        nextToken = MULT_OP;
        break;
    case '/' :
        addChar();
        nextToken = DIV_OP;
        break;
	case '=' :
		addChar();
		nextToken = ASSIGN_OP;
		break;
	case ';' :
		addChar();
		nextToken = SEMI_COLON;
		break;
    default :
        addChar();
        nextToken = EOF;
        break;
    }
    return nextToken;
}

/*****************************************************/ 
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
}

/*****************************************************/
/* getChar - a function to get the next character of
input and determine its character class */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
                charClass = DIGIT;
        else charClass = UNKNOWN;
    } 
    else
        charClass = EOF;
}


/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank() {
    while (nextChar <= 32 && nextChar != -1) {
        getChar();
    }
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
    
    /* Parse identifiers */
    case LETTER:
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
		{
            addChar(); 
            getChar();
        }
        nextToken = IDENT;
        break;

    /* Parse integer literals */    
    case DIGIT:
        addChar();
        getChar();
        while (charClass == DIGIT)
		{
            addChar(); 
            getChar();
        }
        nextToken = INT_LIT;
        break;

    /* Parentheses and operators */
    case UNKNOWN:
        lookup(nextChar);
        getChar();
        break;

    /* EOF */
    case EOF:
        nextToken = EOF;
        lexeme[0] = 'E';
        lexeme[1] = 'O';
        lexeme[2] = 'F';
        lexeme[3] = 0;
        break;
    } /* End of switch */
	switch (nextToken)
	{
		case IDENT:
			cntId++;
			break;
		case INT_LIT:
			cntConst++;
			break;
		case ASSIGN_OP:
			cntOp++;
			break;
		case ADD_OP:
			cntOp++;
			break;
		case SUB_OP:
			cntOp++;
			break;
		case MULT_OP:
			cntOp++;
			break;
		case DIV_OP:
			cntOp++;
			break;
		case LEFT_PAREN:
			break;
		case RIGHT_PAREN:
			break;
		case SEMI_COLON:
			printf("ID : %d; CONST : %d; OP : %d;\n", cntId, cntConst, cntOp);
			cntId = 0;
			cntConst = 0;
			cntOp = 0;
			break;
	}


    //printf("Next Token is : %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */
