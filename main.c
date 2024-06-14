#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum {
  
  INT,
  
}TypeLiterals;

typedef enum {

  EXIT,

}TypeKeywords;

typedef enum {

    SEMI,
    OPEN_PAREN,
    CLOSE_PAREN,

}TypeSeperator;

typedef struct {

    TypeLiterals type;
    int value;

}TokenLiterals;

typedef struct {

    TypeSeperator type;

}TokenSeperator;

typedef struct {

    TypeKeywords type;

}TokenKeyword;

TokenLiterals generateNumber(char ch, FILE* file)
{
    TokenLiterals token;
    token.type = INT;
    int num = 0, test;
    while (ch != EOF)
    {
        if (!isdigit(ch)){
            fseek(file, -1, SEEK_CUR);
            break;
        }
        test = (int)(ch - '0');
        // printf("Test: %d\n", test);
        num = num * 10 + test;
        ch = fgetc(file);
    }

    token.value = num;
    return token;
}

void lexer(FILE *file)
{

    char ch = fgetc(file);
    while (ch != EOF)
    {
       
       if (ch == ';'){
        printf ("FOUND SEMICOLON\n");
       }else if (ch == '('){
        printf ("FOUND OPEN PAREN\n");
       }else if (ch == ')')
       {
        printf ("FOUND CLOSE PAREN\n");
       }else if (isdigit(ch)){
        
        TokenLiterals Test_Token = generateNumber(ch, file);
        //printf ("FOUND NUMBER: %d\n", (int)(ch - '0'));
        printf("FOUND NUMBER: %d\n", Test_Token.value);
       }else if(isalpha(ch)){
        printf("FOUND ALPHABET: %c\n", ch);
       }
       
       ch = fgetc(file);

    }

}

int main()
{

    FILE *file;
    file = fopen("test.new", "r");

    lexer(file);

}