#include "parser.h"
#include <fstream>
extern "C" {
    #include <tree_sitter/api.h>
}

Parser::Parser(Language lang) {
    parser = ts_parser_new();
    if (lang == PYTHON) {
        ts_parser_set_language(parser, tree_sitter_python());
    }
}

TSTree* Parser::parse_file(string& file) {
    cur_code = read_file(file);
    TSTree *tree = ts_parser_parse_string(
            parser,
            NULL,
            cur_code.c_str(),
            cur_code.length()
    );
    return tree;
}

string Parser::read_file(const string& path) {
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    return string((istreambuf_iterator<char>(input_file)), istreambuf_iterator<char>());
}

string Parser::get_identifier(uint32_t start, uint32_t end) {
    char const *c = cur_code.c_str();
    string str(c + start, c + end);
    return str;
}