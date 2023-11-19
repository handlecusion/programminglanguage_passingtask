#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "calculer.h"

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define SEMI_COLON 27
#define SPACE 9

/* Struct define */
typedef struct
{
    char    *name;
    int     value;
} Var;

/* Global declarations */
/* Variables */
int 	charClass;
char 	lexeme[100];
char 	nextChar;
int 	lexLen;
int 	token;
int 	nextToken;
FILE 	*in_fp, *fopen();

int		cntId, cntConst, cntOp;
int		correct;
char	formular;

/* Function declarations */
int     lookup(char ch);
void    addChar();
void    getChar();
void    getNonBlank();
int     lex();

Var var[100];

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
*/

#endif
