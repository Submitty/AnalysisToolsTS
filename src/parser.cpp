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
    string source_code = read_file(file);
    TSTree *tree = ts_parser_parse_string(
            parser,
            NULL,
            source_code.c_str(),
            source_code.length()
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