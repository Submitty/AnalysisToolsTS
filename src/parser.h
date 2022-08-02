#ifndef PARSER_H
#define PARSER_H

#include <iostream>
extern "C" {
#include <tree_sitter/api.h>
}

enum Language { PYTHON, C, CPP };

class Parser {
public:
  explicit Parser(const Language &lang);
  TSTree *parse_file(const std::string &file);
  std::string read_file(const std::string &path);
  std::string get_identifier(int start, int end);

private:
  TSParser *parser;
  std::string cur_code;
};

#endif