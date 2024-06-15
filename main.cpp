#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unordered_map>

// Extra Structures

typedef struct {
    std::string word = "";
    bool KeyOrNot = false;
} TempKeyWrd;

// Compiler enums and structure 


typedef enum {
    INT,
} TypeLiterals;

typedef enum {
    EXIT,
    FOR, 
    WHILE,
} TypeKeywords;

typedef enum {
    SEMI,
    OPEN_PAREN,
    CLOSE_PAREN,
} TypeSeperator;

typedef struct {
    TypeLiterals type;
    int value;
} TokenLiterals;

typedef struct {
    TypeSeperator type;
} TokenSeperator;

typedef struct {
    TypeKeywords type;
} TokenKeyword;

const std::unordered_map<std::string, TypeKeywords> stringToEnumMap = {
    {"exit", EXIT},
    {"while", WHILE},
    {"for", FOR}
};

// Compiler functions
TokenLiterals generateNumber(char& ch, std::ifstream& file) {
    TokenLiterals token;
    token.type = INT;
    int num = 0, test;
    while (ch != EOF && isdigit(ch)) {
        test = (int)(ch - '0');
        num = num * 10 + test;
        ch = file.get();
    }
    if (!isdigit(ch)) {
        file.unget();
    }
    token.value = num;
    return token;
}

bool checkKeyWord(std::string word){

    return stringToEnumMap.find(word) != stringToEnumMap.end();

}

TempKeyWrd generateWord(char& ch, std::ifstream& file) {
    TempKeyWrd return_word;
    while (ch != EOF && isalpha(ch)) {
        return_word.word += ch;
        ch = file.get();
    }
    if (!isalpha(ch)) {
        file.unget();
    }

    return_word.KeyOrNot = checkKeyWord(return_word.word);

    return return_word;
}

void lexer(std::ifstream& file) {
    char ch = file.get();
    while (ch != EOF) {
        if (ch == ';') {
            std::cout << "FOUND SEMICOLON\n";
        } else if (ch == '(') {
            std::cout << "FOUND OPEN PAREN\n";
        } else if (ch == ')') {
            std::cout << "FOUND CLOSE PAREN\n";
        } else if (isdigit(ch)) {
            TokenLiterals Test_Token = generateNumber(ch, file);
            std::cout << "FOUND NUMBER: " << Test_Token.value << "\n";
        } else if (isalpha(ch)) {
            
            TempKeyWrd word = generateWord(ch, file);
            
            // std::cout << "FOUND WORD: " << word << "\n";
            if (word.KeyOrNot){
                std::cout << "FOUND KEYWORD: " << word.word << "\n";
            }else{
                std::cout << "FOUND WORD: " << word.word << "\n";
            }


        }
        ch = file.get();
    }
}

int main() {
    std::ifstream file("test.new");
    if (!file) {
        std::cerr << "Failed to open file\n";
        return 1;
    }
    lexer(file);
    return 0;
}
