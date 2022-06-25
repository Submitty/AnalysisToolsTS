#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>


extern "C" {
    #include <tree_sitter/api.h>
}

extern "C" TSLanguage * tree_sitter_python();


using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;

string readFileIntoString(const string& path) {
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}


int main()
{
    TSLanguage *tree_sitter_python();
    string filename("solution.py");
    string source_code;

    source_code = readFileIntoString(filename);
    TSParser *parser = ts_parser_new();
    int count = 0;
    ts_parser_set_language(parser, tree_sitter_python());
    TSTree *tree = ts_parser_parse_string(
        parser,
        NULL,
        source_code.c_str(),
        source_code.length()
    );
    TSNode root_node = ts_tree_root_node(tree);
    TSTreeCursor cursor = ts_tree_cursor_new(root_node);
    while (true)
    {
        bool same_depth = false;
        if (ts_tree_cursor_goto_first_child(&cursor)) {
          same_depth = true;
        }
        else if (ts_tree_cursor_goto_next_sibling(&cursor)) {
          same_depth = true;
        }
        if (same_depth) {
          TSNode cur = ts_tree_cursor_current_node(&cursor);
          if (!strcmp("=", ts_node_type(cur))) {
            count += 1;
          }
          continue;
        }
        bool had_sibling = true;
        ts_tree_cursor_goto_parent(&cursor);
        while(!ts_tree_cursor_goto_next_sibling(&cursor)) {
          if (!ts_tree_cursor_goto_parent(&cursor)) {
            had_sibling = false;
            break;
          }
        }
        TSNode cur = ts_tree_cursor_current_node(&cursor);
        if(had_sibling && !strcmp("=", ts_node_type(cur))) {
          count += 1;
        }
        if (ts_node_is_null(ts_node_parent(cur))) {
          break;
        }
    }
    cout << count << "\n";
    char *string = ts_node_string(root_node);
    exit(EXIT_SUCCESS);
}
