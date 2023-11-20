# pml

### programming assignment #1

### develop environment
**OS** : ubuntu, macOS  
**Compiler** : gcc

### how to execute
1. enter **make** in current directory
```
make
```
2. execute **a.exe with textfile**(ex. test.txt)
```
./a.exe test.txt
```

### explain implementation
**parser.c and parser.h**
> parser는 Recursive Descent Parsing 기법을 통해 과제에서 제공된 문법으로 parsing을 진행한다.  
> parser.h는 include, global variable, struct definition, function declaration 을 포함한다.  
> parser.h에서 declaration한 부분들은 parser.c에 구체적으로 구현되어 있다.  

**calculer.c and calculer.h**
> calculer는 문자열 formular를 받아서 식을 계산하는 함수이다.  
> 비슷한 기능을 하는 함수로는 python의 eval function이 있다.  
> .c와 .h의 구조는 parser와 동일하다.  

### error, warning process
statements에서 세미콜론(;)을 만나 처리하는 부분에 오류 메시지를 띄워주도록 했다. 에러가 없으면 (OK), 있다면 해당 에러에 맞는 메시지를 출력했다.  
밑으로 err 1번 케이스, 2번, 3번을 설명하겠다.  
```c
void statements() {
    statement();
    if (nextToken == SEMI_COLON) {
		/*
		*	other content
		*/
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
		/*
		*	other content
		*/
    }
}
```

1. duplication operator
operator을 처리하는 부분(term_tail, factor_tail, etc.)에서 lexeme의 길이를 측정하고 1이 아니라면 중복된 operator이므로 global variable인 err을 1로 수정하여 에러임을 알렸다.
```c
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
```
```c
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
```

2. wrong identifier name
c언어의 명명규칙에서 어긋나는 방향으로 identifier의 name이 들어오면 앞에 기호나 숫자 부분을 무시하고 읽도록 구현했다.
다시 규칙에 맞는 name을 받고 ident function을 다시 호출했다.
```c
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
```

3. not defined identifier
정의되지 않은 identifier에 접근하려고 하면 null value를 사용했다.
```c
void factor() {
		/*
		*	other content
		*/
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
	}
		/*
		*	other content
		*/
}
```

searchValue에서 value가 NULL일 때 처리를 if 에 조건을 더 붙여서 해주었다.
```c
char	*searchValue(char *s)
{
	for (int i = 0; i < varLen; i++)
	{
		if (!strcmp(s, var[i].name) && var[i].value != NULL)
			return (strdup(var[i].value));
	}
	return NULL;
}
```

statement function에서 nextToken이 ;이면 formular을 calculer에 있는 evaluate에 parameter로 넣어주면 value를 얻을 수 있는데 NULL 값이 포함되어 있으면 계산할 수 없으므로 위에서 넣은 $기호가 있으면 value에 NULL을 넣어주었다.
```c
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
```
