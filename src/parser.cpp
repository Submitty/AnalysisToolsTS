#include <iostream>
#include "parser.h"
#include <fstream>
extern "C" {
    #include <tree_sitter/api.h>
}
extern "C" TSLanguage * tree_sitter_python();

Parser::Parser(Language lang) {
    parser = ts_parser_new();
    if (lang == PYTHON) {
        ts_parser_set_language(parser, tree_sitter_python());
    }
}

TSTree* Parser::parse_file(std::string& file) {
    cur_code = read_file(file);
    TSTree *tree = ts_parser_parse_string(
            parser,
            NULL,
            cur_code.c_str(),
            cur_code.length()
    );
    return tree;
}

std::string Parser::read_file(const std::string& path) {
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
             << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

std::string Parser::get_identifier(uint32_t start, uint32_t end) {
    char const *c = cur_code.c_str();
    std::string str(c + start, c + end);
    return str;
}