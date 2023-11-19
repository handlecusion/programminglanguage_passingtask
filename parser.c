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

void program();
void statements();
void statement();
void expression();
void term();
void term_tail();
void factor();
void factor_tail();
void ident();
void constt();
void match(int expectedToken);
void error();

void program() {
    statements();
}

void statements() {
    statement();
    printf("statements");
    if (nextToken == SEMI_COLON) {
        match(SEMI_COLON);
        statements();
    }
}

void statement() {
    ident();
    getChar();
    match(ASSIGN_OP);
    expression();
    printf("statement");
    if (nextChar = ';') {
        nextToken = SEMI_COLON;
        match(SEMI_COLON);
        printf("Assignment statement parsed\n");
    } else if(charClass == EOF)
    {
        return ;
    } else
    {
        error();
    }
}

void expression() {
    term();
    term_tail();
}

void term_tail() {
    printf("term_tail function in\n");
    getChar();
    getNonBlank();
    if (nextChar == '+') {
        match(ADD_OP);
        term();
        term_tail();
    }
}

void term() {
    factor();
    factor_tail();
}

void factor_tail() {
    if (nextToken == MULT_OP) {
        match(MULT_OP);
        factor();
        factor_tail();
    }
}

void factor() {
    //printf("factor func in\n");
    getChar();
    getNonBlank();
    if (nextChar == '(') {
        match(LEFT_PAREN);
        expression();
        match(RIGHT_PAREN);
    } else if (charClass == LETTER) {
        ident();
        printf("Identifier parsed: %s\n", lexeme);
    } else if (charClass == DIGIT) {
        constt();
        printf("Constant parsed: %s\n", lexeme);
    } else {
        error();
    }
}

void ident() {
    lexLen = 0;
    getNonBlank();
    if (charClass == LETTER) {
        addChar();
        getChar();
        while (charClass == LETTER || charClass == DIGIT)
		{
            addChar(); 
            getChar();
        }
    }
}

void constt() {
    lexLen = 0;
    getNonBlank();
    if (charClass == DIGIT) {
        addChar();
        getChar();
        while (charClass == DIGIT)
		{
            addChar(); 
            getChar();
        }
        nextToken = INT_LIT;
    }
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
        getChar();
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


    //printf("Next Token is : %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */
