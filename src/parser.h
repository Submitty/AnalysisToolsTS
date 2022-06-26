#include <iostream>
extern "C" {
    #include <tree_sitter/api.h>
}

extern "C" TSLanguage * tree_sitter_python();

using namespace std;

#ifndef PARSER_H
#define PARSER_H

enum Language {
  PYTHON,
  C
};

class Parser {
    public:
        Parser(Language lang);
        TSTree* parse_file(string& file);
        string read_file(const string& path);
    private:
        TSParser *parser;
};

#endif