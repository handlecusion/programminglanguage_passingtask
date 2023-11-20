#include "parser.h"
#include "calculer.h"

void program() {
    statements();
	printf("Result ==> ");
	for (int i = 0; i < varLen; i++)
		printf("%s: %s; ", var[i].name, var[i].value);
	printf("\n");
}

void statements() {
    statement();
    if (nextToken == SEMI_COLON) {
		printf("%s", line);
		printf("ID : %d ; CONST : %d ; OP : %d ;\n", cntId, cntConst, cntOp);
		if (err == 0)
			printf("(OK)\n");
		else if (err == 1)
			printf("(Warning) duplication operator : process by adding the last op\n");
		else if (err == 2)
			printf("(Warning) wrong ident name : remove front wrong part\n");
		else if (err == 3)
			printf("(Error) not defined ident : use null value\n");
		else
			printf("(Error) \n");
		formular[0] = 0;
		cntId = 0;
		cntConst = 0;
		cntOp = 0;
		lineLen = 0;
		line[0] = 0;
		err = 0;
		lex();
		if (nextChar == EOF)
			return ;
        statements();
    }
}

void statement() {
    ident();
	var[varLen].name = strdup(lexeme);
	lex(); //add
	strcat(line, " = ");
	lex();
    //match(ASSIGN_OP);
    expression();
    if (nextToken == SEMI_COLON)
	{
		if (strchr(formular, '$'))
			var[varLen].value = NULL;
		else
			var[varLen].value = evaluate(formular);
		varLen++;
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
    if (nextToken == ADD_OP)
	{
		strcat(formular, "+");
		if (strlen(lexeme) != 1)
			err = 1;
		lex();
        term();
        term_tail();
    }
	else if(nextToken == SUB_OP)
	{
		strcat(formular, "-");
		if (strlen(lexeme) != 1)
			err = 1;
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
    if (nextToken == MULT_OP)
	{
		strcat(formular, "*");
		if (strlen(lexeme) != 1)
			err = 1;
		lex();
        factor();
        factor_tail();
    }
	else if (nextToken == DIV_OP)
	{
		strcat(formular, "/");
		if (strlen(lexeme) != 1)
			err = 1;
		lex();
		factor();
		factor_tail();
	}
}

void factor() {
    if (nextToken == LEFT_PAREN) {
		strcat(formular, "(");
		if (strlen(lexeme) != 1)
			err = 1;
		lex();
        expression();
		if (nextToken == RIGHT_PAREN)
		{
			strcat(formular, ")");
			if (strlen(lexeme) != 1)
				err = 1;
			lex();
		}
		else
			error();
    } else if (nextToken == IDENT) {
		if (searchValue(lexeme) == NULL)
		{
			err = 3;
			lexeme[0] = '$';
			lexeme[1] = 0;
			strcat(formular, lexeme);
		}
		else
			strcat(formular, searchValue(lexeme));
        ident();
		lex();
    } else if (nextToken == INT_LIT) {
		strcat(formular, lexeme);
        constt();
    } else {
        error();
    }
}

void ident() {
	if (nextToken == IDENT)
		cntId++;
	else
	{
		if (charClass == LETTER)
		{
			lexLen = 0;
			addChar();
			getChar();
			while (charClass == LETTER || charClass == DIGIT)
			{
				addChar(); 
				getChar();
			}
			nextToken = IDENT;
			lexLen = 0;
			err = 2;
			ident();
		}
		else
		{
			lex();
			ident();
		}
	}
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

char	*searchValue(char *s)
{
	for (int i = 0; i < varLen; i++)
	{
		if (!strcmp(s, var[i].name) && var[i].value != NULL)
			return (strdup(var[i].value));
	}
	return NULL;
}

void error() {
    printf("Error: Unexpected token : %d\n", nextToken);
    printf("Error: next char : %d\n", nextChar);
    exit(1);
}

int main(int argc, char **argv) {

    if ((in_fp = fopen(argv[1], "r")) == NULL) {
        printf("ERROR");
    } else {
		lex();
        program();
        fclose(in_fp);
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
    if (lineLen <= 198) {
        line[lineLen++] = nextChar;
        line[lineLen] = 0;
    }
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

    //printf("Next Token is : %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
} /* End of function lex */
