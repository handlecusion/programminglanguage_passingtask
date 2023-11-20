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


void program() {
    statements();
}

void statements() {
    statement();
    if (nextToken == SEMI_COLON) {
        //match(SEMI_COLON);
		printf("ID : %d ; CONST : %d ; OP : %d ;\n", cntId, cntConst, cntOp);
		cntId = 0;
		cntConst = 0;
		cntOp = 0;
		lex();
		if (nextChar == EOF)
			return ;
        statements();
    }
}

void statement() {
    ident();
	lex();
    //match(ASSIGN_OP);
    expression();
    if (nextToken == SEMI_COLON)
	{
        nextToken = SEMI_COLON;
        //match(SEMI_COLON);
	}
    else
    {
        error();
    }
}

void expression() {
    term();
    term_tail();
}

void term_tail() {
    //printf("term_tail function in\n");
    if (nextToken == ADD_OP)
	{
        //match(ADD_OP);
		lex();
        term();
        term_tail();
    }
	else if(nextToken == SUB_OP)
	{
		//match(SUB_OP);
		lex();
		term();
		term_tail();
	}
}

void term() {
    factor();
    factor_tail();
}

void factor_tail() {
	//printf("factor_tail : nextChar %d\n", nextChar);
    if (nextToken == MULT_OP)
	{
        //match(MULT_OP);
		lex();
        factor();
        factor_tail();
    }
	else if (nextToken == DIV_OP)
	{
		//match(DIV_OP);
		lex();
		factor();
		factor_tail();
	}
}

void factor() {
    //printf("factor func in\n");
    if (nextToken == LEFT_PAREN) {
        //match(LEFT_PAREN);
		lex();
        expression();
		if (nextToken == RIGHT_PAREN)
			lex();
		else
			error();
        //match(RIGHT_PAREN);
    } else if (nextToken == IDENT) {
        printf("Identifier parsed: %s\n", lexeme);
        ident();
    } else if (nextToken == INT_LIT) {
        printf("Constant parsed: %s\n", lexeme);
        constt();
    } else {
        error();
    }
}

void ident() {
	if (nextToken == IDENT)
	{
		cntId++;
		lex();
	}
	else
		error();
}

void constt() {
	if (nextToken == INT_LIT)
	{
		cntConst++;
		lex();
	}
	else
		error();
}

void match(int expectedToken) {
    if (nextToken == expectedToken) {
        printf("match expectToken : %d\n", nextChar);
    } else {
        error();
    }
}

void error() {
    printf("Error: Unexpected token : %d\n", nextToken);
    printf("Error: next char : %d\n", nextChar);
    exit(1);
}

int main(int argc, char **argv) {
    printf("Starting ..... \n");

    if ((in_fp = fopen(argv[1], "r")) == NULL) {
        printf("ERROR");
    } else {
		lex();
        program();
        fclose(in_fp);
        printf("File closed\n");
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
	//printf("addChar : nextChar : %c\n", nextChar);
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
        //printf("getChar : nextChar - %d\n", nextChar);
        if (isalpha(nextChar) || nextChar == '_')
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
		else if (nextChar <= 32 && nextChar != -1)
			charClass = SPACE;
        else {
            charClass = UNKNOWN;
            lookup(nextChar);
        }
    } 
    else
        charClass = EOF;
}


/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character */
void getNonBlank() {
	do {
		getChar();
	} while (nextChar <= 32 && nextChar != -1);
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
        //lookup(nextChar);
        getChar();
        break;
	
	/* Space */
	case SPACE:
    	getNonBlank();
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

    printf("Next Token is : %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */
