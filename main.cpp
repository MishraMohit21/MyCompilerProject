#include "lexer.h"

std::string GetCodeFromFile(std::ifstream& file){

    std::ostringstream ss;
    ss << file.rdbuf(); 
    return ss.str();

}



int main(int argc, char* argv[]){

    if (argc != 2){
        std::cout << "Usage: " << argv[0] << " <filename.new>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Failed to open file\n";
        return 1;
    }

    std::string code = GetCodeFromFile(file);

    std::cout << code;

    Lexer lexy;

    lexy.StartLexing(code);

    return 0;
}