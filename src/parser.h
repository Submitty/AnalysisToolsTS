#ifndef PARSER_H
#define PARSER_H

#include <iostream>
extern "C" {
    #include <tree_sitter/api.h>
}

enum Language {
  PYTHON,
  C
};

class Parser {
    public:
        Parser(Language lang);
        TSTree* parse_file(std::string& file);
        std::string read_file(const std::string& path);
        std::string get_identifier(uint32_t start, uint32_t end);
    private:
        TSParser *parser;
        std::string cur_code;
};

#endif