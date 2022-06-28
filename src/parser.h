#ifndef PARSER_H
#define PARSER_H

#include <iostream>
extern "C" {
    #include <tree_sitter/api.h>
}

extern "C" TSLanguage * tree_sitter_python();

using namespace std;

enum Language {
  PYTHON,
  C
};

class Parser {
    public:
        Parser(Language lang);
        TSTree* parse_file(string& file);
        string read_file(const string& path);
        string get_identifier(uint32_t start, uint32_t end);
    private:
        TSParser *parser;
        string cur_code;
};

#endif